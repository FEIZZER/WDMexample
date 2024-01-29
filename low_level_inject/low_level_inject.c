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



HINSTANCE Dll_Instance = NULL;
PCHAR Inject_Code = NULL;
SIZE_T Inject_Code_Size = 0;

LowLevelError Inject_Init_Template() 
{
    LowLevelError err;
    PCHAR binData = NULL;
    do {
        err = Init_Load_Dll_Err;

        WCHAR dllFilePath[Max_File_Len];
        DWORD filePathLen = GetModuleFileNameW(Dll_Instance, dllFilePath, Max_File_Len);
        if (filePathLen == 0)
        {
            printf("get filePath failed:%x\n", GetLastError());
            break;
        }
        dllFilePath[filePathLen] = L'\0';

        HANDLE hFile = CreateFileW(dllFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
        {
            break;
        }
        Inject_Code_Size = GetFileSize(hFile, NULL);
        if (Inject_Code_Size <= 0)
        {
            printf("GetFileSize failed:%x\n", GetLastError());
            break;
        }
        binData = malloc(Inject_Code_Size);
        if (binData == NULL)
        {
            printf("malloc buf failed:%x\n", GetLastError());
            break;
        }
        DWORD readSize = 0;
        if (!ReadFile(hFile, binData, Inject_Code_Size, &readSize, NULL))
        {
            printf("ReadFile failed:%x\n", GetLastError());
            break;
        }

        err = Init_Parse_Dll_Err;

        PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)binData;
        if (pDosHeader->e_magic != 'MZ' && pDosHeader->e_magic != 'ZM')
        {
            printf("bad magic num\n");
            break;
        }
        // todo: need access memory verify
        PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)(binData + pDosHeader->e_lfanew);
        if (pNtHeader->Signature != IMAGE_NT_SIGNATURE)
        {
            printf("bad signature\n");
            break;
        }
        // todo: need to distinguish pe32 and pe64
        if (pNtHeader->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC)
        {
            printf("bad opt magic\n");
            break;
        }
        PIMAGE_NT_HEADERS64 pNtHeader64 = (PIMAGE_NT_HEADERS64)pNtHeader;
        PIMAGE_OPTIONAL_HEADER64 pOptHeader64 = &pNtHeader64->OptionalHeader;
        PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(pNtHeader);
        if (pNtHeader->FileHeader.NumberOfSections < 2)
        {
            printf("bad Section num\n");
            break;
        }
        if (strncmp(section[TEXT_SECTION_IDX].Name, TEXT_SECTION_NAME, sizeof(TEXT_SECTION_NAME)) ||
            strncmp(section[ZZZZ_SECTION_IDX].Name, ZZZZ_SECTION_NAME, sizeof(ZZZZ_SECTION_NAME)))
        {
            printf("bad section name\n");
            break;
        }
        PSection_Offset target = (PSection_Offset)&binData[section[ZZZZ_SECTION_IDX].PointerToRawData];
        Inject_Code_Size = section[TEXT_SECTION_IDX].SizeOfRawData;
        Inject_Code = binData + section[TEXT_SECTION_IDX].PointerToRawData;
        err = LOWLEVEL_SUCCESS;
    } while (0);

    if (err != LOWLEVEL_SUCCESS && binData != NULL)
    {
        free(binData);
        binData = NULL;
    }
    return err;
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
    if (Inject_CopyCode(hProcess, Inject_Code, Inject_Code_Size) == NULL)
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

void* Inject_CopyCode(HANDLE hProcess,void* codePtr, SIZE_T codeSize)
{
    void* remoteAddr = Inject_AllocVirMem(hProcess, codeSize, TRUE);
    if (remoteAddr == NULL) 
    {
        return NULL;
    }
    SIZE_T writedSize = 0;
    if (!WriteProcessMemory(hProcess, remoteAddr, codePtr, codeSize, &writedSize))
    {
        return NULL;
    }
    if (codeSize != writedSize)
    {
        return NULL;
    }

    return remoteAddr;
}

void* Inject_AllocVirMem(HANDLE hProcess, SIZE_T size, BOOLEAN executable) {

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


BOOL DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        Dll_Instance = hInstance;
        break;
    case DLL_PROCESS_DETACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    default:
        break;
    }
    return TRUE;
}