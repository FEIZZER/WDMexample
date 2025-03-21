#pragma once
#include <mutex>
#include <shared_mutex>
#include <queue>

namespace ez {

template <typename T>
class queue
{
public:
	~queue() = default;
	explicit queue() : max_size_(1000) {}
	explicit queue(std::size_t max) : max_size_(max) {}

	queue(const queue&) = delete;
	queue& operator=(const queue&) = delete;

	bool push_back(const T& item)
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);

		if (queue_.size() >= max_size_)
		{
			return false;
		}
		queue_.push(item);
		return true;
	}

	bool push_back(T&& item)
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);

		if (queue_.size() >= max_size_)
		{
			return false;
		}
		queue_.push(item);
		return true;

	}

	bool pop_front(T& item)
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);

		if (queue_.empty())
		{
			return false;
		}
		item = queue_.front();
		queue_.pop();
		return true;
	}
	bool back(T& item)
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);
		if (queue_.empty())
		{
			return false;
		}
		item = queue_.back();
		return true;
	}
	bool front(T& item)
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);
		if (queue_.empty())
		{
			return false;
		}
		item = queue_.front();
		return true;
	}
	bool empty()
	{
		return queue_.empty();
	}

	std::size_t size()
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);
		return queue_.size();
	}

private:


private:
	std::size_t					max_size_;
	mutable std::shared_mutex	mutex_;
	std::queue<T>				queue_;
};

}

