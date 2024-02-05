//
// Created by DELL on 2023/9/4.
//
#include "windows.h"


typedef short PE_CHARACTER;
#define INVALID     0x1000
#define PE64        0x0001
#define EXE         0x0002
#define DLL         0x0004
#define SYS         0x0008

BOOL LoadFile(char* filePath, char** pBuf, unsigned long* pSize);

PE_CHARACTER IsPE(char*, unsigned long long);

int ClassifyPE32(char*, unsigned long long);
int ClassifyPE64(char*, unsigned long long);

BOOL ParseImportTable32(char* base, unsigned long long size);
BOOL ParseImportTable64(char* base, unsigned long long size);

BOOL ParseExportTable32(char* base, unsigned long long size);
BOOL ParseExportTable64(char* base, unsigned long long size);

void* FindDllExport32(char* base, const char* procName);

PE_CHARACTER IsPE(char* buffer, unsigned long long size) {

    PE_CHARACTER res = 0;

    if (size <= sizeof(IMAGE_DOS_HEADER) + sizeof(IMAGE_NT_HEADERS32))
    {
        return INVALID;
    }

    if (*(WORD*)buffer != IMAGE_DOS_SIGNATURE)
    {
        return INVALID;
    }

    unsigned long offset = ((PIMAGE_DOS_HEADER)buffer)->e_lfanew;
    if (size <= offset + sizeof(IMAGE_NT_HEADERS32))
    {
        return INVALID;
    }

    PIMAGE_NT_HEADERS pNtHeader = (IMAGE_NT_HEADERS*)((UCHAR*)buffer + offset);
    if (pNtHeader->Signature != IMAGE_NT_SIGNATURE)
    {
        return INVALID;
    }
    if (pNtHeader->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
    {
        offset = sizeof(IMAGE_NT_HEADERS32);
    }
    else if (pNtHeader->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
    {
        offset = sizeof(IMAGE_NT_HEADERS64);
        res |= PE64;
    }
    else if (pNtHeader->OptionalHeader.Magic == IMAGE_ROM_OPTIONAL_HDR_MAGIC)
    {
        return INVALID;
    }

    offset = offset + pNtHeader->FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER);
    if (size < offset)
    {
        return INVALID;
    }
    return res;
}