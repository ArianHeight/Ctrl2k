#pragma once
#include <type_traits>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <queue>
#include <functional>
#include "Logger.h"

#define LOAD_FAIL_VALUE 0
typedef int AssetId;

enum AssetState
{
	ASSET_LOADING_QUEUED,		//When the texture is loading in the background
	ASSET_LOADED,				//Texture has been loaded
	ASSET_DELETE_QUEUED,		//Texture is queued for delete
	ASSET_DELETED,				//Texture has been deleted
	ASSETSTATE_SIZE				//TEXSTATE_SIZE must remain the final member here
};

struct AssetMetaDataTemplate
{
	AssetId id;
};

struct AssetTemplate
{
	AssetState state;
	FilePath path;
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
	std::unordered_map<FileNameNoExt, TexId> nameMap;
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
	AssetId loadAssetInternal(const FilePath& path, std::function<void(AssetData&, AssetMetaData&)> loadFunc)
	{
		AssetId id = -1;
		//initial load request
		{
			std::lock_guard<std::mutex> lock(assetMutex);
			std::string name = FilePath_GetFileNameNoExt(path);
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
				LOG_WARNING_PUSH("Trying to unload asset " + FilePath_GetFileNameNoExt(assets[id].path) + ", an already unloaded asset");
				return;
			}
			else if(assets[id].refcount == 0)
			{
				LOG_FATAL_PUSH("Asset " + FilePath_GetFileNameNoExt(assets[id].path) + " has refcount 0 but is being requested to unload");
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