#include <Windows.h>
#include <TlHelp32.h>

#include <iostream>
#include <list>
#include <fstream>

#include "flog.h"
#include "hook_proc.h"

extern HMODULE gDllModule;
HHOOK gHookProc;

LRESULT WINAPI MyHookProc(
    _In_ int nCode,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam)
{
    return CallNextHookEx(NULL, nCode, wParam, lParam);
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

uintptr_t SetHook(ULONG targetProcessId)
{
	ULONG threadId = GetMainThreadId((ULONG)targetProcessId);
	if (threadId == 0)
	{
		std::cout << "GetMainThreadId failed" << std::endl;
		return 0;
	}
    auto proc_addr = SetWindowsHookExW(WH_MOUSE, MyHookProc, gDllModule, threadId);
	if (proc_addr == nullptr)
	{
		std::cout << "SetWindowsHookExW failed: " << GetLastError() << std::endl;
	}

	if (!SetWindowsHookExW(WH_KEYBOARD, MyHookProc, gDllModule, threadId))
	{
		std::cout << "hook WH_KEYBOARD failed: " << GetLastError() << std::endl;
	}
	if (!SetWindowsHookExW(WH_GETMESSAGE, MyHookProc, gDllModule, threadId))
	{
		std::cout << "hook WH_GETMESSAGE failed: " << GetLastError() << std::endl;
	}
	if (!SetWindowsHookExW(WH_CALLWNDPROC, MyHookProc, gDllModule, threadId))
	{
		std::cout << "hook WH_CALLWNDPROC failed: " << GetLastError() << std::endl;
	}
	if (!SetWindowsHookExW(WH_CALLWNDPROCRET, MyHookProc, gDllModule, threadId))
	{
		std::cout << "hook WH_CALLWNDPROCRET failed: " << GetLastError() << std::endl;
	}

	return (uintptr_t)proc_addr;
}

#include <iostream>
bool UnHook(uintptr_t proc_addr)
{
	BOOL bRet = UnhookWindowsHookEx((HHOOK)proc_addr);
	std::cout << "unhook err:" << GetLastError() << ", bRet:" << bRet << std::endl;
	return bRet;
}