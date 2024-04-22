#pragma once
#include <queue>
#include <mutex>
#include <functional>

namespace core
{
	template <typename T>
	class ThreadSafeQueue
	{
	private:
		std::mutex m;
		std::queue<T> data;

	public:
		ThreadSafeQueue() {}
		~ThreadSafeQueue() {}

		void readData(std::function<void(const std::vector<T>&)> readFunc) const
		{
			std::unique_lock<std::mutex> scopedLock(m);
			readFunc(data);
		}

		const size_t size() const
		{
			std::unique_lock<std::mutex> scopedLock(m);
			return data.size();
		}

		const bool empty() const
		{
			std::unique_lock<std::mutex> scopedLock(m);
			return data.size();
		}

		void modifyData(std::function<void(std::queue<T>)&> modifyFunc)
		{
			std::unique_lock<std::mutex> scopedLock(m);
			modifyFunc(data);
		}

		void push(const T& item)
		{
			std::unique_lock<std::mutex> scopedLock(m);
			data.push(item);
		}

		void push(T&& item)
		{
			std::unique_lock<std::mutex> scopedLock(m);
			data.push(item);
		}

		void pop()
		{
			std::unique_lock<std::mutex> scopedLock(m);
			data.pop();
		}
	};
}