#pragma once

#include "windows.h"
#include "ShObjIdl_core.h"
#include "shlobj_core.h"
#include <string>

bool hook();

bool unhook();

using type_CreateFileW = HANDLE (WINAPI*)(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, 
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
using type_MoveFileW			= BOOL (WINAPI*)(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName);

using type_GetOpenFileNameW		= BOOL (APIENTRY*)(LPOPENFILENAMEW);
using type_GetOpenFileNameA		= BOOL (APIENTRY*)(LPOPENFILENAMEA);
using type_GetClipboardData		= HANDLE (WINAPI*)(_In_ UINT uFormat);
using type_DragQueryFileW		= UINT (STDAPICALLTYPE*)(_In_ HDROP hDrop, _In_ UINT iFile, _Out_writes_opt_(cch) LPWSTR lpszFile, _In_ UINT cch);
using type_DragQueryFileA		= UINT (STDAPICALLTYPE*)(_In_ HDROP hDrop, _In_ UINT iFile, _Out_writes_opt_(cch) LPSTR lpszFile, _In_ UINT cch);
using type_DoDragDrop			= HRESULT (STDAPICALLTYPE*) (IN LPDATAOBJECT pDataObj, IN LPDROPSOURCE pDropSource, IN DWORD dwOKEffects, OUT LPDWORD pdwEffect);
using type_CopyFileW			= BOOL (WINAPI*)(_In_ LPCWSTR lpExistingFileName, _In_ LPCWSTR lpNewFileName, _In_ BOOL bFailIfExists);
using type_ReadFile				= BOOL (WINAPI*)(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
using type_WriteFile			= BOOL(WINAPI*)(HANDLE hFile, LPCVOID lpBuffer, _In_ DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
using type_CreateFileMapping	= HANDLE (WINAPI*)(HANDLE hFile, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCWSTR lpName);
using type_RegisterDragDrop		= HRESULT(STDAPICALLTYPE*)(IN HWND hwnd, IN LPDROPTARGET pDropTarget);
using type_CreateProcessW		= BOOL (WINAPI*)(_In_opt_ LPCWSTR lpApplicationName, _Inout_opt_ LPWSTR lpCommandLine, 
	_In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,	_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes, _In_ BOOL bInheritHandles, _In_ DWORD dwCreationFlags,
	_In_opt_ LPVOID lpEnvironment, _In_opt_ LPCWSTR lpCurrentDirectory, _In_ LPSTARTUPINFOW lpStartupInfo, _Out_ LPPROCESS_INFORMATION lpProcessInformation
);

using type_IDropTarget_Drop		= HRESULT(STDMETHODCALLTYPE*)(IDropTarget* pDropTarget, IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
using type_IDropTarget_DragEnter= HRESULT(STDMETHODCALLTYPE*)(IDropTarget* pDropTarget, IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
using type_IFileOpenDialog_Show	= HRESULT (STDMETHODCALLTYPE*)(IFileOpenDialog* This, HWND hwndOwner);
using type_IFileDialog_Show		= HRESULT (STDMETHODCALLTYPE*)(IFileDialog* This, HWND hwndOwner);
using type_IFileOpenDialog_QueryInterface = HRESULT(STDMETHODCALLTYPE*)(IFileOpenDialog* This, REFIID riid, void** ppvObject);
using type_IFileOperation_CopyItems = HRESULT (STDMETHODCALLTYPE*)(IFileOperation* pFileOperation, IUnknown* punkItems, IShellItem* psiDestinationFolder);


using type_SHBrowseForFolderW	= PIDLIST_ABSOLUTE(STDAPICALLTYPE*) (_In_ LPBROWSEINFOW lpbi);
using type_SHBrowseForFolderA	= PIDLIST_ABSOLUTE(STDAPICALLTYPE*) (_In_ LPBROWSEINFOA lpbi);

using type_CoCreateInstanceEx	= HRESULT (STDAPICALLTYPE*)(REFCLSID Clsid, LPUNKNOWN punkOuter, DWORD dwClsCtx, COSERVERINFO* pServerInfo, DWORD dwCount, MULTI_QI* pResults);
using type_CoCreateInstance		= HRESULT(STDAPICALLTYPE*)(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID  FAR* ppv);
using type_OleGetClipboard		= HRESULT (STDAPICALLTYPE *)(LPDATAOBJECT FAR* ppDataObj);

typedef struct _API2HOOK {
	std::string dllName;
	std::string procName;
	void**		originProcPtr;
	void*		mineProcPtr;

}API2HOOK, *PAPI2HOOK;

