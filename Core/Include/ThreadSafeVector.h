#pragma once
#include <vector>
#include <mutex>
#include <functional>

namespace core
{
	template <typename T>
	class ThreadSafeVector
	{
	private:
		std::mutex m;
		std::vector<T> data;

	public:
		ThreadSafeVector() {}
		~ThreadSafeVector() {}

		void readData(std::function<void(const std::vector<T>&)> readFunc) const
		{
			std::unique_lock<std::mutext> scopedLock(m);
			readFunc(data);
		}

		const size_t capacity() const
		{
			std::unique_lock<std::mutext> scopedLock(m);
			return data.capacity();
		}

		const size_t size() const
		{
			std::unique_lock<std::mutext> scopedLock(m);
			return data.size();
		}

		const bool empty() const
		{
			std::unique_lock<std::mutext> scopedLock(m);
			return data.empty();
		}

		void modifyData(std::function<void(std::vector<T>)&> modifyFunc)
		{
			std::unique_lock<std::mutext> scopedLock(m);
			modifyFunc(data);
		}

		void reserve(size_t n)
		{
			std::unique_lock<std::mutext> scopedLock(m);
			data.reserve(n);
		}

		void push_back(const T& newItem)
		{
			std::unique_lock<std::mutext> scopedLock(m);
			data.push_back(newItem);
		}

		void push_back(T&& newItem)
		{
			std::unique_lock<std::mutext> scopedLock(m);
			data.push_back(newItem);
		}

		void pop_back()
		{
			std::unique_lock<std::mutext> scopedLock(m);
			data.pop_back();
		}
	};
}