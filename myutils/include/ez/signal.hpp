#pragma once
#include <mutex>
#include <chrono>
#include <atomic>
#include <condition_variable>
namespace ez {

class signal
{
public:
	signal() = default;
	~signal() = default;

	void wait(){
		std::unique_lock<std::mutex> lock(mu_);
		cv_.wait(lock);
	}

	template<class _Predicate>
	void wait(_Predicate pred) {
		std::unique_lock<std::mutex> lock(mu_);
		cv_.wait(lock, pred);
	}

	template <class _Rep, class _Period>
	bool wait_for(const std::chrono::duration<_Rep, _Period>& wait_time) {
		std::unique_lock<std::mutex> lock(mu_);
		auto status = cv_.wait_for(lock, wait_time);
		return status == std::cv_status::no_timeout;
	}

	template <class _Rep, class _Period, class _Predicate>
	bool wait_for(const std::chrono::duration<_Rep, _Period>& wait_time, _Predicate pred) {
		std::unique_lock<std::mutex> lock(mu_);
		return cv_.wait_for(lock, wait_time, pred);
	}

	void notify_all() {
		cv_.notify_all();
	}

	void notify_one(){
		cv_.notify_one();
	}


private:
	std::mutex				mu_;
	std::atomic_bool		sig_;
	std::condition_variable cv_;
};
}