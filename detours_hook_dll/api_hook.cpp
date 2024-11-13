#define CINTERFACE
#include "api_hook.h"

#include "windows.h"
#include "winuser.h"
#include "shlobj_core.h"
#include "psapi.h"
#include "objidl.h"


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
		// INFO_LOG(L"filepath:{}", lpFileName);
		std::wstring wstrFilePath = lpFileName;
		if (wstrFilePath.rfind(L"test.txt") != std::wstring::npos)
		{
			// INFO_LOG(L"block filepath:{}", lpFileName);
			// block = true;
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

type_MoveFileW MoveFileW_Origin = nullptr;


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
	INFO_LOG("");
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

	block = true;

	if (block)
	{
		return FALSE;
	}
	else
	{
		return bRes;
	}
}

type_GetOpenFileNameA GetOpenFileNameA_Origin = nullptr;
BOOL APIENTRY GetOpenFileNameA_Mine(LPOPENFILENAMEA pOpenFileName)
{
	INFO_LOG("");
	return GetOpenFileNameA_Origin(pOpenFileName);
}

type_DragQueryFileW DragQueryFileW_Origin = nullptr;
bool GetClipboardDropFile(HANDLE hFile)
{
	HDROP hDrop = (HDROP)hFile;
	int fileNum = 0;
	if (DragQueryFileW_Origin != nullptr)
	{
		INFO_LOG("here");
		fileNum = DragQueryFileW_Origin(hDrop, 0xFFFFFFFF, NULL, NULL);
	}
	else 
	{
		fileNum = DragQueryFileW(hDrop, 0xFFFFFFFF, NULL, NULL);
	}
	INFO_LOG("fileNum:{}", fileNum);

	wchar_t wFilePath[MAX_PATH * 2];

	for (int i = 0; i < fileNum; i++)
	{
		memset(wFilePath, 0, MAX_PATH * 2);
		if (DragQueryFileW_Origin != nullptr)
			DragQueryFileW_Origin(hDrop, i, wFilePath, MAX_PATH * 2);
		else
			DragQueryFileW(hDrop, i, wFilePath, MAX_PATH * 2);
		INFO_LOG(L"filepath:{}", wFilePath);
	}
	GlobalUnlock(hFile);
	return true;
}
type_GetClipboardData GetClipboardData_Origin = nullptr;
HANDLE WINAPI GetClipboardData_Mine(_In_ UINT uFormat)
{



	HANDLE hRet = GetClipboardData_Origin(uFormat);

	INFO_LOG(L"uFormat:{} hRet:{}", uFormat, (uintptr_t)hRet);
	
	do {
		if (hRet == NULL || hRet == INVALID_HANDLE_VALUE)
		{
			INFO_LOG("GetClipboardData_Origin return NULL");
			break;
		}


		if (uFormat == CF_HDROP   || uFormat > 0x12)
		{ 
			if (!IsClipboardFormatAvailable(CF_HDROP))
			{
				break;
			}

			HANDLE hFile = GetClipboardData_Origin(CF_HDROP);
			if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
			{
				break;
			}
			GetClipboardDropFile(hFile);
			hFile = NULL;
		}
		else
		{
			ERROR_LOG("no CF_HDROP");
		}

	} while (false);

	return hRet;
}


UINT STDAPICALLTYPE DragQueryFileW_Mine(_In_ HDROP hDrop, _In_ UINT iFile, _Out_writes_opt_(cch) LPWSTR lpszFile, _In_ UINT cch)
{
	auto nRet = DragQueryFileW_Origin(hDrop, iFile, lpszFile, cch);

	POINT point;

	if (lpszFile != nullptr)
	{
		INFO_LOG(L"DragQueryFileW_Mine iFile:{} lpszFile:{}, nRet:{}", iFile, lpszFile, nRet);
		memset(lpszFile, 0, nRet * 2);
		nRet = 0;
	}
	else
	{
		INFO_LOG(L"DragQueryFileW_Mine iFile:{} nRet:{}", iFile, nRet);
	}
	return nRet;
}

