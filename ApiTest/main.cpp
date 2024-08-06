#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <wchar.h>
#include <time.h>
#include <chrono>
#include <string>
#include <map>
#include <shlobj_core.h>
#pragma comment(lib, "version.lib")

VOID GetMainThreadId(ULONG processId)
{
	ULONG mainThread = 0;
	UINT64 maxTime = MAXUINT64;
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

			if (*(PUINT64)&createTime && *(PUINT64)&createTime < maxTime)
			{
				mainThread = threadEntry.th32ThreadID;
				maxTime = *(PUINT64)&createTime;
			}

            std::cout << "createTime:" << createTime.dwHighDateTime << createTime.dwLowDateTime << std::endl;
        }
    } while (Thread32Next(threadSnapshot, &threadEntry));
	std::cout << "mainThreadId:" << mainThread << std::endl;
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

bool QueryValueW(const std::string& ValueName, const std::wstring& szModuleName, std::string& RetStr)
{
	bool bSuccess = FALSE;
	BYTE* m_lpVersionData = NULL;
	DWORD   m_dwLangCharset = 0;
	CHAR* tmpstr = NULL;
	do
	{
		if (!ValueName.size() || !szModuleName.size())
			break;

		DWORD dwHandle;
		// 判断系统能否检索到指定文件的版本信息
		DWORD dwDataSize = GetFileVersionInfoSizeW((LPCWSTR)szModuleName.c_str(), &dwHandle);
		if (dwDataSize == 0)
			break;

		m_lpVersionData = new (std::nothrow) BYTE[dwDataSize];// 分配缓冲区
		if (NULL == m_lpVersionData)
			break;

		// 检索信息
		if (!GetFileVersionInfoW((LPCWSTR)szModuleName.c_str(), NULL, dwDataSize, (void*)m_lpVersionData))
			break;

		UINT nQuerySize;
		DWORD* pTransTable;
		// 设置语言
		if (!VerQueryValueA(m_lpVersionData, "\\VarFileInfo\\Translation", (void**)&pTransTable, &nQuerySize))
			break;

		m_dwLangCharset = MAKELONG(HIWORD(pTransTable[0]), LOWORD(pTransTable[0]));
		if (m_lpVersionData == NULL)
			break;

		tmpstr = new (std::nothrow) CHAR[128];// 分配缓冲区
		if (NULL == tmpstr)
			break;
		
		sprintf_s(tmpstr, 128, "\\StringFileInfo\\%08lx\\%s", m_dwLangCharset, ValueName.c_str());
		
		LPVOID lpData;

		// 调用此函数查询前需要先依次调用函数GetFileVersionInfoSize和GetFileVersionInfo
		if (!VerQueryValueA((void*)m_lpVersionData, tmpstr, &lpData, &nQuerySize))
		{
			break;
		}
		RetStr = (char*)lpData;
		bSuccess = TRUE;

	} while (0);
	// 销毁缓冲区
	if (m_lpVersionData)
	{
		delete[] m_lpVersionData;
		m_lpVersionData = NULL;
	}
	if (tmpstr)
	{
		delete[] tmpstr;
		tmpstr = NULL;
	}
	return true;
}

