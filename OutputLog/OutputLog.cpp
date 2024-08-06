// OutputLog.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include "log.h"
#include <iostream>

#define	NO_LOG	
#define INFO_LOG(...)	if ( F_spdlog::getInstance()->f_file_logger != nullptr) \
							SPDLOG_LOGGER_INFO(F_spdlog::getInstance()->f_file_logger, __VA_ARGS__)

int main()
{
	std::string name = "feizzer";
	std::string str(name, 1000);
	std::cout << str;
}
