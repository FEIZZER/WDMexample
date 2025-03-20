#include <Windows.h>
#include <minwindef.h>
#include "flog.h"

HANDLE th = NULL;
bool exit_ = false;

DWORD WINAPI thread_run(void*)
{
    int i = 0;

    while (!exit_) {
        i++;
        INFO_LOG("wait... {}", i);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    INFO_LOG("thread done");
    return 0;
}

void init() {
    LOG_INIT("test", "c:\\users\\hillstone\\desktop\\test.log");
    INFO_LOG("init");
    th = CreateThread(NULL, 0, thread_run, NULL, NULL, NULL);

}
#include <strsafe.h>
VOID WINAPI PrintMsgA(IN LPSTR strOutputString, ...)
{
    char strBuffer[512] = { NULL };
    va_list vlArgs;

    va_start(vlArgs, strOutputString);
    StringCbVPrintfA(strBuffer, sizeof(strBuffer) - 1, strOutputString, vlArgs);
    va_end(vlArgs);

    OutputDebugStringA(strBuffer);
    OutputDebugStringA("\n");
}


char* output_memory = new char[STRSAFE_MAX_CCH];

void __stdcall OutputDebugA(char* output_string, ...)
{
    // STRSAFE_MAX_CCH * sizeof(TCHAR)
    memset(output_memory, 0, STRSAFE_MAX_CCH);
    va_list al_args;
    va_start(al_args, output_string);
    StringCbVPrintfA(output_memory, STRSAFE_MAX_CCH - 1, output_string, al_args);
    va_end(al_args);
    OutputDebugStringA(output_memory);
    OutputDebugStringA("\n");
}

void uninit() {
    INFO_LOG("uninit");
    exit_ = true;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    CloseHandle(th);
    LOG_UNINIT();
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        init();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        uninit();
        break;
    }
    return TRUE;
}

