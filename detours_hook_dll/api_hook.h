#pragma once

#include "windows.h"
#include "ShObjIdl_core.h"
#include <string>

bool hook();

bool unhook();

using type_CreateFileW = HANDLE (WINAPI*)
(
	_In_ LPCWSTR lpFileName,
	_In_ DWORD dwDesiredAccess,
	_In_ DWORD dwShareMode,
	_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	_In_ DWORD dwCreationDisposition,
	_In_ DWORD dwFlagsAndAttributes,
	_In_opt_ HANDLE hTemplateFile
);


using type_GetOpenFileNameW		= BOOL (APIENTRY*)(LPOPENFILENAMEW);
using type_GetClipboardData		= HANDLE (WINAPI*)(_In_ UINT uFormat);
using type_DragQueryFileW		= UINT (STDAPICALLTYPE*)(_In_ HDROP hDrop, _In_ UINT iFile, _Out_writes_opt_(cch) LPWSTR lpszFile, _In_ UINT cch);
using type_DoDragDrop			= HRESULT (STDAPICALLTYPE*) (IN LPDATAOBJECT pDataObj, IN LPDROPSOURCE pDropSource, IN DWORD dwOKEffects, OUT LPDWORD pdwEffect);
using type_CopyFileW			= BOOL (WINAPI*)(_In_ LPCWSTR lpExistingFileName, _In_ LPCWSTR lpNewFileName, _In_ BOOL bFailIfExists);
using type_ReadFile				= BOOL (WINAPI*)(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
using type_RegisterDragDrop		= HRESULT(STDAPICALLTYPE*)(IN HWND hwnd, IN LPDROPTARGET pDropTarget);
using type_CreateProcessW		= BOOL (WINAPI*)(_In_opt_ LPCWSTR lpApplicationName, _Inout_opt_ LPWSTR lpCommandLine, 
	_In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,	_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes, _In_ BOOL bInheritHandles, _In_ DWORD dwCreationFlags,
	_In_opt_ LPVOID lpEnvironment, _In_opt_ LPCWSTR lpCurrentDirectory, _In_ LPSTARTUPINFOW lpStartupInfo, _Out_ LPPROCESS_INFORMATION lpProcessInformation
);

using type_IDropTarget_Drop		= HRESULT(STDMETHODCALLTYPE*)(IDropTarget* pDropTarget, IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
using type_IFileOpenDialog_Show	= HRESULT (STDMETHODCALLTYPE*)(IFileOpenDialog* This, HWND hwndOwner);
typedef struct _API2HOOK {
	std::string dllName;
	std::string procName;
	void**		originProcPtr;
	void*		mineProcPtr;

}API2HOOK, *PAPI2HOOK;
