#include "ez_shared_memory/shared_memory.h"

#include <windows.h>

using namespace ez;

shared_memory::shared_memory(const std::string& fm_name, unsigned long long size)
{
	do {
		fm_handle_ = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, fm_name.c_str());
		if (fm_handle_ == nullptr)  break;

		fm_memory_ = MapViewOfFile(fm_handle_, FILE_MAP_ALL_ACCESS, 0, 0, size);
		if (fm_memory_ == nullptr)  break;

		size_ = size;
		fm_name_ = fm_name;
		inited_.store(true);
	} while (false);

	if (!inited_.load())
	{
		if (fm_memory_ != nullptr) {
			UnmapViewOfFile(fm_memory_); 
			fm_memory_ = nullptr;
		}
		if (fm_handle_ != nullptr) {
			CloseHandle(fm_handle_);
			fm_handle_ = nullptr;
		}
	}

}

shared_memory::~shared_memory()
{
	if (fm_memory_ != nullptr){
		UnmapViewOfFile(fm_memory_);
		fm_memory_ = nullptr;
	}

	if (fm_handle_ != nullptr) {
		CloseHandle((HANDLE)fm_handle_);
		fm_handle_ = nullptr;
	}
}

bool shared_memory::read(void* buffer, unsigned long long size)
{
	if (buffer == nullptr) {
		return false;
	}
	// read lock
	memcpy(buffer, fm_memory_, size);
}

bool shared_memory::write(void* buffer, unsigned long long size)
{
	if (buffer == nullptr) {
		return false;
	}
	// write lock
	memcpy(fm_memory_, buffer, size);
}