#pragma once
#include <string>
#include <atomic>
namespace ez {

class shared_memory
{
public:
	shared_memory(const std::string& fm_name, unsigned long long size);
	~shared_memory();

	bool read(void* buffer, unsigned long long size);
	bool write(void* buffer, unsigned long long size);

private:
	std::atomic_bool	inited_;

	void*				fm_handle_;
	void*				fm_memory_;
	std::string			fm_name_;
	unsigned long long	size_;
};

}