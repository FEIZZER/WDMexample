#pragma once
#include "ez_queue/ez_queue.h"

#include <vector>
#include <thread>
#include <future>
#include <functional>
#include <type_traits>


class ez_thread_pool
{
public:
	using task_type = std::function<void()>;

	~ez_thread_pool()
	{
		try {
			ez_thread_pool::shutdown();
			for (auto& thread : threads_)
			{
				thread.join();
			}
		}
		catch (...) {
			// todo
		}
	}

	explicit ez_thread_pool(std::size_t max_item, std::size_t thread_num)
		: queue_(max_item)
	{
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
	explicit ez_thread_pool(std::size_t max_item) : ez_thread_pool(max_item, 10) {}
	explicit ez_thread_pool() : ez_thread_pool(1000) {}

	void shutdown()
	{
		exit_.store(true);
	}


	template<typename F, typename ...Args>
	auto post_work_task(F& f, Args&& ...args)  -> std::future<decltype(f(args...))>
	{
		using return_type = decltype(f(args...));

		auto packaged_task = std::make_shared<std::packaged_task<return_type()>>(std::bind(f, std::forward<Args>(args)...));

		queue_.push_back([packaged_task] {(*packaged_task)();});

		return packaged_task->get_future();
	}

private:
	void worker_loop()
	{
		while (process_next_item() && !exit_.load()) {}
	}
	bool process_next_item()
	{
		task_type task;
		if (!queue_.pop_front(task))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			return true;
		}

		task();

		return true;
	}

private:
	std::atomic_bool			exit_ = false;

	ez_queue<task_type>			queue_;

	std::vector<std::thread>	threads_;
};