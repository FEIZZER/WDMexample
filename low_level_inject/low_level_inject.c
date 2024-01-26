#include "windows.h"
#include "low_level_inject.h"
#include <stdio.h>
#include <stdbool.h>
#include <wchar.h>
void PrintTest()
{
    printf("Print Test\n");
}

//
#define TEXT_SECTION_NAME   ".text"
#define TEXT_SECTION_IDX    0
#define ZZZZ_SECTION_NAME   "zzzz"
#define ZZZZ_SECTION_IDX    1

typedef struct _Section_Offset {
    unsigned long long entry;

}Section_Offset, *PSection_Offset;



extern Dll_Instance;


LowLevelError Inject_Init_Template() 
{
    LowLevelError err = Init_Load_Dll_Err;

    WCHAR dllFilePath[Max_File_Len];
    if (GetModuleFileNameW(Dll_Instance, dllFilePath, Max_File_Len) == 0)
    {
        return err;
    }

    wprintf(L"init done, filePath%s\n", dllFilePath);

}

// load dll resources
LowLevelError Inject_Init()
{
    LowLevelError err = Init_Load_Dll_Err;
    
    HRSRC hrsrc = FindResourceW(NULL, L"LOWLEVEL64", RT_RCDATA);
    if (hrsrc == NULL)
    {
        return err;
    }
    ULONG binSize = SizeofResource(Dll_Instance, hrsrc);
    if (!binSize)
    {
        return err;
    }
    HGLOBAL hglob = LoadResource(Dll_Instance, hrsrc);
    if (!hglob)
    {
        return err;
    }
    PCHAR binData = LockResource(hglob);
    if (binData == NULL)
    {
        return err;
    }

    err = Init_Parse_Dll_Err;

    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)binData;
    if (pDosHeader->e_magic != 'MZ' && pDosHeader->e_magic != 'ZM')
    {
        return err;
    }
    // todo: need access memory verify
    PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)(binData + pDosHeader->e_lfanew);
    if (pNtHeader->Signature != IMAGE_NT_SIGNATURE)
    {
        return err;
    }
    // todo: need to distinguish pe32 and pe64
    if (pNtHeader->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC)
    {
        return err;
    }
    PIMAGE_NT_HEADERS64 pNtHeader64 = (PIMAGE_NT_HEADERS64)pNtHeader;
    PIMAGE_OPTIONAL_HEADER64 pOptHeader64 = &pNtHeader64->OptionalHeader;
    PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(pNtHeader);
    if (pNtHeader->FileHeader.NumberOfSections < 2) 
    {
        return err;
    }
    if (strncmp(section[TEXT_SECTION_IDX].Name, TEXT_SECTION_NAME, sizeof(TEXT_SECTION_NAME)) ||
        strncmp(section[ZZZZ_SECTION_IDX].Name, ZZZZ_SECTION_NAME, sizeof(ZZZZ_SECTION_NAME)) )
    {
        return err;
    }

    PSection_Offset target = (PSection_Offset)binData[section[ZZZZ_SECTION_IDX].PointerToRawData];
    printf("entry: %llx\n", target->entry);
    return LOWLEVEL_SUCCESS;
}

LowLevelError Inject(HANDLE hProcess)
{
    if (hProcess == NULL || hProcess == INVALID_HANDLE_VALUE)
    {
        return 1;
    }

    return LOWLEVEL_SUCCESS;
}

int main2(int argc, char* argv[]) 
{

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