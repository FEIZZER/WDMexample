#include "stl/auto_ptr.h"
#include "BaseClass.h"
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include "stl/template.h"
void parameter_transfer(Base& base)
{
	std::cout << "lvalue reference" << std::endl;
}
void parameter_transfer(Base&& base)
{
	std::cout << "rvalue reference" << std::endl;
}

template<typename T>
void f1(T param) {}

template<typename T>
void f2(T& param) {}

int main()
{
#define aa "aa"
	char path[256] = {1,2,3,4,5,6,7,'a','a', 0};

	std::cout << strlen(path) << "  " << strncmp(aa, path + strlen(path) - sizeof(aa) + 1, sizeof(aa)) << "  "  << sizeof(aa);


}