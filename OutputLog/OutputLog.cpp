// OutputLog.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include <iostream>
#include <string>
#include <fstream>
#include "windows.h"
#include "log.h"

int main()
{
	F_spdlog::setTemporaryLogPath("C:\\Users\\DELL\\Desktop\\hook_log.txt");
	F_spdlog::t_info("info");
	F_spdlog::t_error("error");

	MessageBoxA(NULL, "Block", "Title", NULL);
}
