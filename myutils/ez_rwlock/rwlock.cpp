#include "ez_rwlock/rwlock.h"
#include <filesystem>
#include <windows.h>

using namespace ez;


rwlock::rwlock(const std::string& file_name)
{
	if (std::filesystem::exists(file_name))
	{
		// warn
	}
	else
	{
		std::filesystem::create_directories(file_name);
	}

}

rwlock::~rwlock()
{
}



void rwlock::read_lock()
{

}

void rwlock::read_unlock()
{

}

void rwlock::write_lock()
{

}

void rwlock::write_unlock()
{

}