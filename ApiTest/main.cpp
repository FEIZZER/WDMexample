#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
VOID GetMainThreadId(ULONG processId)
{
    HANDLE threadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, NULL);
    THREADENTRY32 threadEntry;
    threadEntry.dwSize = sizeof(THREADENTRY32);

    if (!Thread32First(threadSnapshot, &threadEntry))
    {
        return;
    }

    do {
        if (threadEntry.th32OwnerProcessID == processId)
        {
            HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION, FALSE, threadEntry.th32ThreadID);
            if (hThread == NULL)
            {
                std::cout << "OpenThread failed:" << GetLastError() << std::endl;
                continue;
            }

            FILETIME createTime = { };
            FILETIME exitTime = { };
            FILETIME kernelTime = { };
            FILETIME userTime = { };
            if (!GetThreadTimes(hThread, &createTime, &exitTime, &kernelTime, &userTime))
            {
                std::cout << "GetThreadTimes failed:" << GetLastError() << std::endl;
                continue;
            }

            std::cout << "createTime:" << createTime.dwHighDateTime << createTime.dwLowDateTime << std::endl;
        }
    } while (Thread32Next(threadSnapshot, &threadEntry));

}

BOOL AddRegKey()
{
    HKEY hKey = NULL;

    LSTATUS status = RegCreateKeyExW(
        HKEY_LOCAL_MACHINE,
        L"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network\\Test",
        0,
        REG_NONE,
        REG_OPTION_NON_VOLATILE,
        KEY_WRITE | KEY_WOW64_64KEY | KEY_SET_VALUE,
        NULL,
        &hKey,
        NULL
    );

    std::cout << "res:" << status << std::endl;
    return TRUE;
}

int main() {
    AddRegKey();
	return 0;
}