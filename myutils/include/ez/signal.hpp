#pragma once
#include <mutex>
#include <chrono>
#include <atomic>
#include <condition_variable>
namespace ez {

class signal
{
public:
	signal()
	{
		re_ = true;
		sg_ = false;
	}

	~signal() = default;

	void wait(){
		std::unique_lock<std::mutex> lock(mu_);
		while (!sg_) {
			cv_.wait(lock);
		}
		if(re_)
			sg_ = false;
	}


	template <class _Rep, class _Period>
	bool wait_for(const std::chrono::duration<_Rep, _Period>& wait_time) {
		bool no_timeout;
		auto dead_line = std::chrono::system_clock::now() + wait_time;
		std::unique_lock<std::mutex> lock(mu_);
		while (!sg_) {
			no_timeout = cv_.wait_until(lock, dead_line) == std::cv_status::no_timeout;
			if (!no_timeout) {
				break;
			}
		}
		if (no_timeout && re_)  
			sg_ = false;
		
		return no_timeout;
	}

	void notify_all() {
		{
			std::lock_guard<std::mutex> lock(mu_);
			sg_ = true;
			re_ = false;
		}
		cv_.notify_all();
	}

	void notify_one(){
		{
			std::lock_guard<std::mutex> lock(mu_);
			sg_ = true;
		}
		cv_.notify_one();
	}

	void reset() {
		std::lock_guard<std::mutex> lock(mu_);
		sg_ = false;
		re_ = true;
	}


private:
	bool					re_;
	std::mutex				mu_;
	std::atomic_bool		sg_;
	std::condition_variable cv_;
};
}