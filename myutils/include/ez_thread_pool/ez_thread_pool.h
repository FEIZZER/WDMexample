#pragma once
// deprecated,  use ez_thread_pool.hpp instead
#include "ez_queue/ez_queue.h"

#include <vector>
#include <thread>
#include <future>
#include <functional>
#include <type_traits>

template<typename T, typename F, 
typename = typename std::enable_if_t<std::is_same_v<F, void(*)(T&)>> >
class ez_thread_pool
{
public:
	using item_type = T;
	using function_type = F;
	using task_type = std::function<void()>;

	ez_thread_pool() = delete;
	~ez_thread_pool()
	{
		try {
			ez_thread_pool::shutdown();
			for (auto& thread: threads_)
			{
				thread.join();
			}
		}
		catch(...){
			// todo
		}
	}

	explicit ez_thread_pool(function_type f, std::size_t max_item, std::size_t thread_num)
		: func_(f), queue_(max_item)
	{
		if (func_ == nullptr)
		{
			throw std::exception("ez_thread_pool(): invalid function_ptr");
		}
		if (thread_num <= 0 || thread_num >= 1000)
		{
			throw std::exception("ez_thread_pool(): invalid thread_num, range is [1, 1000], default is 10");
		}

		for (int i = 0; i < thread_num; i++)
		{
			threads_.emplace_back([this] {
				this->worker_loop();
				});
		}
	}
	explicit ez_thread_pool(function_type f, std::size_t max_item) : ez_thread_pool(f, max_item, 10) {}
	explicit ez_thread_pool(function_type f) : ez_thread_pool(f, 1000) {}

	void shutdown()
	{
		exit_.store(true);
	}
	bool post_work_item(const item_type& item)
	{
		return queue_.push_back(item);
	}

	template<typename F>
	bool post_work_task(F& f) // -> std::future<decltype(f(args...))>
	{
		return true;
	}

private:
	void worker_loop() 
	{
		while (process_next_item() && !exit_.load()) {}
	}
	bool process_next_item()
	{
		item_type item;
		if (!queue_.pop_front(item))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			return true;
		}

		func_(item);

		return true;
	}

private:
	std::atomic_bool			exit_ = false;
	ez_queue<item_type>			queue_;
	// ez_queue<>
	function_type				func_ = nullptr;
	std::vector<std::thread>	threads_;
};