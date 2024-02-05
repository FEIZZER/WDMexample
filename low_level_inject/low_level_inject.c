#include "windows.h"
#include "low_level_inject.h"
#include <stdio.h>
#include <stdbool.h>
#include <wchar.h>
void PrintTest()
{
    MessageBoxW(NULL, L"you have be injected", L"This is a Title", MB_OK);
}

//
#define TEXT_SECTION_NAME   ".text"
#define TEXT_SECTION_IDX    0
#define ZZZZ_SECTION_NAME   "zzzz"
#define ZZZZ_SECTION_IDX    1

typedef struct _Section_Offset {
    unsigned long long entry;

}Section_Offset, *PSection_Offset;

ULONG Start_Offset = 0;

HINSTANCE Dll_Instance = NULL;
PCHAR Inject_Code = NULL;
SIZE_T Inject_Code_Size = 0;


void* Inject_CopyCode(HANDLE, void*, SIZE_T);
void* Inject_AllocVirMem(HANDLE, SIZE_T, BOOLEAN);
bool Inject_WriteJump(HANDLE, void*, void*, bool);
void* Inject_WriteJmpTable(HANDLE hProcess, void* target_ptr, void* inject_ptr);



// is  in 32bit jmp range
bool Inject_InRange32Bit(ULONG_PTR target_ptr, ULONG_PTR inject_ptr)
{
    long long diff = target_ptr - inject_ptr;
    diff = diff < 0 ? diff * -1 : diff;
    return diff < 0x80000000;
}

