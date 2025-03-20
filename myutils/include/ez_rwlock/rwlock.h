#pragma once
#include <string>

namespace ez {

class rwlock
{
public:
	rwlock(const std::string& file_name);
	~rwlock();

	void read_lock();
	void read_unlock();

	void write_lock();
	void write_unlock();

private:
	void*		file_handle_;
	std::string file_name_;

};

}