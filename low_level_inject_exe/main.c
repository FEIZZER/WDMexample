#include <stdio.h>

#include "windows.h"
#include "../low_level_inject/low_level_inject.h"

int main(int argc, char* argv[]) 
{

	if (argc != 2)
	{
		printf("bad arg, need pid");
		return 1;
	}

	int pid = atoi(argv[1]);
	HANDLE hProcess = NULL;
	const ULONG _DesiredAccess = PROCESS_TERMINATE | PROCESS_SUSPEND_RESUME | PROCESS_QUERY_INFORMATION
		| PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE;

	hProcess = OpenProcess(_DesiredAccess, FALSE, pid);
	if (hProcess == NULL || hProcess == INVALID_HANDLE_VALUE)
	{
		printf("open process:%d failed, err:%d\n", pid, GetLastError());
		return 1;
	}

	printf("init start!\n");

	if (Inject_Init_Template() != LOWLEVEL_SUCCESS)
	{
		printf("init failed\n");
		return 1;
	}


	if (Inject(hProcess) != LOWLEVEL_SUCCESS)
	{
		printf("inject failed\n");
		return 1;
	}

	

	return 0;
}