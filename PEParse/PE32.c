//
// Created by DELL on 2023/9/4.
//

#include "PE32.h"

#include "pch.h"
#include "windows.h"

#define NTOSKRNL        "ntoskrnl.exe"

#define IMAGE_FIRST_SECTION32(ntheader) ((PIMAGE_SECTION_HEADER) \
					    ((ULONG_PTR)ntheader +                    \
						FIELD_OFFSET(IMAGE_NT_HEADERS32,OptionalHeader) + \
						((PIMAGE_NT_HEADERS32)(ntheader))->FileHeader.SizeOfOptionalHeader))

extern char* oleaut32_ordinal2funcname[444]; 
extern char* ws2_32_ordinal2funcname[501];

PIMAGE_SECTION_HEADER Rva2Section32(PIMAGE_NT_HEADERS32 pNtHeader, DWORD Rva) {

    if (NULL == pNtHeader) {
        return NULL;
    }

    // 找到 Rva 属于哪一个section
    PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION32(pNtHeader);

    for (INT i = 0; i < pNtHeader->FileHeader.NumberOfSections; i++) {
        if (Rva >= pSectionHeader->VirtualAddress &&
            Rva < pSectionHeader->VirtualAddress + pSectionHeader->Misc.VirtualSize) {
            return pSectionHeader;
        }
        pSectionHeader++;
    }

    return NULL;
}

ULONG_PTR RVA2FA32(char* base, DWORD Rva, PIMAGE_SECTION_HEADER* ppSectionHeader) {

    PIMAGE_NT_HEADERS32 pNtHeader = PNTHEADER(base);
    PIMAGE_SECTION_HEADER pSectionHeader = NULL;

    if (ppSectionHeader != NULL && *ppSectionHeader != NULL)
    {
        pSectionHeader = *ppSectionHeader;
    }
    else
    {
        pSectionHeader = Rva2Section32(pNtHeader, Rva);
    }
    if (pSectionHeader == NULL)
    {
        return NULL;
    }
    if (ppSectionHeader != NULL)
    {
        *ppSectionHeader = pSectionHeader;
    }

    return (ULONG_PTR)base + Rva - pSectionHeader->VirtualAddress +pSectionHeader->PointerToRawData;
}

bool ParseExportTable32(char* base, unsigned long long size)
{
    if (base == NULL)
    {
        return false;
    }
    PIMAGE_NT_HEADERS32 pNtHeader = ((PIMAGE_DOS_HEADER)base)->e_lfanew + base;

    DWORD iedRVA = pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    PIMAGE_SECTION_HEADER pSection = NULL;
    PIMAGE_EXPORT_DIRECTORY pExportTableDes = RVA2FA32(base, iedRVA, &pSection);
    if (pExportTableDes == NULL)
    {
        return false;
    }

    DWORD* funcAdddrs = (DWORD*)RVA2FA32(base, pExportTableDes->AddressOfFunctions, NULL);
    DWORD* funcNames = (DWORD*)RVA2FA32(base, pExportTableDes->AddressOfNames, NULL);
    WORD* funcNameOrds = (WORD*)RVA2FA32(base, pExportTableDes->AddressOfNameOrdinals, NULL);
    
    char* dllName = RVA2FA32(base, pExportTableDes->Name, NULL);
    DWORD ordBase = pExportTableDes->Base;
   
    ULONG_PTR* funcNameFOA = malloc(sizeof(ULONG_PTR) * pExportTableDes->NumberOfFunctions);
    if (funcNameFOA == NULL)
    {
        return false;
    }
    ZeroMemory(funcNameFOA, sizeof(ULONG_PTR) * pExportTableDes->NumberOfFunctions);

    for (DWORD i = 0; i < pExportTableDes->NumberOfNames; i++)
    {
        if (funcNameOrds[i] >= pExportTableDes->NumberOfFunctions)
        {
            return false;
        }
        funcNameFOA[funcNameOrds[i]] = RVA2FA32(base, funcNames[i], NULL);
    }

    for (DWORD i = 0; i < pExportTableDes->NumberOfFunctions; i++)
    {
        if (funcAdddrs[i] == 0)
        {
            continue;
        }
        /**
        * dllName:  dllName
        * ordinal:  ordBase + i    
        * func addr:funcAddrs[i]
        * funcName: funcNameFOA[i]
        */
    }


    return true;
}

