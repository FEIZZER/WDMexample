#pragma once
#include <string>
#include <sstream>
#include <Windows.h>
#include <DbgHelp.h>
#include <TlHelp32.h>
#include <Shlwapi.h>
#include "debug_helper.hpp"
namespace ez {

inline unsigned int execute_command_line(const std::string& cmd, const std::string&)
{

}

class windows_dump
{
public:
	windows_dump() {
		wchar_t path[MAX_PATH] = { 0 };
		HMODULE hMod = GetModuleHandleW(NULL);
		GetModuleFileNameW(hMod, path, MAX_PATH);
		currentModulePath = path;
	}
	~windows_dump() = default;

	static const std::wstring formatTimeString() {
		std::tm tm;
		auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		localtime_s(&tm, &t);
		std::wstringstream ws;
		ws << std::put_time(&tm, L"%Y%m%d%H%M%S");
		return ws.str();
	}

	static LONG __stdcall UnHandleExceptionFilter(PEXCEPTION_POINTERS pException);

	void SetExceptionFilter() {
		previous_exception_filter_ = SetUnhandledExceptionFilter(UnHandleExceptionFilter);
	}

private:


	/*using tMiniDumpWriteDump = BOOL (__stdcall*)(
		HANDLE hProcess,DWORD ProcessId, 
		HANDLE hFile, MINIDUMP_TYPE DumpType, 
		PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
		PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam, 
		PMINIDUMP_CALLBACK_INFORMATION CallbackParam);*/
	using tMiniDumpWriteDump = decltype(&MiniDumpWriteDump);
	static bool WriteMiniDump(PEXCEPTION_POINTERS pException) 
	{
		HMODULE hDll = NULL;
		HANDLE hFile = NULL;
		tMiniDumpWriteDump pMiniDumpWriteDump = NULL;

		auto newDumpFile = currentModulePath + L"." + formatTimeString() + L".dmp";

		do {
			hDll = LoadLibraryW(L"dbghelp.dll");
			if (hDll == NULL) {
				break;
			}

			pMiniDumpWriteDump = (tMiniDumpWriteDump)GetProcAddress(hDll, "MiniDumpWriteDump");
			if (pMiniDumpWriteDump == NULL) {
				break;
			}

			// suspendthread
			hFile = CreateFileW(newDumpFile.c_str(), GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_FLAG_WRITE_THROUGH, 0);
			if (hFile == NULL || hFile == INVALID_HANDLE_VALUE) {
				break;
			}

			MINIDUMP_EXCEPTION_INFORMATION miniDumpExceptionInfo = { 0 };
			miniDumpExceptionInfo.ThreadId = GetCurrentThreadId();
			miniDumpExceptionInfo.ExceptionPointers = pException;
			miniDumpExceptionInfo.ClientPointers = FALSE;
			MINIDUMP_CALLBACK_INFORMATION miniDumpCallbackInfo = { 0 };
			miniDumpCallbackInfo.CallbackParam = 0;
			miniDumpCallbackInfo.CallbackRoutine = 0;
			pMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, 
				pException ? &miniDumpExceptionInfo : NULL, NULL, &miniDumpCallbackInfo);
		} while (false);

		if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hFile);
		}

		if (hDll != NULL && hDll != INVALID_HANDLE_VALUE)
		{
			FreeLibrary(hDll);
		}

		return true;
	}

private:
	static inline std::wstring currentModulePath = L"";
	static inline  LPTOP_LEVEL_EXCEPTION_FILTER previous_exception_filter_ = nullptr;
};


LONG __stdcall windows_dump::UnHandleExceptionFilter(PEXCEPTION_POINTERS pException) {

	WriteMiniDump(pException);

	if (previous_exception_filter_ == nullptr) {
		return EXCEPTION_CONTINUE_SEARCH;
	}
	else {
		return previous_exception_filter_(pException);
	}
}

}