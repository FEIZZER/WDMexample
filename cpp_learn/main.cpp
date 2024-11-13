
#define CINTERFACE
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

	void printValue()
	{
		printf("value: %d", value);
	}
	
protected:
	const int value = 1;
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
	const int value = 2;
private:
	int num;
};


typedef struct ApiInfo_ {
	std::string	dll_name;				// api所在模块的名字
	std::string proc_name;				// api的导出名
	uintptr_t	origin_ptr_ptr;			// 存放原api函数地址的变量的地址
	uintptr_t	target_ptr;				// api目标函数的地址
	bool			com_api;					// 该api是否是com接口的成员函数
} ApiInfo, *PApiInfo;

typedef struct HookAppSwitch_ {
	char	appId[10];
	bool	bFileAccess;
	bool	bIMChat;
}HookAppSwitch, *PHookAppSwitch;

typedef enum Hook_Status_ {
	Status_None = 0,	
	Status_Injected,				// 注入
	Status_Hooked,					// 注入后收到注册
	Status_UnInjected,				// 反注入
	Status_Max,
}Hook_Status;

typedef struct HookedProcess_ {
	unsigned long	nPid;			// 进程id
	unsigned long	nPort;			// 进程通信port
	uintptr_t		hhook;			// SetWindowsHookExW 注入后返回的句柄， 反注入时使用
	char			appId[10];		// appid
	Hook_Status		status;			// 管控进程的状态
	bool			bFileAccess;	// 文件访问的开关
	bool			bIMChat;		// IM聊天的开关
}HookedProcess,*PHookedProcess;

int main()
{
	char filepath[MAX_PATH];
	auto nRet = GetModuleFileNameA(NULL, filepath, MAX_PATH);
	
	printf("nRet:%d, filepath:%s \n", nRet, filepath);
	GetLastError
	std::cout << std::string(filepath, 0, nRet);
}