bool ParseImportTable32(char* base, unsigned long long size){
    if (base == NULL) {
        return false;
    }

    PIMAGE_NT_HEADERS32 pNtHeader = ((PIMAGE_DOS_HEADER)base)->e_lfanew + base;
    DWORD iidRVA = pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
    PIMAGE_SECTION_HEADER pSection = NULL;
    IMAGE_IMPORT_DESCRIPTOR  importTableDesNULL = { 0 };
    PIMAGE_IMPORT_DESCRIPTOR pImportTableDes = (PIMAGE_IMPORT_DESCRIPTOR)RVA2FA32(base, iidRVA, &pSection);
    if (pImportTableDes == NULL)
    {
        return false;
    }

    while (true) 
    {
        if (pImportTableDes->Characteristics == 0)
        {
            break;
        }

        char* dllName = RVA2FA32(base, pImportTableDes->Name, NULL);
        printf("dllName:%s\n", dllName);

        PIMAGE_THUNK_DATA32 pINT = RVA2FA32(base, pImportTableDes->OriginalFirstThunk, NULL);
        while (pINT->u1.Function != 0) {

            if (pINT->u1.AddressOfData >> (sizeof(pINT->u1.AddressOfData) * 8 - 1))
            {
                printf("func ord:%d\n", pINT->u1.Ordinal << 1 >> 1);
            }
            else // 通过函数名导出的情况
            {
                PIMAGE_IMPORT_BY_NAME pIIBN = RVA2FA32(base, pINT->u1.AddressOfData, NULL);
                printf("func ord:%d, name:%s\n", pIIBN->Hint, pIIBN->Name);
            }
            pINT++;
        }

        pImportTableDes++;
    }
}

bool IsDriver32(char* base, unsigned long long size)
{
    if (base == NULL) {
        return false;
    }

    PIMAGE_NT_HEADERS32 pNtHeader = ((PIMAGE_DOS_HEADER)base)->e_lfanew + base;
    DWORD iidRVA = pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
    IMAGE_IMPORT_DESCRIPTOR  importTableDesNULL = { 0 };
    PIMAGE_IMPORT_DESCRIPTOR pImportTableDes = (PIMAGE_IMPORT_DESCRIPTOR)RVA2FA32(base, iidRVA, NULL);
    if (pImportTableDes == NULL)
    {
        return false;
    }

    while (true)
    {
        if (pImportTableDes->Characteristics == 0)
        {
            break;
        }

        char* dllName = RVA2FA32(base, pImportTableDes->Name, NULL); 
        if (0 == _strnicmp(dllName, NTOSKRNL, sizeof(NTOSKRNL)))
        {
            return true;
        }
        pImportTableDes++;
    }
    return false;
}

int ClassifyPE32(char* buffer, unsigned long long size) {

    PIMAGE_NT_HEADERS32 pNtHeader = ((PIMAGE_DOS_HEADER)buffer)->e_lfanew + buffer;

    if (pNtHeader->FileHeader.Characteristics & IMAGE_FILE_DLL) {
        return 1;
    }
    else if (pNtHeader->OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_WINDOWS_GUI ||
             pNtHeader->OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_WINDOWS_CUI)
    {
        return 2;
    }
    else if (IsDriver32(buffer, size))
    {
        return 3;
    }

    return 0;
}

bool GenImpHash32(char* base, unsigned long long size, char impHash[])
{

    return true;
}

// base is image memory
// can not call other function
void* FindDllExportProcAddr32(char* base, const char* procName)
{
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)base;
    PIMAGE_NT_HEADERS32 pNtHeader = (PIMAGE_NT_HEADERS32)(base + pDosHeader->e_lfanew);
    PIMAGE_OPTIONAL_HEADER32 pOptHeader = &pNtHeader->OptionalHeader;
    
    IMAGE_DATA_DIRECTORY exportDir = pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
    if (exportDir.Size == 0 || exportDir.VirtualAddress == 0)
    {
        return NULL;
    }

    PIMAGE_EXPORT_DIRECTORY pIED = (PIMAGE_EXPORT_DIRECTORY)(base + exportDir.VirtualAddress);
    ULONG procNameLen = 0;
    for (procNameLen; procName[procNameLen]; procNameLen++);
    ULONG* name_offsets = (ULONG*)(base + pIED->AddressOfNames);
    USHORT* name2ordinal = (USHORT*)(base + pIED->AddressOfNameOrdinals);
    ULONG* function_offsets = (ULONG*)(base + pIED->AddressOfFunctions);
    void* procAddr = NULL;
    ULONG i, j = 0;
    for (ULONG i = 0; i < pIED->NumberOfNames; i++)
    {
        char* name = base + name_offsets[i];
        for ( j = 0; j < procNameLen; j++)
        {
            if (name[j] != procName[j]) break;
        }

        if (j != procNameLen) continue;
        if (name2ordinal[i] >= pIED->NumberOfFunctions) continue;

        procAddr = base + function_offsets[name2ordinal[i]];
        break;
    }

    return procAddr;

}
