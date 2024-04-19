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


void Test(size_t v)
{
	std::cout << v << std::endl;
}
#define V 2048
int main()
{
	Test((size_t)V * 1024 * 1024);
}