bool QueryValue(const std::string& ValueName, const std::string& szModuleName, std::string& RetStr)
{
	bool bSuccess = FALSE;
	BYTE* m_lpVersionData = NULL;
	DWORD   m_dwLangCharset = 0;
	CHAR* tmpstr = NULL;

	do
	{
		if (!ValueName.size() || !szModuleName.size())
			break;

		DWORD dwHandle;
		// 判断系统能否检索到指定文件的版本信息
		DWORD dwDataSize = GetFileVersionInfoSizeA((LPCSTR)szModuleName.c_str(), &dwHandle);
		if (dwDataSize == 0)
			break;

		m_lpVersionData = new (std::nothrow) BYTE[dwDataSize];// 分配缓冲区
		if (NULL == m_lpVersionData)
			break;

		// 检索信息
		if (!GetFileVersionInfoA((LPCSTR)szModuleName.c_str(), NULL, dwDataSize, (void*)m_lpVersionData))
			break;

		UINT nQuerySize;
		DWORD* pTransTable;
		// 设置语言
		if (!VerQueryValueA(m_lpVersionData, "\\VarFileInfo\\Translation", (void**)&pTransTable, &nQuerySize))
			break;

		m_dwLangCharset = MAKELONG(HIWORD(pTransTable[0]), LOWORD(pTransTable[0]));
		if (m_lpVersionData == NULL)
			break;

		tmpstr = new (std::nothrow) CHAR[128];// 分配缓冲区
		if (NULL == tmpstr)
			break;

		sprintf_s(tmpstr, 128, "\\StringFileInfo\\%08lx\\%s", m_dwLangCharset, ValueName.c_str());
		printf("tmpstr:%s\n",tmpstr);
		LPVOID lpData;

		// 调用此函数查询前需要先依次调用函数GetFileVersionInfoSize和GetFileVersionInfo
		if (::VerQueryValueA((void*)m_lpVersionData, tmpstr, &lpData, &nQuerySize))
			RetStr = (char*)lpData;
		

		bSuccess = TRUE;
	} while (FALSE);

	// 销毁缓冲区
	if (m_lpVersionData)
	{
		delete[] m_lpVersionData;
		m_lpVersionData = NULL;
	}
	if (tmpstr)
	{
		delete[] tmpstr;
		tmpstr = NULL;
	}

	return bSuccess;
}

//获取文件说明
bool GetFileDescription(const std::string& szModuleName, std::string& RetStr)
{
	return QueryValue("FileDescription", szModuleName, RetStr);
};

//获取文件版本
bool GetFileVersion(const std::string& szModuleName, std::string& RetStr)
{
	return QueryValue("FileVersion", szModuleName, RetStr);
};

//获取公司名称
bool GetCompanyName(const std::string& szModuleName, std::string& RetStr)
{
	return QueryValue("CompanyName", szModuleName, RetStr);
};

//获取产品名称
bool GetProductName(const std::string& szModuleName, std::string& RetStr)
{
	return QueryValue("ProductName", szModuleName, RetStr);
};

//获取产品版本
bool GetProductVersion(const std::string& szModuleName, std::string& RetStr)
{
	return QueryValue("ProductVersion", szModuleName, RetStr);
};

//获取版权
bool GetLegalCopyright(const std::string& szModuleName, std::string& RetStr)
{
	return QueryValue("LegalCopyright", szModuleName, RetStr);
};


bool GetCurFileName(std::string& app)
{
	bool bRet = false;

	char* path_buffer = NULL;
	int path_length = 260;
	int max_try = 5;

	for (int i = 0; i < max_try; i++)
	{
		path_buffer = new char[path_length];
		if (path_buffer == NULL)
		{
			break;
		}

		DWORD ret_size = GetModuleFileNameA(NULL, path_buffer, path_length);
		if (ret_size == 0)
		{
			// log
			break;
		}

		if (ret_size >= path_length)
		{
			//log
			delete[] path_buffer;
			path_buffer = NULL;
			path_length = path_length * 2;
			continue;
		}

		bRet = true;
		break;
	}

	if (bRet)
	{
		std::string path = path_buffer;
		std::cout << path;
		int index = path.find_last_of('\\', path.length());
		if (index <= 0 || index >= path.length())
		{
			bRet = false;
		}
		else
		{
			app = path.substr(index + 1, path.length());
		}
	}

	if (path_buffer != NULL)
	{
		delete[] path_buffer;
	}
	return bRet;
}

