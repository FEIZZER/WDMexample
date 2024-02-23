#include <Windows.h>
#include <stdio.h>
#include <list>
#include <fstream>
#include <TlHelp32.h>
#include "hook_proc.h"

extern HMODULE gDllModule;
HHOOK gHookProc;

LRESULT MyHookProc(
    _In_ int nCode,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam)
{

    std::fstream fileStream;
    fileStream.open("C:\\Users\\DELL\\Desktop\\hook_log.txt", std::ios::out | std::ios::app);


    PMSLLHOOKSTRUCT extraInfo = (PMSLLHOOKSTRUCT)lParam;
    switch (wParam)
    {
    case WM_MOUSEMOVE:  // 鼠标移动
        break;

    default:
        fileStream << "pid hooked msg: " << GetCurrentProcessId();
        fileStream << ", nCode: " << nCode << " ,wParam" << wParam;
        fileStream << ", lParam->mouseData: " << extraInfo->mouseData <<
            ", pos:" << extraInfo->pt.x << "," << extraInfo->pt.y << std::endl;
        break;
    }



    fileStream.close();

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

ULONG_PTR SetHook(HANDLE targetProcessId)
{
    std::list<ULONG> threadId2Hook = GetSpecialThreadId(HandleToULong(targetProcessId));

    HHOOK proc_addr = NULL;
    for (auto threadId : threadId2Hook)
    {
        // 只要注入一个线程, 该进程就注入成功了
        proc_addr = SetWindowsHookExW(WH_MOUSE, MyHookProc, gDllModule, threadId);
        if (proc_addr != NULL)
        {
            gHookProc = proc_addr;
            printf("hook processId:%d, threadId:%d, pro_addr:%p \n", HandleToULong(targetProcessId), threadId, proc_addr);
            break;
        }
        printf("SetWindowsHookExW failed:%x\n", GetLastError());
    }
    return (ULONG_PTR)proc_addr;
}

BOOL UnHook(ULONG_PTR proc_addr)
{
    return UnhookWindowsHookEx((HHOOK)proc_addr);
}