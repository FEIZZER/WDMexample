//
// Created by DELL on 2023/9/4.
//
#include "windows.h"
#include "stdbool.h"

#define IMAGE_FIRST_SECTION32(ntheader) ((PIMAGE_SECTION_HEADER) \
					    ((ULONG_PTR)ntheader +                    \
						FIELD_OFFSET(IMAGE_NT_HEADERS32,OptionalHeader) + \
						((PIMAGE_NT_HEADERS32)(ntheader))->FileHeader.SizeOfOptionalHeader))

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

Rva2Va32(PIMAGE_NT_HEADERS32 pNtHeader, DWORD Rva, char* base, PIMAGE_SECTION_HEADER* pSectionHeader) {

    if (NULL == pNtHeader || NULL == base) {
        return NULL;
    }

    if (NULL == *pSectionHeader) {
        *pSectionHeader = Rva2Section32(pNtHeader, Rva);
        if (NULL == *pSectionHeader) {
            return NULL;
        }
    }

    return (PVOID)((ULONG_PTR)base + (Rva - (*pSectionHeader)->VirtualAddress) +
                   (*pSectionHeader)->PointerToRawData);
}

bool ParseImportTable32(char* base, unsigned long long size){
    if (base == NULL) {
        return false;
    }
    PIMAGE_NT_HEADERS32 pNtHeader = ((PIMAGE_DOS_HEADER)base)->e_lfanew + base;

    DWORD iidRVA = pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
    PIMAGE_SECTION_HEADER pSection = NULL;
    IMAGE_IMPORT_DESCRIPTOR  importTableDesNULL = { 0 };
    PIMAGE_IMPORT_DESCRIPTOR pImportTableDes = (PIMAGE_IMPORT_DESCRIPTOR)Rva2Va32(pNtHeader, iidRVA, base, &pSection);
    if (pImportTableDes == NULL) {
        return false;
    }
    pSection = NULL;


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
    else
    {
        return 3;
    }

    return 0;
}