type_DragQueryFileA DragQueryFileA_Origin = nullptr;
UINT STDAPICALLTYPE DragQueryFileA_Mine(_In_ HDROP hDrop, _In_ UINT iFile, _Out_writes_opt_(cch) LPSTR lpszFile, _In_ UINT cch)
{
	INFO_LOG("");
	return DragQueryFileA_Origin(hDrop, iFile, lpszFile, cch);
}

type_DoDragDrop DoDragDrop_Origin = nullptr;
HRESULT STDAPICALLTYPE DoDragDrop_Mine(IN LPDATAOBJECT pDataObj, IN LPDROPSOURCE pDropSource, IN DWORD dwOKEffects, OUT LPDWORD pdwEffect)
{
	INFO_LOG("");
	STGMEDIUM stgMedium = { 0 };
	FORMATETC formateTC = { 0 };
	/*formateTC.cfFormat = CF_HDROP;
	formateTC.dwAspxiaect = DVASPECT_CONTENT;
	formateTC.lindex = -1;
	formateTC.tymed = TYMED_HGLOBAL;*/

	
	IEnumFORMATETC* pEnumFTC = nullptr;
	pDataObj->lpVtbl->EnumFormatEtc(pDataObj, DATADIR_GET, &pEnumFTC);
	while (true) {
		if (pEnumFTC == nullptr) break;

		if (pEnumFTC->lpVtbl->Next(pEnumFTC, (ULONG)1, &formateTC, NULL) != S_OK)
		{
			ERROR_LOG("Next failed:{}");
			break;
		}

		INFO_LOG("cfFormat:{} tymed:{}", formateTC.cfFormat, formateTC.tymed);
	}

	
	/*auto fileNum = DragQueryFileW_Origin((HDROP)stgMedium.hGlobal, UINT32_MAX, NULL, NULL);
	INFO_LOG("fileNum:{}", fileNum);*/
	return DRAGDROP_S_CANCEL;
}

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

	// return FALSE;

	// std::this_thread::sleep_for(std::chrono::milliseconds(50000));

	return CopyFileW_Origin(lpExistingFileName, lpNewFileName, bFailIfExists);
}