ULONG_PTR GetMessageBoxWAddr()
{
    HMODULE hUser32dll = LoadLibraryW(L"user32.dll");
    // HMODULE hUser32dll = GetModuleHandleW(L"C:\\Windows\\System32\\user32.dll");
    if (hUser32dll == NULL || hUser32dll == INVALID_HANDLE_VALUE)
    {
        printf("load dll failed:%x\n", GetLastError());
        return NULL;
    }
    ULONG_PTR MessgaeBoxWProc_addr = GetProcAddress(hUser32dll, "MessageBoxW");
    if (MessgaeBoxWProc_addr == NULL)
    {
        printf("can not find MessageBoxW\n");
        return NULL;
    }
    return MessgaeBoxWProc_addr;
}

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
        Start_Offset = (ULONG)(target->entry - pOptHeader64->ImageBase - section[TEXT_SECTION_IDX].VirtualAddress);
        printf("target->entry:%x, imageBase:%x, start_offset:%x\n", 
            target->entry,pOptHeader64->ImageBase, Start_Offset);
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
    ULONG_PTR remote_addr = Inject_CopyCode(hProcess, Inject_Code, Inject_Code_Size);
    if (remote_addr == NULL)
    {
        return 1;
    }

    ULONG_PTR hookProc_addr = GetMessageBoxWAddr();
    printf("target_ptr:%llx, remote_ptr:%llx, start_offset:%x delta:%llx\n", 
        hookProc_addr, remote_addr,  Start_Offset, hookProc_addr-remote_addr);

    if (!Inject_WriteJump(hProcess, hookProc_addr, remote_addr + Start_Offset, Inject_InRange32Bit(hookProc_addr, remote_addr)))
    {
        printf("Inject_WriteJump failed, hookProc_addr:%llux \n", hookProc_addr);
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

bool Inject_WriteJump(HANDLE hProcess, void* target_ptr, void* inject_ptr, bool long_diff) {

    ULONG myProtect;
    UCHAR jumpCode[20];

    // >= win10 ø…”√
    if (long_diff)
    {
        jumpCode[0] = 0X48;
        jumpCode[1] = 0xe9;
        *(ULONG*)(jumpCode + 2) = (ULONG)(ULONG_PTR)inject_ptr - (ULONG)(ULONG_PTR)target_ptr - 6;
    }
    else 
    {
        printf("here\n");
        void* jmp_table = 0;
        if ((ULONG_PTR)inject_ptr & 0xffffffff00000000)
        {
            int inject_len = 6;
            
            jmp_table = Inject_WriteJmpTable(hProcess, target_ptr, inject_ptr);
            if (jmp_table == NULL)
            {
                printf("Inject_WriteJmpTable failed\n");
                return false;
            }
            printf("jmp_table:%p\n", jmp_table);
            *(USHORT*)jumpCode = 0x25FF;
            *(ULONG *)(jumpCode + 2) = (ULONG)((ULONG_PTR)jmp_table - (ULONG_PTR)target_ptr - 6);
            
        }
        else
        {
            printf("unexpected situation\n");
            return false;
        }
    }

    if (!VirtualProtectEx(hProcess, target_ptr, 6, PAGE_READWRITE, &myProtect)) {
        printf("change vm failed:%x\n", GetLastError());
        return false;
    }
    size_t outLen;
    if (!WriteProcessMemory(hProcess, target_ptr, jumpCode, 6, &outLen)) {
        printf("write failed \n");
        return false;
    }
    if (!VirtualProtectEx(hProcess, target_ptr, 6, myProtect, &myProtect)) {
        printf("change2 vm failed:%x\n", GetLastError());
        return false;
    }
    printf("Inject_WriteJump done! wirtedLen:%d\n", outLen);
 
    return true;
}

void* Inject_WriteJmpTable(HANDLE hProcess,void* target_ptr, void* inject_ptr)
{
    HMODULE Ntdll = GetModuleHandleW(L"ntdll.dll");
    HMODULE Kernel32 = GetModuleHandleW(L"kernel32.dll");
    HMODULE User32 = GetModuleHandleW(L"user32.dll");
    void* temp_ptr = ((ULONG_PTR)User32 < (ULONG_PTR)Kernel32 ? (ULONG_PTR)User32 : (ULONG_PTR)Kernel32) - 0x10000;
    void* jmp_table = VirtualAllocEx(hProcess, temp_ptr, 0x100, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    SIZE_T writed_len = 0;
    
    if (jmp_table && Inject_InRange32Bit(jmp_table, target_ptr))
    {
        WriteProcessMemory(hProcess, jmp_table, &inject_ptr, 8, &writed_len);
        if (writed_len == 8)
        {
            return jmp_table;
        }
    }
    jmp_table = NULL;
    temp_ptr = (ULONG_PTR)target_ptr - 8;
    short prefix_buf[1024];
    SIZE_T read_len = 0;
    ReadProcessMemory(hProcess, temp_ptr, prefix_buf, 8, &read_len);
    if (*(ULONG_PTR*)prefix_buf == 0x9090909090909090 || 
        *(ULONG_PTR*)prefix_buf == 0xcccccccccccccccc)
    {
        printf("write jmp done!\n");
        jmp_table = temp_ptr;
    }
    else
    {
        ReadProcessMemory(hProcess, (void*)((ULONG_PTR)temp_ptr + 0x100000), prefix_buf, sizeof(prefix_buf), &read_len);
        if (read_len != sizeof(prefix_buf))
        {
            printf("ReadProcessMemory failed\n");
            return NULL;
        }
        printf("before jmp_table:%p\n", jmp_table);
        for (int i = 0; i < sizeof(prefix_buf) && !jmp_table; i++) {
            if (*((ULONG_PTR*)&prefix_buf[i]) == 0x9090909090909090 ||
                *((ULONG_PTR*)&prefix_buf[i]) == 0xcccccccccccccccc) {
                jmp_table = (void*)((ULONG_PTR)temp_ptr + i);
                printf("jmp_table:%p\n", jmp_table);
                /*
                sprintf(buffer,"HACK: table found at %p, index %x\n",myTable, i);
                OutputDebugStringA(buffer);
                */
            }
        }

        if (jmp_table == NULL)
        {
            printf("no patch area\n");
            return NULL;
        }
    }

    ULONG myProtect;
    if (VirtualProtectEx(hProcess, jmp_table, sizeof(void*), PAGE_READWRITE, &myProtect)) {
        SIZE_T len2 = 0;
        BOOL myVM = WriteProcessMemory(hProcess, jmp_table, &inject_ptr, 8, &len2);
        if (myVM && 8 == len2) {
            myVM = VirtualProtectEx(hProcess, jmp_table, 8, myProtect, &myProtect);
            if (myVM) {
                return jmp_table;
            }
        }
    }
    return NULL;
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