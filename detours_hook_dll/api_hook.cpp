#define CINTERFACE
#include "api_hook.h"

#include "windows.h"
#include "winuser.h"
#include "shlobj_core.h"

#include <string>

#include "detours.h"
#include "flog.h"

extern HMODULE gDllModule;

std::vector<API2HOOK> apis;

bool DetourTargetProc(void** origin_ptr, void* detour_ptr)
{
	bool bRet = false;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	LONG nErr = DetourAttach(origin_ptr, detour_ptr);

	if (nErr == NO_ERROR)
	{

		bRet = true;
	}
	else
	{
		ERROR_LOG("DetourAttach failed:{}", nErr);
	}

	DetourTransactionCommit();

	return bRet;
}

bool DetourTargetProc(const std::string& dllName, const std::string& procName, void** origin_ptr, void* detour_ptr)
{
	bool bRet = false;
	HMODULE hMod = nullptr;
	do {
		if (origin_ptr == nullptr || detour_ptr == nullptr)
		{
			ERROR_LOG("wrong params");
			break;
		}

		hMod = LoadLibraryA(dllName.c_str());
		if (hMod == nullptr)
		{
			ERROR_LOG("LoadLibraryA failed:{}", GetLastError());
			break;
		}

		*origin_ptr = GetProcAddress(hMod, procName.c_str());
		if (*origin_ptr == nullptr)
		{
			ERROR_LOG("GetProcAddress failed:{}", GetLastError());
			break;
		}

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		LONG nErr = DetourAttach(origin_ptr, detour_ptr);
		
		if (nErr == NO_ERROR)
		{
			
			bRet = true;
		}
		else
		{
			ERROR_LOG("DetourAttach failed:{}", nErr);
		}

		DetourTransactionCommit();

	} while (false);

	if (hMod != nullptr)
	{
		FreeLibrary(hMod);
	}

	return bRet;
}

bool UnDetourTargetProc(void** origin_ptr, void* detour_ptr)
{
	bool bRet = false;

	if (origin_ptr == nullptr || detour_ptr == nullptr)
	{
		return bRet;
	}

	if (*origin_ptr == nullptr)
	{
		return bRet;
	}

	DetourTransactionBegin();

	DetourUpdateThread(GetCurrentThread());

	LONG nErr = DetourDetach(origin_ptr, detour_ptr);
	if (nErr == NO_ERROR)
	{
		bRet = true;
	}
	else
	{
		ERROR_LOG("DetourDetach failed:{}", nErr);
	}
	DetourTransactionCommit();

	return bRet;
}

