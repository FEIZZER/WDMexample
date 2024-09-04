#include <Windows.h>
#include <TlHelp32.h>

#include <iostream>
#include <list>
#include <fstream>

#include "hook_proc.h"

extern HMODULE gDllModule;
HHOOK gHookProc;

LRESULT WINAPI MyHookProc(
    _In_ int nCode,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam)
{
    return CallNextHookEx(gHookProc, nCode, wParam, lParam);
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
	return (uintptr_t)proc_addr;
}

bool UnHook(uintptr_t proc_addr)
{
	return UnhookWindowsHookEx((HHOOK)proc_addr);
}