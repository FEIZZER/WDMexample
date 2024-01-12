#include <iostream>
#include <windows.h>
#include "elf_struct.h"

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

        if (!ReadFile(hFile, buffer, size, (DWORD*)0, NULL)) {
            break;
        }

        bRet = true;

    } while (0);

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

int main() {

    PCHAR pFileBuffer = NULL;
    unsigned long fileSize = NULL;
    CHAR filePath[] = "C:\\Users\\DELL\\Desktop\\main.o";
    LoadFile(filePath, &pFileBuffer, &fileSize);

    if (pFileBuffer == NULL) {
        return 1;
    }



	return 0;
}