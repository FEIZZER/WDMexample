#pragma once

#include <map>
#include <mutex>
#include <shared_mutex>

template<typename _Kty, typename _Ty, class _Pr = std::less<_Kty>, class _Alloc = std::allocator<std::pair<const _Kty, _Ty>> >
class ez_map
{
public:
	using read_lock		= std::shared_lock<std::shared_mutex>;
	using write_lock	= std::unique_lock<std::shared_mutex>;

	ez_map() = default;
	~ez_map() 
	{
		write_lock lock;
		map_.clear();
	}

private:

	std::shared_mutex					mutex_;
	std::map<_Kty, _Ty, _Pr, _Alloc>	map_;
  
};