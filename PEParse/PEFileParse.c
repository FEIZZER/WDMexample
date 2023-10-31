#include "PEFileParse.h"
#include "pch.h"
#include "PE32.h"
#include "PE64.h"
#include "windows.h"
#include <stdio.h>



bool LoadFile(char* filePath, char** pBuf, unsigned long* pSize) {

    HANDLE hFile = NULL;
    bool bRet = false;
    void* buffer = NULL;
    DWORD size = 0;

    do {
     
        hFile = CreateFileA(filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == NULL || hFile == INVALID_HANDLE_VALUE) {
            break;
        }
        
        size = GetFileSize(hFile, NULL);
        buffer = malloc(size);
        if (buffer == NULL) {
            break;
        }

        if(!ReadFile(hFile, buffer, size, (DWORD*)0, NULL)) {
            break;
        }

        bRet = true;

    }while(0);

    if (bRet && buffer != NULL) {
        *pBuf = buffer;
        *pSize = size;
    }
    if (!bRet && buffer != NULL) {
        free(buffer);
    }
    if (hFile != NULL) {
        CloseHandle(hFile);
    }
    return bRet;
}

bool IsPE(char* buffer, unsigned long long size){

    if (size <= sizeof(IMAGE_DOS_HEADER) + sizeof(IMAGE_NT_HEADERS32))
    {
        return false;
    }

    if (*(WORD*)buffer != IMAGE_DOS_SIGNATURE)
    {
        printf("2");
        return false;
    }

    unsigned long ntHeaderOffset = ((PIMAGE_DOS_HEADER)buffer)->e_lfanew;
    if (size <= ntHeaderOffset + sizeof(IMAGE_NT_HEADERS32)) 
    {
        printf("3");
        return false;
    }

    PIMAGE_NT_HEADERS pNtHeader = PNTHEADER(buffer);
    unsigned long peHeaderLen = 0;

    if (pNtHeader->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
    {
        peHeaderLen = sizeof(IMAGE_NT_HEADERS32);
    }
    else if (pNtHeader->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
    {
        peHeaderLen = sizeof(IMAGE_NT_HEADERS64);
    }
    else if (pNtHeader->OptionalHeader.Magic == IMAGE_ROM_OPTIONAL_HDR_MAGIC)
    {
        printf("4");
        return false;
    }

    peHeaderLen = peHeaderLen + pNtHeader->FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER);
    if (size <= peHeaderLen)
    {
        printf("5");
        return false;
    }
    return true;
}

int ClassifyPE(char* buffer, unsigned long long size) {

    if (!IsPE(buffer, size)) {
        return 0;
    }

    unsigned short e_magic = *(unsigned short*)(*(unsigned long*)(buffer + 60) + buffer + 24);
    if (e_magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
        return ClassifyPE32(buffer, size);
    }
    else if (e_magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC){
        return ClassifyPE64(buffer, size);
    }
    return 0;
}

bool ParseExportTable(char* base, unsigned long long size)
{
    if (!IsPE(base, size)) {
        return 0;
    }

    unsigned short e_magic = PNTHEADER(base)->OptionalHeader.Magic;
    if (e_magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
        return ParseExportTable32(base, size);
    }
    else if (e_magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
        return 0;
    }
}

bool ParseImportTable(char* base, unsigned long long size)
{
    if (!IsPE(base, size)) {
        return 0;
    }

    unsigned short e_magic = PNTHEADER(base)->OptionalHeader.Magic;
    if (e_magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
        return ParseImportTable32(base, size);
    }
    else if (e_magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
        return 0;
    }
    return true;
}


