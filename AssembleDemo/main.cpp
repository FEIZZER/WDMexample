
#include <iostream>
#include <string>
#include <stdio.h>
#include <boost/filesystem.hpp>
#include <fstream>



int main()
{
	boost::filesystem::wpath path(L"C:\\Users\\DELL\\Desktop\\test\\test");
	try
	{
		std::fstream f;
		f.open("C:\\Users\\DELL\\Desktop\\test\\test");
		f.close();
		int res = boost::filesystem::remove_all(path.parent_path());
		printf("delete:%d", res);
		boost::filesystem::remove(path);
	}
	catch (const std::exception& e)
	{
		printf("error:%s", e.what());
	}
}