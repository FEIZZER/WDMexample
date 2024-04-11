// OutputLog.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//



#include "log.h"
#include "flog.h"

int main()
{
	F_spdlog::initLog("spdlog.log");
	F_spdlog::info("Hello {}", "World");
	F_spdlog::error("Hello {}", "World");

	f_info("test:", 1, "hello");

}
