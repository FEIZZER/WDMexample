#include <Windows.h>
#include <stdio.h>
#include <list>
#include <TlHelp32.h>
#include "hook_proc.h"

extern HMODULE gDllModule;
HHOOK gHookProc;

LRESULT MyHookProc(
    _In_ int nCode,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam)
{
    printf("hook!");
    return CallNextHookEx(gHookProc, nCode, wParam, lParam);
}

std::list<ULONG> GetSpecialThreadId(ULONG processId)
{
    std::list<ULONG> res{};
    HANDLE threadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, NULL);
    THREADENTRY32 threadEntry;
    threadEntry.dwSize = sizeof(THREADENTRY32);

    if (!Thread32First(threadSnapshot, &threadEntry))
    {
        return res;
    }

    do {
        if (threadEntry.th32OwnerProcessID == processId)
        {
            res.push_back(threadEntry.th32ThreadID);
        }
    } while (Thread32Next(threadSnapshot, &threadEntry));

    return res;
}

BOOL SetHook(HANDLE targetProcessId)
{
    std::list<ULONG> threadId2Hook = GetSpecialThreadId(HandleToULong(targetProcessId));
    printf("threadid:%d to hook\n", threadId2Hook.front());
    gHookProc = SetWindowsHookExW(WH_CALLWNDPROC, MyHookProc, gDllModule, 0);
    if (gHookProc == NULL) 
    {
        printf("SetWindowsHookExW failed:%x\n", GetLastError());
        return FALSE;
    }
    return TRUE;
}

BOOL UnHook()
{
    if (gHookProc != NULL)
    {
       return UnhookWindowsHookEx(gHookProc);
    }
    return TRUE;
}