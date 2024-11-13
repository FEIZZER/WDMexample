
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
	std::string	dll_name;				// api����ģ�������
	std::string proc_name;				// api�ĵ�����
	uintptr_t	origin_ptr_ptr;			// ���ԭapi������ַ�ı����ĵ�ַ
	uintptr_t	target_ptr;				// apiĿ�꺯���ĵ�ַ
	bool			com_api;					// ��api�Ƿ���com�ӿڵĳ�Ա����
} ApiInfo, *PApiInfo;

typedef struct HookAppSwitch_ {
	char	appId[10];
	bool	bFileAccess;
	bool	bIMChat;
}HookAppSwitch, *PHookAppSwitch;

typedef enum Hook_Status_ {
	Status_None = 0,	
	Status_Injected,				// ע��
	Status_Hooked,					// ע����յ�ע��
	Status_UnInjected,				// ��ע��
	Status_Max,
}Hook_Status;

typedef struct HookedProcess_ {
	unsigned long	nPid;			// ����id
	unsigned long	nPort;			// ����ͨ��port
	uintptr_t		hhook;			// SetWindowsHookExW ע��󷵻صľ���� ��ע��ʱʹ��
	char			appId[10];		// appid
	Hook_Status		status;			// �ܿؽ��̵�״̬
	bool			bFileAccess;	// �ļ����ʵĿ���
	bool			bIMChat;		// IM����Ŀ���
}HookedProcess,*PHookedProcess;

int main()
{
	char filepath[MAX_PATH];
	auto nRet = GetModuleFileNameA(NULL, filepath, MAX_PATH);
	
	printf("nRet:%d, filepath:%s \n", nRet, filepath);
	GetLastError
	std::cout << std::string(filepath, 0, nRet);
}