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


#include <memory>
#include <set>

class test {
public:
	test(int x)
	{
		num = x;
		std::cout << "new test:" << num << std::endl;
	}

	~test()
	{
		std::cout << "delete test: " << num << std::endl;
	}

	test(test&& other)
	{
		this->num = other.num;
		other.num = 0;
		std::cout << "move test:" << num << std::endl;
	}

	test(const test& other)
	{
		this->num = other.num;
		std::cout << "copy test: " << num << std::endl;
	}

	bool operator<(const test& other) const 
	{
		return this->num < other.num;
	}

	/*bool operator ==(const test& other) const 
	{
		return this->num == other.num;
	}*/
private:
	int num = 0;
};


class A1 {
public:
	virtual void fun1() {
		std::cout << "A1::fun\n";
	}
};
class A {
public:
	virtual void fun() {
		std::cout << "A::fun\n";
	}
};
class B :public A, public A1 {
public:
	B() {
		num = 33;
	}
	void fun() {
		std::cout << "B::fun, num:" << num << std::endl;
	}
private:
	int num;
};

void new_func(B* b)
{
	std::cout << "you have be hooked" << std::endl;
}

int main()
{
	A* pObj[2] = { new A, new B };

	B* b = new B();

	uintptr_t vtb_addr = *(uintptr_t*)b;
	uintptr_t func_addr = *(uintptr_t*)vtb_addr;
	using func_type = void (*) (B* b);

	func_type pfunc = (func_type)func_addr;
	// b->fun();
	pfunc(b);
	std::cout << "vtb_addr, this: " << *(uintptr_t*)b << ", next: " << *((uintptr_t*)b + 1) << std::endl;

	std::cout << "func_addr, this_value:" << *((uintptr_t*)vtb_addr) << ", next_value:" << *((uintptr_t*)vtb_addr + 1) << std::endl;

	DWORD oldAttribute;
	VirtualProtect((void*)vtb_addr, sizeof(uintptr_t), PAGE_READWRITE, &oldAttribute);

	*(uintptr_t*)vtb_addr = (uintptr_t)new_func;
	VirtualProtect((void*)vtb_addr, sizeof(uintptr_t), oldAttribute, &oldAttribute);


	b->fun();
}