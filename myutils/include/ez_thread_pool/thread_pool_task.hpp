#pragma once

#include <functional>
#include <future>

class ez_thread_pool_task
{
public:
	ez_thread_pool_task();
	~ez_thread_pool_task();

	template<typename F, typename ...Args>
	auto post_work_task(F& f, Args&& ...args) -> std::future<decltype(f(args...))>
	{
		
	}


private:

};