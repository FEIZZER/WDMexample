//
// Created by DELL on 2023/9/4.
//
#include "pch.h"

#include "PE64.h"
#include "windows.h"
#define IMAGE_FIRST_SECTION64(ntheader) ((PIMAGE_SECTION_HEADER) \
					( (ULONG_PTR)ntheader +                    \
						FIELD_OFFSET(IMAGE_NT_HEADERS64,OptionalHeader) + \
						((PIMAGE_NT_HEADERS64)(ntheader))->FileHeader.SizeOfOptionalHeader))

int ClassifyPE64(char* buffer, unsigned long long size) {

    PIMAGE_NT_HEADERS64 pNtHeader = ((PIMAGE_DOS_HEADER)buffer)->e_lfanew + buffer;

    if (pNtHeader->FileHeader.Characteristics & IMAGE_FILE_DLL) {
        return 1;
    }
    else if (pNtHeader->OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_WINDOWS_GUI ||
             pNtHeader->OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_WINDOWS_CUI) {
        return 2;
    }else {
        return 3;
    }
}