typedef struct INFO_ {
	unsigned long dwProcessId;

}INFO, *PINFO;

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	DWORD dwProcessId;
	GetWindowThreadProcessId(hWnd, &dwProcessId);
	PINFO pInfo = (PINFO)lParam;
	if (dwProcessId == pInfo->dwProcessId)
	{
		std::cout << "In Target Process" << std::endl;
		CHAR szClassName[128];
		CHAR szCaption[128];
		GetClassNameA(hWnd, szClassName, 128);
		GetWindowTextA(hWnd, szCaption, 128);
		std::cout << "ClassName:" << (char*)szClassName << ", Caption:" << (char*)szCaption << std::endl;
		/*if (NULL != pInfo->lpClassName && _tcslen(pInfo->lpClassName) > 0)
		{
			TCHAR szClassName[128] = _T("");
			GetClassName(hWnd, szClassName, 128);
			if (0 != _tcscmp(szClassName, pInfo->lpClassName))
				return TRUE;
		}

		if (NULL != pInfo->lpCaption && _tcslen(pInfo->lpCaption) > 0)
		{
			TCHAR szCaption[128] = _T("");
			GetWindowText(hWnd, szCaption, 128);
			if (0 != _tcscmp(szCaption, pInfo->lpCaption))
				return TRUE;
		}*/
		// return FALSE;
	}
	return TRUE;
}

HWND GetMainWndFromProcessID(DWORD dwProcessId, LPTSTR  lpCaption /*= NULL*/, LPTSTR  lpClassName /*= NULL*/)
{
	INFO info = { 0 };
	info.dwProcessId = dwProcessId;

	EnumWindows(EnumWindowsProc, (LPARAM)&info);

	return NULL;
}

