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



void run()
{
	std::cout << "Done!" << std::endl;
}


int main()
{
	int a = 0x123456;

	std::cout << "a:" << a << std::endl;

	MessageBoxA(NULL, "this is secret, not read it", "warning", NULL);

	return 0;
}