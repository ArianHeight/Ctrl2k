#pragma once
#include <type_traits>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <queue>
#include <functional>
#include <future>
#include <atomic>
#include <thread>
#include "Core/CtrlMiscUtils/ThreadSafeVector.h"
#include "Core/CtrlMiscUtils/ThreadSafeQueue.h"
#include "Core/GreatBath/Logger.h"

#define LOAD_FAIL_VALUE 0
typedef int AssetId;

enum AssetState
{
	ASSET_LOADING_QUEUED,		//When the asset is loading in the background
	ASSET_LOADED,				//Asset has been loaded
	ASSET_DELETE_QUEUED,		//Asset is queued for delete
	ASSET_DELETED,				//Asset has been deleted
	ASSETSTATE_SIZE				//ASSETSTATE_SIZE must remain the final member here
};

struct AssetMetaDataTemplate
{
	AssetId id;
};

struct AssetTemplate
{
	AssetState state;
	gbt::FilePath path;
	unsigned int refcount;

	AssetTemplate() : state(AssetState::ASSET_LOADING_QUEUED), path(""), refcount(1) {}
};

#define MSG_NAMEMAP_SYNC_ISSUE "nameMap and assets are out of sync while loading " + name
#define MSG_DEL_ASSETS_SYNC_ISSUE "deletedAssets and assets are out of sync while loading " + name
#define MSG_ASSET_NAME_OVERLAP_ISSUE "name: " + name + " conflicts with existing asset"

//Internal assets manager
//only meant to be used in one single thread
//Is a singleton please refrain from making multiple instances of any children
//Multiple instances of different children are fine though
//
//Refrain from locking mutexes randomly in the children pls
//Asset MUST be a child of AssetTemplate
//AssetData MUST be a pointer to something
//AssetMetaData MUST be a child of AssetMetaDataTemplate
template <typename Asset, typename AssetData, typename AssetMetaData>
class InternalAsyncAssetsManager
{
private:
	static void forceTypeInheritance()
	{
		static_assert(std::is_base_of<AssetTemplate, Asset>::value, "Given type Asset not child of AssetTemplate");
		static_assert(std::is_base_of<AssetMetaDataTemplate, AssetMetaData>::value, "Given type AssetMetaData not child of AssetMetaDataTemplate");
		static_assert(std::is_pointer<AssetData>::value, "Given type AssetData not a pointer");
	}

protected:
	std::mutex assetMutex;
	std::vector<Asset> assets;
	std::unordered_map<gbt::FileNameNoExt, TexId> nameMap;
	std::queue<AssetId> deletedAssets;

	std::mutex loadMutex;
	std::queue<AssetData> loadData;
	std::queue<AssetMetaData> loadMetaData;

	std::mutex unloadMutex;
	std::queue<AssetId> unloadQueue;

	std::mutex assetCountMutex;
	unsigned int assetsLoaded;

	InternalAsyncAssetsManager() : assetsLoaded(0) {}

	~InternalAsyncAssetsManager()
	{
		
	}

	//returns true if id exists
	//not thread-safe
	bool exists(AssetId id)
	{
		return (id >= 0 && id < assets.size());
	}

	//Loads the asset, meant to be used on a loading thread(ie not the same thread the assets actually belong to)
	//Return the AssetId of the asset
	//Thread-safe
	//loadFunc loads data into AssetData and AssetMetaData, inputs are the data and metadata that need to be loaded into, 
	//	has no mutex locked
	//This is the portion of loading done by the loader threads
	AssetId loadAssetInternal(const gbt::FilePath& path, std::function<void(AssetData&, AssetMetaData&)> loadFunc)
	{
		AssetId id = -1;
		//initial load request
		{
			std::lock_guard<std::mutex> lock(assetMutex);
			std::string name = path.fileNameNoExt();
			auto it = nameMap.find(name);
			if(it != nameMap.end()) //asset exists already
			{
				id = it->second;
				if(!exists(id))
				{
					LOG_FATAL_PUSH(MSG_NAMEMAP_SYNC_ISSUE);
					return LOAD_FAIL_VALUE;
				}
#ifdef _DEBUG
				if(assets[id].path != path)
				{
					LOG_WARNING_PUSH(MSG_ASSET_NAME_OVERLAP_ISSUE);
					return LOAD_FAIL_VALUE;
				}
				if(assets[id].state == AssetState::ASSET_DELETED)
				{
					LOG_FATAL_PUSH(MSG_NAMEMAP_SYNC_ISSUE);
					return LOAD_FAIL_VALUE;
				}
#endif
				assets[id].refcount++; //inc ref count when asset exists

				if(assets[id].state == AssetState::ASSET_DELETE_QUEUED)
					assets[id].state = AssetState::ASSET_LOADED;
				return id; //exit here because asset is already loaded
			}
			else //asset does not exist
			{
				if(!deletedAssets.empty())
				{
					id = deletedAssets.front();
					deletedAssets.pop();

					if(!exists(id))
					{
						LOG_FATAL_PUSH(MSG_DEL_ASSETS_SYNC_ISSUE);
						return LOAD_FAIL_VALUE;
					}
#ifdef _DEBUG
					if(assets[id].state != AssetState::ASSET_DELETED)
					{
						LOG_FATAL_PUSH(MSG_DEL_ASSETS_SYNC_ISSUE);
						return LOAD_FAIL_VALUE;
					}
#endif
					assets[id] = Asset();
				}
				else
				{
					id = assets.size();
					assets.push_back(Asset());
				}
				assets[id].path = path;
				nameMap[name] = id;
			}
		} //assetMutex scope
		//At this point, the asset does not exist and needs loading

		AssetData newAsset;
		AssetMetaData metaData;
		loadFunc(newAsset, metaData);
		metaData.id = id;

		{
			std::lock_guard<std::mutex> lock(loadMutex);
			loadMetaData.push(metaData);
			loadData.push(newAsset); //ownership of the data is passed onto loadData, which will delete it when it deems necessary
		}

		return id;
	}

