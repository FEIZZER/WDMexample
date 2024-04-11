#pragma once
#ifndef FLOG
#define FLOG

#include <string>
#include <fstream>

const static std::string log_file_path = "C:\\Users\\DELL\\Desktop\\hook_log.txt";

template<typename T, typename... Args>
void print(std::fstream& fs, const T& t)
{
	fs << t;
}

template<typename T, typename... Args>
void print(std::fstream& fs, const T& t, const Args&... rest)
{
	fs << t;
	print(fs, rest...);
}

template<typename... Args>
void f_info(const std::string& fmt, Args... args)
{
	std::fstream fs;
	fs.open(log_file_path, std::ios::app);

	print(fs, fmt, args...);

	fs << std::endl;

	fs.close();
}

#endif // F_LOG