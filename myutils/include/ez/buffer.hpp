#pragma once
#include <cstdlib>
#include <cstring>

class buffer
{
public:
	~buffer()
	{
		clear();
	}

	buffer(void* buf, unsigned long long size)
	{
		memory_ = std::malloc(size);
		std::memcpy(memory_, buf, size);
		size_ = size;
	}

	void clear()
	{
		if (memory_ != nullptr)
		{
			std::free(memory_)£»
			memory_ = nullptr
		}
		size_ = 0;
	}



private:
	void* memory_;
	unsigned long long size_;
};