	//Decrements the refcount of the asset, meant to be used on a loading thread(ie not the same thread the assets actually belong to)
	//If the refcount hits zero, queue the asset id for delete
	//Thread-safe
	void unloadAssetInternal(AssetId id)
	{
		bool addToUnloadQueue = false;
		{
			std::lock_guard<std::mutex> lock(assetMutex);

			if(!exists(id))
				return;
#ifdef _DEBUG
			if(assets[id].state != AssetState::ASSET_LOADED)
			{
				LOG_WARNING_PUSH("Trying to unload asset {}, an already unloaded asset", assets[id].path.fileNameNoExtView());
				return;
			}
			else if(assets[id].refcount == 0)
			{
				LOG_FATAL_PUSH("Asset {} has refcount 0 but is being requested to unload", assets[id].path.fileNameNoExtView());
				return;
			}
#endif
			assets[id].refcount--;
			if(assets[id].refcount == 0) //signal queue for delete
			{
				assets[id].state = AssetState::ASSET_DELETE_QUEUED;
				addToUnloadQueue = true;
			}
		}

		if(addToUnloadQueue)
		{
			std::lock_guard<std::mutex> lock(unloadMutex);
			unloadQueue.push(id);
		}
	}

	//Locks the internal assets lock mutex
	void lockAssetsInternal()
	{
		assetMutex.lock();
	}

	//Unlocks the internal assets lock mutex
	void unlockAssetsInternal()
	{
		assetMutex.unlock();
	}

	//clears loadData, unloadQueue, and updates assetLoadCount
	//does internal updates
	//unloadFunc unloads data from AssetData and AssetMetaData so they can be deleted safely, has no mutex locked
	//internalLoadFunc is the portion of the loading done on the manager thread itself
	//intnernalUnloadFunc is unloading the full asset
	void updateAssetsInternal(
		std::function<void(AssetData&, AssetMetaData&)> unloadFunc, 
		std::function<void(AssetData&, AssetMetaData&)> internalLoadFunc, 
		std::function<void(AssetId)> internalUnloadFunc)
	{
		int loadCount = 0;
		int unloadCount = 0;
		{
			std::lock_guard<std::mutex> assetlock(assetMutex);

			//loads data
			{
				std::lock_guard<std::mutex> lock(loadMutex);
				while(!loadData.empty() && loadCount < 2) //2 is arbitrary TODO change to a timer based thing
				{
					AssetData& data = loadData.front();
					AssetMetaData& meta = loadMetaData.front();

					internalLoadFunc(data, meta);
					assets[meta.id].state = AssetState::ASSET_LOADED;
					unloadFunc(data, meta);

					loadData.pop();
					loadMetaData.pop();

					loadCount++;
				}
			}

			//unloads data
			{
				std::lock_guard<std::mutex> lock(unloadMutex);
				while(!unloadQueue.empty() && unloadCount < 2) //TODO replace with some sort of timer thing
				{
					AssetId& id = unloadQueue.front();
					if(assets[id].state == AssetState::ASSET_DELETE_QUEUED)
					{
						internalUnloadFunc(id);
						assets[id].state = AssetState::ASSET_DELETED;

						deletedAssets.push(id);
						unloadCount++;
					}
					unloadQueue.pop();
				}
			}
		} //end of assetLock

		//updates assetsLoaded
		{
			std::lock_guard<std::mutex> lock(assetCountMutex);
			assetsLoaded = assetsLoaded + loadCount - unloadCount;
		}
	}
};

