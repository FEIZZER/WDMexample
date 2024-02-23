#include <stdio.h>
#include <map>
#include "windows.h"

#include "../detours_hook_dll/hook_proc.h"
#include "../low_level_inject/low_level_inject.h"


std::map<ULONG, ULONG_PTR> g_injected_process;
VOID HookProcess(ULONG pid)
{
	ULONG_PTR proc_addr = SetHook(ULongToHandle(pid));
	if (proc_addr == NULL)
	{
		printf("pid:%d hook failed\n", pid);
		return;
	}
	g_injected_process.insert({ pid , proc_addr });
}

VOID UnhookProcess(ULONG pid)
{
	auto iter = g_injected_process.find(pid);
	if (iter == g_injected_process.end())
	{
		return;
	}
	ULONG_PTR pro_addr = iter->second;
	if (!UnHook(pro_addr))
	{
		printf("pid:%d unhook failed\n", pid);
	}
}

int main(int argc, char* argv[])
{
	ULONG pid;
	if (argc >= 2)
	{
		pid = atoi(argv[1]);
		HookProcess(pid);
	}

	char input[1024];
	do {
		scanf_s("%s", input, 1024);
		if (input[0] == 'q')
		{
			break;
		}
		else if (input[0] == 'u')
		{
			pid = atoi(input + 1);
			UnhookProcess(pid);
		}
		else
		{
			pid = atoi(input);
			if (pid != 0)
				HookProcess(pid);
		}

	} while (true);


	for (auto item : g_injected_process)
	{
		UnhookProcess(item.first);
	}

}


//int low_level_inject(int argc, char* argv[]) 
//{
//
//	if (argc != 2)
//	{
//		printf("bad arg, need pid");
//		return 1;
//	}
//
//	int pid = atoi(argv[1]);
//	HANDLE hProcess = NULL;
//	const ULONG _DesiredAccess = PROCESS_TERMINATE | PROCESS_SUSPEND_RESUME | PROCESS_QUERY_INFORMATION
//		| PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE;
//
//	hProcess = OpenProcess(_DesiredAccess, FALSE, pid);
//	if (hProcess == NULL || hProcess == INVALID_HANDLE_VALUE)
//	{
//		printf("open process:%d failed, err:%d\n", pid, GetLastError());
//		return 1;
//	}
//
//	printf("init start!\n");
//
//	if (Inject_Init_Template() != LOWLEVEL_SUCCESS)
//	{
//		printf("init failed\n");
//		return 1;
//	}
//
//
//	if (Inject(hProcess) != LOWLEVEL_SUCCESS)
//	{
//		printf("inject failed\n");
//		return 1;
//	}
//
//	
//
//	return 0;
//}