type_ReadFile ReadFile_Origin = nullptr;
BOOL WINAPI ReadFile_Mine(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
	char lpFilePath[MAX_PATH];
	if (!GetFinalPathNameByHandleA(hFile, lpFilePath, MAX_PATH, 0))
	{
		ERROR_LOG("GetFinalPathNameByHandleA failed:{}", GetLastError());
	}
	else if (strlen(lpFilePath) > 0)
	{
		INFO_LOG("filepath:{}", lpFilePath);
	}

	std::string strSuffix = "test_file.txt";
	std::string strFilePath = lpFilePath;
	if (strFilePath.rfind(strSuffix) == strFilePath.length() - strSuffix.length())
	{
		INFO_LOG("block filepath:{}", strFilePath);
		return FALSE;
	}
	else
	{
		return ReadFile_Origin(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
	}

}

type_WriteFile WriteFile_Origin = nullptr;
BOOL WINAPI WriteFile_Mine(HANDLE hFile, LPCVOID lpBuffer, _In_ DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
	char lpFilePath[MAX_PATH] = { 0 };
	DWORD size = MAX_PATH;
	QueryFullProcessImageNameA(hFile, 0, lpFilePath, &size);
	
	if (!GetFinalPathNameByHandleA(hFile, lpFilePath, MAX_PATH, 0))
	{
		ERROR_LOG("GetFinalPathNameByHandleA failed:{}", GetLastError());
	}
	else if (strlen(lpFilePath) > 0)
	{
		INFO_LOG("filepath:{}", lpFilePath);
	}

	std::string strSuffix = "test_file.txt";
	std::string strFilePath = lpFilePath;
	if (strFilePath.rfind(strSuffix) == strFilePath.length() - strSuffix.length())
	{
		INFO_LOG("block filepath:{}", strFilePath);
		return FALSE;
	}
	else
	{
		return WriteFile_Origin(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
	}

}

type_CreateFileMapping CreateFileMappingW_Origin = nullptr;
HANDLE WINAPI CreateFileMappingW_Mine(HANDLE hFile, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, 
	DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCWSTR lpName)
{
	char lpExePath[MAX_PATH] = { 0 };
	DWORD size = MAX_PATH;

	if (!GetFinalPathNameByHandleA(hFile, lpExePath, MAX_PATH, 0))
	{
		ERROR_LOG("GetFinalPathNameByHandleA failed:{}", GetLastError());
	}
	else if (strlen(lpExePath) > 0)
	{
		INFO_LOG("filepath:{}", lpExePath);
	}
	return CreateFileMappingW_Origin(hFile, lpFileMappingAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName);
}

type_IDropTarget_DragEnter IDropTarget_DragEnter_Origin = nullptr;
HRESULT STDMETHODCALLTYPE IDropTarget_DragEnter_Mine(IDropTarget* pDropTarget, IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
{
	STGMEDIUM stgMedium = { 0 };
	FORMATETC formateTC = { 0 };
	formateTC.cfFormat = CF_HDROP;
	formateTC.dwAspxiaect = DVASPECT_CONTENT;
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

	return E_INVALIDARG;
}

type_IDropTarget_Drop IDropTarget_Drop_Origin = nullptr;
HRESULT STDMETHODCALLTYPE IDropTarget_Drop_Mine(IDropTarget* pDropTarget, IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
{

	STGMEDIUM stgMedium = { 0 };
	FORMATETC formateTC = { 0 };
	formateTC.cfFormat = CF_HDROP;
	formateTC.dwAspxiaect = DVASPECT_CONTENT;
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
	
	// return pDropTarget->lpVtbl->DragLeave(pDropTarget);

	 return E_INVALIDARG;
	//return IDropTarget_Drop_Origin(pDropTarget, pDataObj, grfKeyState, pt, pdwEffect);
}


bool once = true;
type_RegisterDragDrop RegisterDragDrop_Origin = nullptr;
HRESULT STDAPICALLTYPE RegisterDragDrop_Mine(IN HWND hwnd, IN LPDROPTARGET pDropTarget)
{
	INFO_LOG("");
	if (once)
	{
		IDropTarget_Drop_Origin = (type_IDropTarget_Drop)pDropTarget->lpVtbl->Drop;
		if (!DetourTargetProc((void**)&IDropTarget_Drop_Origin, IDropTarget_Drop_Mine))
		{
			ERROR_LOG("DetourTargetProc failed, IDropTarget::Drop");
		}
		apis.push_back(API2HOOK{ "", "", (void**)&IDropTarget_Drop_Origin, IDropTarget_Drop_Mine });

		IDropTarget_DragEnter_Origin = pDropTarget->lpVtbl->DragEnter;
		if (!DetourTargetProc((void**)&IDropTarget_DragEnter_Origin, IDropTarget_DragEnter_Mine))
		{
			ERROR_LOG("DetourTargetProc failed, IDropTarget::Drop");
		}
		apis.push_back(API2HOOK{ "", "", (void**)&IDropTarget_DragEnter_Origin, IDropTarget_DragEnter_Mine });

		once = false;
	}

	return RegisterDragDrop_Origin(hwnd, pDropTarget);
}

type_SHBrowseForFolderW SHBrowseForFolderW_Origin = nullptr;
PIDLIST_ABSOLUTE STDAPICALLTYPE SHBrowseForFolderW_Mine(_In_ LPBROWSEINFOW lpbi)
{
	INFO_LOG("");
	return SHBrowseForFolderW_Origin(lpbi);
}

type_SHBrowseForFolderA SHBrowseForFolderA_Origin = nullptr;
PIDLIST_ABSOLUTE STDAPICALLTYPE SHBrowseForFolderA_Mine(_In_ LPBROWSEINFOA lpbi)
{
	INFO_LOG("");
	// CoCreateInstance
	return SHBrowseForFolderA_Origin(lpbi);
}

type_CoCreateInstanceEx CoCreateInstanceEx_Origin = nullptr;
HRESULT STDAPICALLTYPE CoCreateInstanceEx_Mine(REFCLSID Clsid, LPUNKNOWN punkOuter, DWORD dwClsCtx, COSERVERINFO* pServerInfo, DWORD dwCount, MULTI_QI* pResults)
{
	auto nRet = CoCreateInstanceEx_Origin(Clsid, punkOuter, dwClsCtx, pServerInfo, dwCount, pResults);
	INFO_LOG("classID:{} {} {}", Clsid.Data1, Clsid.Data2, Clsid.Data3);
	INFO_LOG("refID:{} {} {}", pResults->pIID->Data1, pResults->pIID->Data2, pResults->pIID->Data3);
	return nRet;
}

type_CoCreateInstance CoCreateInstance_Origin = nullptr;
HRESULT STDAPICALLTYPE CoCreateInstance_Mine(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID  FAR* ppv)
{
	if (rclsid == CLSID_FileOpenDialog)
	{
		INFO_LOG("cls ==");
		INFO_LOG("classID:{} {} {}", rclsid.Data1, rclsid.Data2, rclsid.Data3);
		INFO_LOG("refID:{} {} {}", riid.Data1, riid.Data2, riid.Data3);
	}
	if (riid == IID_IFileDialog)
	{
		INFO_LOG("iid ==");
		INFO_LOG("classID:{} {} {}", rclsid.Data1, rclsid.Data2, rclsid.Data3);
		INFO_LOG("refID:{} {} {}", riid.Data1, riid.Data2, riid.Data3);
	}
	return CoCreateInstance_Origin(rclsid, pUnkOuter, dwClsContext, riid, ppv);
}

type_OleGetClipboard OleGetClipboard_Origin = nullptr;

bool EnumFmt(LPDATAOBJECT pDataObj)
{
	INFO_LOG("");
	wchar_t wc[4] = L"Äê¶È³Ô";

	return true;

}

HRESULT STDAPICALLTYPE  OleGetClipboard_Mine(LPDATAOBJECT FAR* ppDataObj)
{
	INFO_LOG("");
	HRESULT nRet = OleGetClipboard_Origin(ppDataObj);
	LPDATAOBJECT pDataObj = *ppDataObj;
	if (!SUCCEEDED(nRet) || pDataObj == NULL)
	{
		ERROR_LOG("OleGetClipboard_Origin failed, err:{}", nRet);
		return nRet;
	}

	uSTGMEDIUM stg = { 0 };
	FORMATETC fmt = { 0 };
	fmt.cfFormat = CF_TEXT;
	fmt.ptd = NULL;
	fmt.dwAspxiaect = DVASPECT_CONTENT;
	fmt.lindex = -1;
	fmt.tymed = TYMED_HGLOBAL;

	nRet = pDataObj->lpVtbl->GetData(pDataObj, &fmt, &stg);
	if (nRet != S_OK)
	{
		ERROR_LOG("GetData failed, err:{}", nRet);
		return nRet;
	}


	INFO_LOG("stg tymed:{}, size:{}", stg.tymed, GlobalSize(stg.lpszFileName));
	

	return nRet;
}

type_IFileOpenDialog_QueryInterface IFileOpenDialog_QueryInterface_Origin = nullptr;
HRESULT STDMETHODCALLTYPE IFileOpenDialog_QueryInterface_Mine(IFileOpenDialog* pFileOpenDialog, REFIID riid, void** ppvObject)
{
	INFO_LOG("");
	if (riid == IID_IFileDialogCustomize)
	{
		INFO_LOG("query IID_IFileDialogCustomize");
	}
	return IFileOpenDialog_QueryInterface_Origin(pFileOpenDialog, riid, ppvObject);
}
type_IFileOpenDialog_Show IFileOpenDialog_Show_Origin = nullptr;
HRESULT STDMETHODCALLTYPE IFileOpenDialog_Show_Mine(IFileOpenDialog* pFileOpenDialog, HWND hwndOwner)
{
	INFO_LOG("");
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
bool hookIFileOpenDialog()
{
	INFO_LOG("");
	IFileOpenDialog* pFileOpenDialog = nullptr;
	// auto res = CoInitializeEx(NULL, COINIT_DISABLE_OLE1DDE);
	LRESULT res = CoInitialize(0); 
	if (res != S_OK && res != S_FALSE)
	{
		ERROR_LOG("CoInitialize failed, res:{}", res);
	}
	res = CoCreateInstance_Origin(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_IFileOpenDialog, (void**)&pFileOpenDialog);
	if (res != S_OK || pFileOpenDialog == nullptr)
	{
		ERROR_LOG("CoCreateInstance failed£¬ res:{}", res);
		return false;
	}
	{
		IFileOpenDialog_Show_Origin = (type_IFileOpenDialog_Show)pFileOpenDialog->lpVtbl->Show;
		if (!DetourTargetProc((void**)&IFileOpenDialog_Show_Origin, IFileOpenDialog_Show_Mine))
		{
			ERROR_LOG("DetourTargetProc failed, IModalWindow_Show");
		}
		apis.push_back(API2HOOK{ "", "", (void**)&IFileOpenDialog_Show_Origin, IFileOpenDialog_Show_Mine });

		IFileOpenDialog_QueryInterface_Origin = (type_IFileOpenDialog_QueryInterface)pFileOpenDialog->lpVtbl->QueryInterface;
		if (!DetourTargetProc((void**)&IFileOpenDialog_QueryInterface_Origin, IFileOpenDialog_QueryInterface_Mine))
		{
			ERROR_LOG("DetourTargetProc failed, IFileOpenDialog_QueryInterface");
		}
		apis.push_back(API2HOOK{ "", "", (void**)&IFileOpenDialog_QueryInterface_Origin, IFileOpenDialog_QueryInterface_Mine });
	}
	pFileOpenDialog->lpVtbl->Release(pFileOpenDialog);
	return true;
}

type_IFileDialog_Show IFileDialog_Show_Origin = nullptr;
HRESULT STDMETHODCALLTYPE IFileDialog_Show_Mine(IFileDialog* pFileDialog, HWND hwndOwner)
{
	INFO_LOG("");

	HRESULT hRet = IFileDialog_Show_Origin(pFileDialog, hwndOwner);
	if (!SUCCEEDED(hRet))
	{
		return hRet;
	}

	IShellItem* pShellItem = nullptr;
	hRet = pFileDialog->lpVtbl->GetResult(pFileDialog, &pShellItem);
	if (hRet != S_OK || pShellItem == nullptr)
	{
		ERROR_LOG("GetResult failed:{}", hRet);

		IFileOpenDialog* pFileOpenDialog = nullptr;
		pFileDialog->lpVtbl->QueryInterface(pFileDialog, IID_IFileOpenDialog, (void**)&pFileOpenDialog);
		if (pFileOpenDialog == nullptr)
		{
			return hRet;
		}

		IShellItemArray* pShellItemArray = nullptr;
		pFileOpenDialog->lpVtbl->GetResults(pFileOpenDialog, &pShellItemArray);
		if (pShellItemArray == nullptr)
		{
			return hRet;
		}

		DWORD cnt = 0;
		pShellItemArray->lpVtbl->GetCount(pShellItemArray, &cnt);
		INFO_LOG("array item num:{}", cnt);
		return hRet;
	}

	LPWSTR displayName = nullptr;
	hRet = pShellItem->lpVtbl->GetDisplayName(pShellItem, SIGDN_FILESYSPATH, &displayName);
	if (SUCCEEDED(hRet) && displayName != nullptr)
	{
		INFO_LOG(L"displayName:{}", displayName);
		CoTaskMemFree(displayName);
	}
	else
	{
		ERROR_LOG("GetDisplayName failed:{}", hRet);
	}

	return HRESULT_FROM_WIN32(ERROR_CANCELLED);
}

bool hookIFileDialog()
{
	INFO_LOG("");
	IFileDialog* pFileDialog = nullptr;
	// auto res = CoInitializeEx(NULL, COINIT_DISABLE_OLE1DDE);
	LRESULT res = CoInitialize(0);
	if (res != S_OK && res != S_FALSE)
	{
		ERROR_LOG("CoInitialize failed, res:{}", res);
	}
	res = CoCreateInstance_Origin(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_IFileDialog, (void**)&pFileDialog);
	if (res != S_OK || pFileDialog == nullptr)
	{
		ERROR_LOG("CoCreateInstance failed£¬ res:{}", res);
		return false;
	}

	{
		IFileDialog_Show_Origin = (type_IFileDialog_Show)pFileDialog->lpVtbl->Show;
		if (!DetourTargetProc((void**)&IFileDialog_Show_Origin, IFileDialog_Show_Mine))
		{
			ERROR_LOG("DetourTargetProc failed, IModalWindow_Show");
		}
		apis.push_back(API2HOOK{ "", "", (void**)&IFileDialog_Show_Origin, IFileDialog_Show_Mine });

	}
	pFileDialog->lpVtbl->Release(pFileDialog);
}

type_IFileOperation_CopyItems IFileOperation_CopyItems_Origin = nullptr;
HRESULT STDMETHODCALLTYPE IFileOperation_CopyItems_Mine(IFileOperation* pFileOperation, IUnknown* punkItems, IShellItem* psiDestinationFolder)
{
	INFO_LOG("");

	return IFileOperation_CopyItems_Origin(pFileOperation, punkItems, psiDestinationFolder);
}
bool hookIFileOperation()
{
	INFO_LOG("");
	bool bRet = false;
	LRESULT res = CoInitialize(0);
	IFileOperation* pFileOperation = nullptr;

	if (res != S_OK && res != S_FALSE)
	{
		ERROR_LOG("CoInitialize failed, res:{}", res);
	}

	res = CoCreateInstance(CLSID_FileOperation, NULL, 
		CLSCTX_REMOTE_SERVER | CLSCTX_LOCAL_SERVER | CLSCTX_INPROC_SERVER, IID_IFileOperation, (void**)&pFileOperation);
	if (res != S_OK || pFileOperation == nullptr)
	{
		ERROR_LOG("CoCreateInstance failed£¬ res:{}", res);
		return false;
	}
	IFileOperation_CopyItems_Origin = pFileOperation->lpVtbl->CopyItems;
	if (!DetourTargetProc((void**)&IFileOperation_CopyItems_Origin, IFileOperation_CopyItems_Mine))
	{
		ERROR_LOG("DetourTargetProc failed, IModalWindow_Show");
	}
	pFileOperation->lpVtbl->Release(pFileOperation);
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
	// FreeLibrary(hMod);
	spdlog::drop_all();
	spdlog::shutdown();
	return true;
}

std::string dllDir = "";
bool hook()
{
	char lpExePath[MAX_PATH] = { 0 };
	DWORD size = MAX_PATH;
	QueryFullProcessImageNameA(GetCurrentProcess(), 0, lpExePath, &size);

	char lpDllPath[MAX_PATH];
	GetModuleFileNameA(gDllModule, lpDllPath, MAX_PATH);

	std::string strExePath = lpExePath;
	std::string strDllPath = lpDllPath;
	auto strExeDir = strExePath.substr(0, strDllPath.find_last_of("\\"));
	auto strDllDir = strDllPath.substr(0, strDllPath.find_last_of("\\"));

	dllDir = strDllDir;
	LOG_INIT("test", strDllDir + "\\log.txt");
	INFO_LOG("dllDir:{}", strDllDir);
	INFO_LOG("exeDir:{}", strExeDir);

	if (strExeDir == strDllDir)
	{
		return false;
	}
	auto th = std::thread(DelayFreeHandle, (HMODULE)NULL);
	th.detach();

	apis.push_back(API2HOOK{ "comdlg32.dll", "GetOpenFileNameW", (void**)&GetOpenFileNameW_Origin, GetOpenFileNameW_Mine });
	apis.push_back(API2HOOK{ "comdlg32.dll", "GetOpenFileNameA", (void**)&GetOpenFileNameA_Origin, GetOpenFileNameA_Mine });
	apis.push_back(API2HOOK{ "Shell32.dll", "DragQueryFileW", (void**)&DragQueryFileW_Origin, DragQueryFileW_Mine });
	apis.push_back(API2HOOK{ "Shell32.dll", "DragQueryFileA", (void**)&DragQueryFileA_Origin, DragQueryFileA_Mine });
	apis.push_back(API2HOOK{ "user32.dll", "GetClipboardData", (void**)&GetClipboardData_Origin, GetClipboardData_Mine });
	apis.push_back(API2HOOK{ "kernel32.dll", "CreateProcessW", (void**)&CreateProcessW_Origin, CreateProcessW_Mine });
	//apis.push_back(API2HOOK{"kernel32.dll", "CreateFileW", (void**)&CreateFileW_Origin, CreateFileW_Mine});
	apis.push_back(API2HOOK{ "kernel32.dll", "CopyFileW", (void**)&CopyFileW_Origin, CopyFileW_Mine });
	//apis.push_back(API2HOOK{ "kernel32.dll", "ReadFile", (void**)&ReadFile_Origin, ReadFile_Mine });
	//apis.push_back(API2HOOK{ "kernel32.dll", "WriteFile", (void**)&WriteFile_Origin, WriteFile_Mine });
	apis.push_back(API2HOOK{ "kernel32.dll", "MoveFileW", (void**)&CopyFileW_Origin, CopyFileW_Mine });
	
	apis.push_back(API2HOOK{ "kernel32.dll", "CreateFileMappingW", (void**)&CreateFileMappingW_Origin, CreateFileMappingW_Mine });

	apis.push_back(API2HOOK{ "Ole32.dll", "RegisterDragDrop", (void**)&RegisterDragDrop_Origin, RegisterDragDrop_Mine });
	apis.push_back(API2HOOK{ "Ole32.dll", "DoDragDrop", (void**)&DoDragDrop_Origin, DoDragDrop_Mine });
	apis.push_back(API2HOOK{ "Ole32.dll", "CoCreateInstance", (void**)&CoCreateInstance_Origin,CoCreateInstance_Mine });
	// apis.push_back(API2HOOK{ "Ole32.dll", "CoCreateInstanceEx", (void**)&CoCreateInstanceEx_Origin, CoCreateInstanceEx_Mine });
	apis.push_back(API2HOOK{ "Ole32.dll", "OleGetClipboard", (void**)&OleGetClipboard_Origin, OleGetClipboard_Mine });

	
	apis.push_back(API2HOOK{ "Shell32.dll", "SHBrowseForFolderW", (void**)&SHBrowseForFolderW_Origin, SHBrowseForFolderW_Mine });
	apis.push_back(API2HOOK{ "Shell32.dll", "SHBrowseForFolderA", (void**)&SHBrowseForFolderA_Origin, SHBrowseForFolderA_Mine });
	
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
	
	hookIFileOperation();
	hookIFileDialog();
	//hookIFileOpenDialog();
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