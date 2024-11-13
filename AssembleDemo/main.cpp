
#include <iostream>
#include <string>
#include <Windows.h>
#include <TlHelp32.h>

#include "flog.h"


using hook_proc = uintptr_t(*)(ULONG targetProcessId);
using unhook_proc = bool(*)(uintptr_t proc_addr);

using hook_process_proc = unsigned long (*) (unsigned long);
using unhook_process_proc = bool (*) (unsigned long long);

void Block()
{
	char in[1024];
	std::string str_quit = "quit";
	do {
		std::cout << "enter \" quit \" to quit" << std::endl;
		std::cin >> in;
		if (str_quit == in)
		{
			break;
		}
	} while (true);
}

ULONG GetMainThreadId(ULONG processId)
{
	ULONG mainThread = 0;
	UINT64 maxTime = MAXUINT64;
	HANDLE threadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, NULL);
	THREADENTRY32 threadEntry;
	threadEntry.dwSize = sizeof(THREADENTRY32);

	if (!Thread32First(threadSnapshot, &threadEntry))
	{
		return 0;
	}

	do {
		if (threadEntry.th32OwnerProcessID == processId)
		{
			HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION, FALSE, threadEntry.th32ThreadID);
			if (hThread == NULL)
			{
				std::cout << "OpenThread failed:" << GetLastError() << std::endl;
				continue;
			}

			FILETIME createTime = { };
			FILETIME exitTime = { };
			FILETIME kernelTime = { };
			FILETIME userTime = { };
			if (!GetThreadTimes(hThread, &createTime, &exitTime, &kernelTime, &userTime))
			{
				std::cout << "GetThreadTimes failed:" << GetLastError() << std::endl;
				continue;
			}

			if (*(PUINT64)&createTime && *(PUINT64)&createTime < maxTime)
			{
				mainThread = threadEntry.th32ThreadID;
				maxTime = *(PUINT64)&createTime;
			}
		}
	} while (Thread32Next(threadSnapshot, &threadEntry));

	return mainThread;
}


int load_dll_test(int nPid) {
	auto hMod = LoadLibraryA("dlp_hook_dll.dll");
	if (hMod == nullptr)
	{
		std::cout << "load dll failed" << std::endl;
		return 1;
	}

	auto hook = (hook_process_proc)GetProcAddress(hMod, "HookProcess");
	if (hook == nullptr)
	{
		std::cout << "get hook proc failed, " << std::endl;
		return 1;
	}

	auto unhook = (unhook_process_proc)GetProcAddress(hMod, "UnHookProcess");
	if (unhook == nullptr)
	{
		std::cout << "get unhook proc failed, " << std::endl;
		return 1;
	}

	auto hhook = hook(nPid);

	if (hhook == 0)
	{
		std::cout << "hook failed: " << GetLastError() << std::endl;
		return 1;
	}

	Block();

	if (!unhook(hhook))
	{
		std::cout << "unhook failed" << std::endl;
		return 1;
	}

	return 0;
}



int main(int args, char* argv[])
{
	

	if (args != 2)
	{
		std::cout << "wrong params" << std::endl;
		return 1;
	}

	auto nPid = std::stoi(argv[1]);
	// return load_dll_test(nPid);
	auto hMod = LoadLibraryA("detours_hook_dll.dll");
	if (hMod == nullptr)
	{
		std::cout << "LoadLibraryA failed: " << GetLastError() << std::endl;
		return 1;
	}

	hook_proc proc = (hook_proc)GetProcAddress(hMod, "SetHook");
	if (proc == nullptr)
	{
		std::cout << "GetProcAddress failed: " << GetLastError() << std::endl;
		return 1;
	}
	uintptr_t hhook = proc(nPid);

	std::cout << "hhook:" << hhook << std::endl;
	
	Block();
	
	unhook_proc unhook = (unhook_proc)GetProcAddress(hMod, "UnHook");
	if (unhook == nullptr)
	{
		std::cout << "GetProcAddress failed: " << GetLastError() << std::endl;
		return 1;
	}
	if (!unhook(hhook))
	{
		std::cout << "unhook failed:" << GetLastError() << std::endl;
	}

	
	return 0;
}