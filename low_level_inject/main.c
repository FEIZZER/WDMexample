#include "windows.h"
#include <stdio.h>
#include <stdbool.h>

extern void Start();

void PrintTest() {
    printf("printf test\n");
}

int main(int argc, char* argv[]) 
{

    //Start();

    // 
    if (argc != 2) 
    {
        printf("bad args\n");
        return 1;
    }
    int ProcessId = atoi(argv[1]);
    if (ProcessId == 0)
    {
        printf("bad args\n");
        return 1;
    }

    HANDLE hProcess = NULL;
    const ULONG _DesiredAccess = PROCESS_TERMINATE | PROCESS_SUSPEND_RESUME | PROCESS_QUERY_INFORMATION
        | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE;

    hProcess = OpenProcess(_DesiredAccess, FALSE, ProcessId);
    if (hProcess == NULL || hProcess == INVALID_HANDLE_VALUE)
    {
        printf("open process:%d failed, err:%d\n", ProcessId, GetLastError());
        return 1;
    }



    return 0;
}

void* InjectAllocVirMem(HANDLE hProcess, SIZE_T size, BOOLEAN executable) {

    void* remote_addr = NULL;

    //NtAllocateVirtualMemory(hProcess, &remote_addr, i, &region_size, MEM_COMMIT | MEM_RESERVE, executable ? PAGE_EXECUTE_READWRITE : PAGE_READWRITE);

    remote_addr = VirtualAllocEx(hProcess, NULL, size, MEM_COMMIT | MEM_RESERVE, executable ? PAGE_EXECUTE_READWRITE : PAGE_READWRITE);

    return remote_addr;
}



bool InjectWriteJump(HANDLE hProcess, void* targetPtr, void* injectPtr) {

    ULONG myProtect;
    UCHAR jumpCode[20];

    // >= win10 ø…”√
    jumpCode[0] = 0X48;
    jumpCode[1] = 0xe9;
    *(ULONG*)(jumpCode + 2) = (ULONG)(ULONG_PTR)injectPtr - (ULONG)(ULONG_PTR)targetPtr - 6;

    if (!VirtualProtectEx(hProcess, targetPtr, 6, PAGE_READWRITE, &myProtect)) {
        printf("change failed \n");
        return false;
    }
    size_t outLen;
    if (!WriteProcessMemory(hProcess, targetPtr, jumpCode, 6, &outLen)) {
        printf("write failed \n");
        return false;
    }
    if (!VirtualProtectEx(hProcess, targetPtr, 6, myProtect, &myProtect)) {
        printf("change2 failed \n");
        return false;
    }

    return true;
}