#pragma once

#include <map>
#include <mutex>
#include <shared_mutex>

namespace ez {

template<typename _Kty, typename _Ty, class _Pr = std::less<_Kty>, class _Alloc = std::allocator<std::pair<const _Kty, _Ty>> >
class safemap
{
public:
	using read_lock = std::shared_lock<std::shared_mutex>;
	using write_lock = std::unique_lock<std::shared_mutex>;
	using key_type = _Kty;
	using item_type = _Ty;

	safemap() = default;
	~safemap() {
		write_lock lock(mu_);
		map_.clear();
	}

	bool find_item(const key_type& key, item_type& item)
	{
		std::unique_lock<std::shared_mutex> lock(mu_);

		return true;
	}


private:
	std::shared_mutex					mu_;
	std::map<_Kty, _Ty, _Pr, _Alloc>	map_;
};


}