type_CreateFileW CreateFileW_Origin = nullptr;
HANDLE WINAPI CreateFileW_Mine(
	_In_ LPCWSTR lpFileName,
	_In_ DWORD dwDesiredAccess,
	_In_ DWORD dwShareMode,
	_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	_In_ DWORD dwCreationDisposition,
	_In_ DWORD dwFlagsAndAttributes,
	_In_opt_ HANDLE hTemplateFile
) {

	bool block = false;

	do {
		
		
		wchar_t wAppDataPath[MAX_PATH];
		SHGetSpecialFolderPath(0, wAppDataPath, CSIDL_APPDATA, false);
		std::wstring wstrAppDataPath = wAppDataPath;
		std::wstring wstrFilePath = lpFileName;
		
		

		if (wstrFilePath.find(wstrAppDataPath) == 0 && wstrFilePath.rfind(L"log.txt") != std::wstring::npos)
		{
			INFO_LOG(L"block filepath:{}, wstrAppDataPath:{}", lpFileName, wstrAppDataPath);
			block = true;
		}
	

	} while (false);

	if (block)
	{
		return nullptr;
	}
	else
	{
		return CreateFileW_Origin(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}
}


type_GetOpenFileNameW GetOpenFileNameW_Origin = nullptr;
bool GetOpenedFilePath(wchar_t* pFileInfoBuf, int bufLen, std::wstring& wstrAccessPath)
{
	bool bRet = false;

	if (pFileInfoBuf == NULL || bufLen == 0)
	{
		return bRet;
	}

	int dirLen = wcslen(pFileInfoBuf);
	wstrAccessPath.clear();
	wstrAccessPath.append(pFileInfoBuf, dirLen);

	int index = dirLen;
	int nNameLen = 0;
	wchar_t* pFileName = pFileInfoBuf;
	int i = 0;
	do {
		if (index >= bufLen - 1)
		{
			break;
		}

		pFileName = pFileInfoBuf + index;

		if (pFileName[0] == L'\0' && pFileName[1] == L'\0')
		{
			break;
		}
		nNameLen = wcslen(pFileName + 1);

		if (i == 0)
		{
			wstrAccessPath.append(L"\\").append(pFileName + 1, nNameLen);
		}
		else
		{
			wstrAccessPath.append(L"|").append(pFileName + 1, nNameLen);
		}

		

		index = nNameLen + index + 1;

		i++;
	} while (true);

	bRet = true;

	return bRet;
}
BOOL APIENTRY GetOpenFileNameW_Mine(LPOPENFILENAMEW pOpenFileName)
{
	bool block = false;
	BOOL bRes = FALSE;
	do {
		if (GetOpenFileNameW_Origin == nullptr)
		{
			ERROR_LOG("GetOpenFileNameW_Origin == nullptr");
			break;
		}

		if (GetOpenFileNameW_Origin == GetOpenFileNameW_Mine)
		{
			ERROR_LOG("GetOpenFileNameW_Origin == GetOpenFileNameW_Mine");
			break;
		}
	
		bRes = GetOpenFileNameW_Origin(pOpenFileName);
		if (bRes)
		{
			INFO_LOG(L"select filepath:{} ", pOpenFileName->lpstrFile);
	
			std::wstring wstrFilePath;
			GetOpenedFilePath(pOpenFileName->lpstrFile, pOpenFileName->nMaxFile, wstrFilePath);

			INFO_LOG(L"wstrFilePath:{}", wstrFilePath);
			
		}

	} while (false);

	if (block)
	{
		return FALSE;
	}
	else
	{
		return bRes;
	}
	
}

type_DragQueryFileW DragQueryFileW_Origin = nullptr;

bool GetClipboardDropFile(HANDLE hFile)
{
	HDROP hDrop = (HDROP)GlobalLock(hFile);

	int fileNum = DragQueryFileW_Origin(hDrop, UINT32_MAX, NULL, NULL);

	INFO_LOG("fileNum:{}", fileNum);

	wchar_t wFilePath[MAX_PATH * 2];

	for (int i = 0; i < fileNum; i++)
	{
		memset(wFilePath, 0, MAX_PATH * 2);
		DragQueryFileW_Origin(hDrop, i, wFilePath, MAX_PATH * 2);
		INFO_LOG(L"filepath:{}", wFilePath);
	}
	GlobalUnlock(hFile);
	return true;
}
type_GetClipboardData GetClipboardData_Origin = nullptr;
HANDLE WINAPI GetClipboardData_Mine(_In_ UINT uFormat)
{
	INFO_LOG("uFormat:{}", uFormat);

	HANDLE hRet = GetClipboardData_Origin(uFormat);

	do {
		if (hRet == nullptr || hRet == INVALID_HANDLE_VALUE)
		{
			break;
		}


		if (uFormat == CF_HDROP /* || uFormat > 0x12*/)
		{
			if (!IsClipboardFormatAvailable(CF_HDROP))
			{
				break;
			}


			GetClipboardDropFile(hRet);
			EmptyClipboard();
			hRet = NULL;
		}

	} while (false);

	return hRet;
}

UINT STDAPICALLTYPE DragQueryFileW_Mine(_In_ HDROP hDrop, _In_ UINT iFile, _Out_writes_opt_(cch) LPWSTR lpszFile, _In_ UINT cch)
{
	auto nRet = DragQueryFileW_Origin(hDrop, iFile, lpszFile, cch);

	POINT point;
	BOOL b = DragQueryPoint(hDrop, &point);

	if (lpszFile != nullptr)
	{
		INFO_LOG(L"DragQueryFileW_Mine iFile:{} lpszFile:{}, nRet:{}, {} {}, {}", iFile, lpszFile, nRet, point.x, point.y, b);
	}
	else
	{
		INFO_LOG(L"DragQueryFileW_Mine iFile:{} nRet:{}, {} {}, {}", iFile, nRet, point.x, point.y, b);
	}
	
	/*if (lpszFile != nullptr)
	{	
		memset(lpszFile, 0, nRet * 2);
		nRet = 0;
	}*/

	return nRet;
}


//type_DoDragDrop DoDragDrop_Origin = nullptr;
//HRESULT STDAPICALLTYPE DoDragDrop_Mine(IN LPDATAOBJECT pDataObj, IN LPDROPSOURCE pDropSource, IN DWORD dwOKEffects, OUT LPDWORD pdwEffect)
//{
//	FORMATETC formateTC = { 0 };
//	STGMEDIUM stgMedium = { 0 };
//
//	formateTC.cfFormat = CF_HDROP;
//	HRESULT result = pDataObj->GetData(&formateTC, &stgMedium);
//	if (result != S_OK)
//	{
//		ERROR_LOG("pDataObj->GetData, error:{}", result);
//	}
//	else
//	{
//		auto fileNum = DragQueryFileW_Origin((HDROP)stgMedium.hGlobal, UINT32_MAX, NULL, NULL);
//		INFO_LOG("fileNum:{}", fileNum);
//	}
//	
//
//	return DRAGDROP_S_CANCEL;
//}

type_CreateProcessW CreateProcessW_Origin = nullptr;
BOOL WINAPI CreateProcessW_Mine(
	_In_opt_ LPCWSTR lpApplicationName,
	_Inout_opt_ LPWSTR lpCommandLine,
	_In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
	_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
	_In_ BOOL bInheritHandles,
	_In_ DWORD dwCreationFlags,
	_In_opt_ LPVOID lpEnvironment,
	_In_opt_ LPCWSTR lpCurrentDirectory,
	_In_ LPSTARTUPINFOW lpStartupInfo,
	_Out_ LPPROCESS_INFORMATION lpProcessInformation
) {
	INFO_LOG(L"lpApplicationName:{}", lpApplicationName);
	INFO_LOG(L"lpCommandLine1:{}", lpCommandLine);

	return CreateProcessW_Origin(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, 
		bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
}

type_CopyFileW CopyFileW_Origin = nullptr;
BOOL WINAPI CopyFileW_Mine(_In_ LPCWSTR lpExistingFileName, _In_ LPCWSTR lpNewFileName, _In_ BOOL bFailIfExists)
{
	INFO_LOG(L"cpoy: {}=>{}, {}", lpExistingFileName, lpNewFileName, bFailIfExists);
	return CopyFileW_Origin(lpExistingFileName, lpNewFileName, bFailIfExists);
}

type_ReadFile ReadFile_Origin = nullptr;
BOOL WINAPI ReadFile_Mine(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
	wchar_t lpFilePath[MAX_PATH];
	GetFinalPathNameByHandleW(hFile, lpFilePath, MAX_PATH, NULL);
	INFO_LOG(L"filepath:{}", lpFilePath);
	return ReadFile_Origin(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}

type_IDropTarget_Drop IDropTarget_Drop_Origin = nullptr;
HRESULT STDMETHODCALLTYPE IDropTarget_Drop_Mine(IDropTarget* pDropTarget, IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
{

	STGMEDIUM stgMedium = { 0 };
	FORMATETC formateTC = { 0 };
	formateTC.cfFormat = CF_HDROP;
	formateTC.dwAspect = DVASPECT_CONTENT;
	formateTC.lindex = -1;
	formateTC.tymed = TYMED_HGLOBAL;
	pDataObj->lpVtbl->GetData(pDataObj, &formateTC, &stgMedium);
	auto fileNum = DragQueryFileW_Origin((HDROP)stgMedium.hGlobal, UINT32_MAX, NULL, NULL);


	INFO_LOG("fileNum:{}", fileNum);

	wchar_t fileName[MAX_PATH] = { 0 };
	for (int i = 0; i < fileNum; i++)
	{
		memset(fileName, 0, MAX_PATH * 2);
		DragQueryFileW_Origin((HDROP)stgMedium.hGlobal, i, fileName, MAX_PATH);
		INFO_LOG(L"fileName:{}", fileName);
	}

	/*stgMedium.hGlobal = NULL;
	pDataObj->lpVtbl->SetData(pDataObj, &formateTC, &stgMedium, TRUE);*/
	
	// pDropTarget->lpVtbl->DragOver(pDropTarget, grfKeyState, pt, pdwEffect);
	return pDropTarget->lpVtbl->DragLeave(pDropTarget);

	// return E_INVALIDARG;
	//return IDropTarget_Drop_Origin(pDropTarget, pDataObj, grfKeyState, pt, pdwEffect);
}

bool once = true;
type_RegisterDragDrop RegisterDragDrop_Origin = nullptr;
HRESULT STDAPICALLTYPE RegisterDragDrop_Mine(IN HWND hwnd, IN LPDROPTARGET pDropTarget)
{
	if (once)
	{
		IDropTarget_Drop_Origin = (type_IDropTarget_Drop)pDropTarget->lpVtbl->Drop;
		if (!DetourTargetProc((void**)&IDropTarget_Drop_Origin, IDropTarget_Drop_Mine))
		{
			ERROR_LOG("DetourTargetProc failed, IDropTarget::Drop");
		}
		apis.push_back(API2HOOK{ "", "", (void**)&IDropTarget_Drop_Origin, IDropTarget_Drop_Mine });
		once = false;
	}

	return RegisterDragDrop_Origin(hwnd, pDropTarget);
}


type_IFileOpenDialog_Show IFileOpenDialog_Show_Origin = nullptr;
HRESULT STDMETHODCALLTYPE IFileOpenDialog_Show_Mine(IFileOpenDialog* pFileOpenDialog, HWND hwndOwner)
{
	HRESULT nRet = IFileOpenDialog_Show_Origin(pFileOpenDialog, hwndOwner);

	IShellItemArray* pShellItemArray = nullptr;
	IShellItem* pShellItem = nullptr;
	bool block = true;

	do {
		if (nRet == HRESULT_FROM_WIN32(ERROR_CANCELLED))
		{
			break;
		}
		if (!SUCCEEDED(nRet))
		{
			break;
		}
		
		nRet = pFileOpenDialog->lpVtbl->GetResults(pFileOpenDialog, &pShellItemArray);
		if (!SUCCEEDED(nRet) || pShellItemArray == nullptr)
		{
			ERROR_LOG("GetResults failed:{}", nRet);
			break;
		}

		DWORD nItemNum = 0;
		nRet = pShellItemArray->lpVtbl->GetCount(pShellItemArray, &nItemNum);
		if (!SUCCEEDED(nRet))
		{
			ERROR_LOG("GetCount failed:{}", nRet);
			break;
		}

		LPWSTR displayName = nullptr;
		for (int i = 0; i < nItemNum; i++)
		{
			pShellItem = nullptr, displayName = nullptr;
			nRet = pShellItemArray->lpVtbl->GetItemAt(pShellItemArray, i, &pShellItem);
			if (!SUCCEEDED(nRet) || pShellItem == nullptr)
			{
				ERROR_LOG("GetItemAt:{} failed:{}", i, nRet);
				continue;
			}

			nRet = pShellItem->lpVtbl->GetDisplayName(pShellItem, SIGDN_FILESYSPATH, &displayName);
			if (SUCCEEDED(nRet) && displayName != nullptr)
			{
				INFO_LOG(L"displayName:{}", displayName);
				CoTaskMemFree(displayName);
			}
			else
			{
				ERROR_LOG("GetDisplayName failed:{}", nRet);
			}
			pShellItem->lpVtbl->Release(pShellItem);
		}

	} while (false);

	if (pShellItem != nullptr)
	{
		pShellItem->lpVtbl->Release(pShellItem);
	}
	if (pShellItemArray != nullptr)
	{
		pShellItemArray->lpVtbl->Release(pShellItemArray);
	}

	if (block)
	{
		return HRESULT_FROM_WIN32(ERROR_CANCELLED);
	}
	else
	{
		return S_OK;
	}
	
}

bool hookIFileDialog()
{
	INFO_LOG("");
	IFileOpenDialog* pFileOpenDialog = nullptr;
	// auto res = CoInitializeEx(NULL, COINIT_DISABLE_OLE1DDE);
	LRESULT res = CoInitialize(0); 
	if (res != S_OK && res != S_FALSE)
	{
		ERROR_LOG("CoInitialize failed, res:{}", res);
	}
	res = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_IFileOpenDialog, (void**)&pFileOpenDialog);
	if (res != S_OK || pFileOpenDialog == nullptr)
	{
		ERROR_LOG("CoCreateInstance failed£¬ res:{}", res);
		return false;
	}
	IFileOpenDialog_Show_Origin = (type_IFileOpenDialog_Show)pFileOpenDialog->lpVtbl->Show;
	if (!DetourTargetProc((void**)&IFileOpenDialog_Show_Origin, IFileOpenDialog_Show_Mine))
	{
		ERROR_LOG("DetourTargetProc failed, IModalWindow_Show");
	}
	apis.push_back(API2HOOK{ "", "", (void**)&IFileOpenDialog_Show_Origin, IFileOpenDialog_Show_Mine });

	pFileOpenDialog->lpVtbl->Release(pFileOpenDialog);
	return true;
}

HMODULE gHMod;
bool DelayFreeHandle(HMODULE hMod)
{
	for (int i = 0; i < 50; i++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		INFO_LOG("waiting:{}...", i);
	}
	FreeLibrary(hMod);
	return true;
}

std::string dllDir = "";
bool hook()
{
	
	char lpDllPath[MAX_PATH];
	GetModuleFileNameA(gDllModule, lpDllPath, MAX_PATH);
	std::string strDllPath = lpDllPath;
	auto strDllDir = strDllPath.substr(0, strDllPath.find_last_of("\\"));
	dllDir = strDllDir;
	LOG_INIT("test", strDllDir + "\\log.txt");
	INFO_LOG("*****test*****");
	
	gHMod = LoadLibraryA("D:\\CodeBench\\c&cpp\\WDMexample\\Release\\detours_hook_dll.dll");
	std::thread(DelayFreeHandle, gHMod).detach();

	apis.push_back(API2HOOK{"kernel32.dll", "CreateFileW", (void**)&CreateFileW_Origin, CreateFileW_Mine});
	apis.push_back(API2HOOK{ "comdlg32.dll", "GetOpenFileNameW", (void**)&GetOpenFileNameW_Origin, GetOpenFileNameW_Mine });
	apis.push_back(API2HOOK{ "user32.dll", "GetClipboardData", (void**)&GetClipboardData_Origin, GetClipboardData_Mine });
	apis.push_back(API2HOOK{ "Shell32.dll", "DragQueryFileW", (void**)&DragQueryFileW_Origin, DragQueryFileW_Mine });
	apis.push_back(API2HOOK{ "kernel32.dll", "CreateProcessW", (void**)&CreateProcessW_Origin, CreateProcessW_Mine });
	apis.push_back(API2HOOK{ "kernel32.dll", "CopyFileW", (void**)&CopyFileW_Origin, CopyFileW_Mine });
	// apis.push_back(API2HOOK{ "kernel32.dll", "ReadFile", (void**)&ReadFile_Origin, ReadFile_Mine });
	apis.push_back(API2HOOK{ "ole32.dll", "RegisterDragDrop", (void**)&RegisterDragDrop_Origin, RegisterDragDrop_Mine });
	// apis.push_back(API2HOOK{ "Ole32.dll", "DoDragDrop", (void**)&DoDragDrop_Origin, DoDragDrop_Mine });
	
	for (auto api : apis)
	{
		if (!DetourTargetProc(api.dllName, api.procName, api.originProcPtr, api.mineProcPtr))
		{
			ERROR_LOG("DetourTargetProc({}, {},...) failed", api.dllName, api.procName);
		}
		else
		{
			INFO_LOG("DetourTargetProc({}, {},...) success", api.dllName, api.procName);
		}
	}
	hookIFileDialog();
	INFO_LOG("hook done!");
	return true;
}

bool unhook()
{
	for (auto api : apis)
	{
		if (!UnDetourTargetProc(api.originProcPtr, api.mineProcPtr))
		{
			ERROR_LOG("UnDetourTargetProc({}, {}) dll:{} proc:{} success", 
				(uintptr_t)api.originProcPtr, (uintptr_t)api.mineProcPtr, api.dllName, api.procName);
		}
		else
		{
			INFO_LOG("UnDetourTargetProc() dll:{} proc: {} success", api.dllName, api.procName);
		}
	}

	LOG_UNINIT();

	return true;
}