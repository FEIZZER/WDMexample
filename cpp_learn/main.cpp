#include "stl/auto_ptr.h"
#include "BaseClass.h"
#include <string>
#include <time.h>
#include <iostream>
#include <thread>
#include <Windows.h>
#include <vector>
#include <array>
#include <fstream>
#include "stl/template.h"
#include "stl/map.h"
#include "stl/std_string.h"
#include <mutex>         //unique_lock
#include <shared_mutex>  //shared_mutex shared_lock
#include "stl/TestClass.h"


void run()
{
	std::cout << "Done!" << std::endl;
}


int main()
{
	auto thread = std::thread(run);

	std::cout << "main" << std::endl;

	Sleep(2000);

	thread.join();

	std::cout << "main exit" << std::endl;

	return 0;
}