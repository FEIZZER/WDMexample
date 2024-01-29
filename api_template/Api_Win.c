#include "windows.h"

BOOL LoadFileW(WCHAR* filePath, PCHAR* pBuf, unsigned long* pSize)
{


}

BOOL LoadFile(char* filePath, char** pBuf, unsigned long* pSize) {

    HANDLE hFile = NULL;
    BOOL bRet = FALSE;
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

        bRet = TRUE;

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