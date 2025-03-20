#pragma once
#include <list>
#include <xmemory>
#include <mutex>
#include <shared_mutex>

namespace ez {

template <class _Ty, class _Alloc = std::allocator<_Ty>>
class list
{
public:
	using read_lock = std::shared_lock<std::shared_mutex>;
	using write_lock = std::unique_lock<std::shared_mutex>;

	list() = default;
	~list() {
		write_lock lock(shared_mutex_);
		list_.empty();
	}

	bool push_back(const _Ty& item) {
		write_lock lock(shared_mutex_);
		list_.push_back(item);
		return true;
	}

	bool push_back(_Ty&& item) {
		write_lock lock(shared_mutex_);
		list_.push_back(std::move(item));
		return true;
	}

	bool push_front(const _Ty& item) {
		write_lock lock(shared_mutex_);
		list_.push_front(item);
		return true;
	}

	bool push_front(_Ty&& item) {
		write_lock lock(shared_mutex_);
		list_.push_front(std::move(item));
		return true;
	}


	bool pop_back(_Ty& item) {
		write_lock lock(shared_mutex_);
		if (list_.empty()) {
			return false;
		}
		item = std::move(list_.back());
		list_.pop_back();
		return true;
	}

	bool pop_front(_Ty& item){
		write_lock lock(shared_mutex_);
		if (list_.empty()) {
			return false;
		}
		item = std::move(list_.front());
		list_.pop_front();
		return true;
	}


	bool empty(){
		read_lock lock(shared_mutex_);
		return list_.empty();
	}

	std::size_t size() {
		read_lock lock(shared_mutex_);
		return list_.size();
	}

private:
	std::list<_Ty, _Alloc>	list_;
	std::shared_mutex		shared_mutex_;
};


}