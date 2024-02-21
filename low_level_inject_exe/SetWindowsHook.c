#include "windows.h"

#include "SetWindowsHook.h"
ULONG GetProcessThreadId(HANDLE targetProcessId)
{


}


BOOL SetWindowsHook2Inject(HANDLE processId)
{
	HMODULE hDll = LoadLibraryW(L"E:\\DllDemo.dll");
	if (hDll == NULL || hDll == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	FARPROC procAddr = GetProcAddress(hDll, "func1");
	if (procAddr == NULL)
	{
		return FALSE;
	}

	HHOOK hhook = SetWindowsHookExW(WH_CALLWNDPROC, procAddr, hDll, 0);
	if (hhook == NULL)
	{
		return FALSE;
	}

	MessageBoxW(NULL, L"my name is feizzer", L"This is a Title", MB_OK);

	return TRUE;
}