//Asynchronous assets manager
//only meant to be used in one single thread
//Is a singleton please refrain from making multiple instances of any children
//Multiple instances of different children are fine though
//
//Refrain from locking mutexes randomly in the children pls
//Asset MUST be a child of AssetTemplate
template <typename Asset>
class AsyncAssetManager
{
protected:
	core::ThreadSafeVector<Asset> assets;
	core::ThreadSafeQueue<AssetId> unloadedAssetsQueue;
	std::atomic_int assetsLoading = 0;
	std::atomic_int assetsLoaded = 0;
	std::atomic_int assetsUnloading = 0;

	//protected cstr, this is supposed to be an abstract class
	AsyncAssetManager() {}

private:
	//only needs to be compiled does not need to be called
	static void forceTypeInheritance()
	{
		static_assert(std::is_base_of<AssetTemplate, Asset>::value, "Given type Asset not child of AssetTemplate");
	}

	inline void counterNewLoading()
	{
		assetsLoading++;
	}
	inline void counterLoadingToLoaded()
	{
		assetsLoading--;
		assetsLoaded++;
	}
	inline void counterLoadedToUnloading()
	{
		assetsLoaded--;
		assetsUnloading++;
	}
	inline void counterUnloadingToUnloaded()
	{
		assetsUnloading--;
	}
	inline void counterUnloadingToLoaded() //used for reloading
	{
		assetsUnloading--;
		assetsLoaded++;
	}

	//this is essentially a manual spin lock and will end up eating cycles if 
	//something is trying to unload an asset that is still being loaded
	//returns ASSET_LOADED if the asset state has been set to unloading successfully
	AssetState trySetStateUnloading(AssetId id)
	{
		AssetState loadState = ASSET_LOADING_QUEUED;
		for(; loadState == ASSET_LOADING_QUEUED; std::this_thread::yield()) //try to unload
		{
			assets.modifyData([&](std::vector<Asset>& data)->void
				{
					loadState = data[id].state;
					if(loadState == ASSET_LOADED) //if loaded, quickly set the asset state to delete queued
					{
						data[id].state = ASSET_DELETE_QUEUED; //this will exit the while loop but loadState will remain as loaded
						counterLoadedToUnloading();
					}
				});
		}

		return loadState == ASSET_LOADED; //true when you've successfully set the loadstate to unloading
	}

protected:
	void asyncLoadAssetInternal(AssetId id, std::function<void(Asset&)> loadFunc)
	{
		std::async([=]()->void
			{
				Asset assetLoading = Asset();

				loadFunc(assetLoading);

				assets.modifyData([&](std::vector<Asset>& data)->void
					{
						data[id] = std::move(assetLoading); //move data
						//set loaded state
						data[id].state = ASSET_LOADED;
						counterLoadingToLoaded();
					});
			});
	}

	AssetId asyncLoadAsset(std::function<void(Asset&)> loadFunc)
	{
		//create new asset slot
		AssetId newAssetId = 0;
		bool useUnloadedSlot = false;
		//check if there's an unloaded slot open
		unloadedAssetsQueue.modifyData([&](std::queue<AssetId>& unloadedQueue)->void
			{
				useUnloadedSlot = !unloadedQueue.empty();
				newAssetId = unloadedQueue.front();
				unloadedQueue.pop();
			});
		//flag start loading
		assets.modifyData([&](std::vector<Asset>& data)->void
			{
				if(useUnloadedSlot) //reset the asset if using an unloaded slot
					data[newAssetId] = Asset(); //default should have state set to loading
				else //add a new slot
				{
					newAssetId = data.size();
					data.push_back(Asset()); //default should have state set to loading
				}
				//set loading state
				counterNewLoading();
			});

		//load asset
		asyncLoadAssetInternal(newAssetId, loadFunc);

		return newAssetId;
	}

	void asyncUnloadAsset(AssetId id, std::function<void(Asset&)> unloadFunc)
	{
		std::async([=]()->void
			{
				if(!trySetStateUnloading(id)) //early exit if unloading state couldn't get set - either asset already unloading or unloaded
					return;

				assets.modifyData([&](std::vector<Asset>& data)->void
					{
						unloadFunc(data[id]);
						//set state properly
						data[id].state = ASSET_DELETED;
						counterUnloadingToUnloaded();
					});

				unloadedAssetsQueue.push(id); //maybe move this into the clause above? not sure if necessary
			});
	}

	void asyncReloadAsset(AssetId id, std::function<void(Asset&)> unloadFunc, std::function<void(Asset&)> loadFunc)
	{
		auto fullReloadFunc = [=]()->void
		{
			if(!trySetStateUnloading(id))
				return; //cannot reload an asset being unloaded?

			//load asset
			Asset assetReloading = Asset();
			loadFunc(assetReloading);

			assets.modifyData([&](std::vector<Asset>& data)->void
				{
					unloadFunc(data[id]);
					
					data[id] = std::move(assetReloading); //move data
					//set loaded state
					data[id].state = ASSET_LOADED;
					counterUnloadingToLoaded();
				});
		};

		std::async(fullReloadFunc);
	}
};