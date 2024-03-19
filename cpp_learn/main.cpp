#include "stl/auto_ptr.h"
#include <string>
#include <fstream>
#include <iostream>
#include <Windows.h>
void test(SIZE_T size)
{
	std::cout << size;
}

int main()
{
	// unique_ptr_test();
	test(500 * 1024 * 1024);

}