//bool QueryFileAttrInfo(const std::string& infoName, const std::string& szModuleName, std::string& RetStr)
//{
//	bool bSuccess = FALSE;
//	BYTE* m_lpVersionData = NULL;
//	DWORD m_dwLangCharset = 0;
//	CHAR* tmpstr = NULL;
//
//	do
//	{
//		if (!infoName.size() || !szModuleName.size())
//		{
//			CGlobal::OutputA("QueryFileAttrInfo(), Invalid Params. infoName:%s szModuleName:%s CurTid:%d Line:%d File:%s",
//				infoName.c_str(), szModuleName.c_str(), GetCurTid(), __LINE__, __FILE__);
//			break;
//		}
//
//
//		DWORD dwHandle;
//		// 判断系统能否检索到指定文件的版本信息
//		DWORD dwDataSize = GetFileVersionInfoSizeA((LPCSTR)szModuleName.c_str(), &dwHandle);
//		if (dwDataSize == 0)
//		{
//			CGlobal::OutputA("QueryFileAttrInfo(), GetFileVersionInfoSizeA Failed:%x szModuleName:%s CurTid:%d Line:%d File:%s",
//				GetLastError(), szModuleName.c_str(), GetCurTid(), __LINE__, __FILE__);
//			break;
//		}
//
//
//		m_lpVersionData = new (std::nothrow) BYTE[dwDataSize];// 分配缓冲区
//		if (NULL == m_lpVersionData)
//			break;
//
//		// 检索信息
//		if (!GetFileVersionInfoA((LPCSTR)szModuleName.c_str(), NULL, dwDataSize, (void*)m_lpVersionData))
//		{
//			CGlobal::OutputA("QueryFileAttrInfo(), GetFileVersionInfoA Failed:%x szModuleName:%s CurTid:%d Line:%d File:%s",
//				GetLastError(), szModuleName.c_str(), GetCurTid(), __LINE__, __FILE__);
//			break;
//		}
//
//		UINT nQuerySize;
//		DWORD* pTransTable;
//
//		if (!VerQueryValueA(m_lpVersionData, "\\VarFileInfo\\Translation", (void**)&pTransTable, &nQuerySize))
//		{
//			CGlobal::OutputA("QueryFileAttrInfo(), VerQueryValueA Failed : % x szModuleName : % s CurTid : % d Line : % d File : % s",
//				GetLastError(), szModuleName.c_str(), GetCurTid(), __LINE__, __FILE__);
//			break;
//		}
//
//		m_dwLangCharset = MAKELONG(HIWORD(pTransTable[0]), LOWORD(pTransTable[0]));
//		if (m_lpVersionData == NULL)
//			break;
//
//		tmpstr = new (std::nothrow) CHAR[128];// 分配缓冲区
//		if (NULL == tmpstr)
//			break;
//		sprintf_s(tmpstr, 128, "\\StringFileInfo\\%08lx\\%s", m_dwLangCharset, infoName.c_str());
//		LPVOID lpData;
//
//		// 调用此函数查询前需要先依次调用函数GetFileVersionInfoSize和GetFileVersionInfo
//		if (::VerQueryValueA((void*)m_lpVersionData, tmpstr, &lpData, &nQuerySize))
//			RetStr = (char*)lpData;
//
//		bSuccess = TRUE;
//	} while (FALSE);
//
//	// 销毁缓冲区
//	if (m_lpVersionData)
//	{
//		delete[] m_lpVersionData;
//		m_lpVersionData = NULL;
//	}
//	if (tmpstr)
//	{
//		delete[] tmpstr;
//		tmpstr = NULL;
//	}
//
//	return bSuccess;
//}
//
//bool QueryFileAttrInfoW(const std::string& infoName, const std::wstring& szModuleName, std::string& RetStr)
//{
//	bool bSuccess = FALSE;
//	BYTE* m_lpVersionData = NULL;
//	DWORD   m_dwLangCharset = 0;
//	CHAR* tmpstr = NULL;
//	do
//	{
//		if (!infoName.size() || !szModuleName.size())
//			break;
//
//		DWORD dwHandle;
//		// 判断系统能否检索到指定文件的版本信息
//		DWORD dwDataSize = GetFileVersionInfoSizeW((LPCWSTR)szModuleName.c_str(), &dwHandle);
//		if (dwDataSize == 0)
//			break;
//
//		m_lpVersionData = new (std::nothrow) BYTE[dwDataSize];// 分配缓冲区
//		if (NULL == m_lpVersionData)
//			break;
//
//		// 检索信息
//		if (!GetFileVersionInfoW((LPCWSTR)szModuleName.c_str(), NULL, dwDataSize, (void*)m_lpVersionData))
//			break;
//
//		UINT nQuerySize;
//		DWORD* pTransTable;
//		// 设置语言
//		if (!VerQueryValueA(m_lpVersionData, "\\VarFileInfo\\Translation", (void**)&pTransTable, &nQuerySize))
//			break;
//
//		m_dwLangCharset = MAKELONG(HIWORD(pTransTable[0]), LOWORD(pTransTable[0]));
//		if (m_lpVersionData == NULL)
//			break;
//
//		tmpstr = new (std::nothrow) CHAR[128];// 分配缓冲区
//		if (NULL == tmpstr)
//			break;
//
//		sprintf_s(tmpstr, 128, "\\StringFileInfo\\%08lx\\%s", m_dwLangCharset, infoName.c_str());
//		LPVOID lpData;
//		// 调用此函数查询前需要先依次调用函数GetFileVersionInfoSize和GetFileVersionInfo
//		if (!VerQueryValueA((void*)m_lpVersionData, tmpstr, &lpData, &nQuerySize))
//		{
//			break;
//		}
//		RetStr = (char*)lpData;
//
//		bSuccess = TRUE;
//
//	} while (0);
//
//	// 销毁缓冲区
//	if (m_lpVersionData)
//	{
//		delete[] m_lpVersionData;
//		m_lpVersionData = NULL;
//	}
//	if (tmpstr)
//	{
//		delete[] tmpstr;
//		tmpstr = NULL;
//	}
//	return bSuccess;
//}

#include <fstream>
bool WriteFileData(IN const std::string& strFilePathUtf8, IN const std::string& strData)
{
	bool bRet = false;

	if (strFilePathUtf8.empty())
	{
		return bRet;
	}

	try
	{
		std::ofstream ofs(strFilePathUtf8, std::ios_base::binary);

		ofs << strData;
		ofs.close();

		bRet = true;
	}
	catch (...)
	{
	}

	return bRet;
}

#include "UIAutomationClient.h"
int main()
{
	GetMainThreadId(15976);
}