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

ULONG GetMainThreadId(ULONG processId)
{
	ULONG mainThread = 0;
	UINT64 maxTime = MAXUINT64;
	HANDLE threadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, NULL);
	THREADENTRY32 threadEntry;
	threadEntry.dwSize = sizeof(THREADENTRY32);

	if (!Thread32First(threadSnapshot, &threadEntry))
	{
		return 1;
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

	return mainThread;
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
		// �ж�ϵͳ�ܷ������ָ���ļ��İ汾��Ϣ
		DWORD dwDataSize = GetFileVersionInfoSizeW((LPCWSTR)szModuleName.c_str(), &dwHandle);
		if (dwDataSize == 0)
			break;

		m_lpVersionData = new (std::nothrow) BYTE[dwDataSize];// ���仺����
		if (NULL == m_lpVersionData)
			break;

		// ������Ϣ
		if (!GetFileVersionInfoW((LPCWSTR)szModuleName.c_str(), NULL, dwDataSize, (void*)m_lpVersionData))
			break;

		UINT nQuerySize;
		DWORD* pTransTable;
		// ��������
		if (!VerQueryValueA(m_lpVersionData, "\\VarFileInfo\\Translation", (void**)&pTransTable, &nQuerySize))
			break;

		m_dwLangCharset = MAKELONG(HIWORD(pTransTable[0]), LOWORD(pTransTable[0]));
		if (m_lpVersionData == NULL)
			break;

		tmpstr = new (std::nothrow) CHAR[128];// ���仺����
		if (NULL == tmpstr)
			break;
		
		sprintf_s(tmpstr, 128, "\\StringFileInfo\\%08lx\\%s", m_dwLangCharset, ValueName.c_str());
		
		LPVOID lpData;

		// ���ô˺�����ѯǰ��Ҫ�����ε��ú���GetFileVersionInfoSize��GetFileVersionInfo
		if (!VerQueryValueA((void*)m_lpVersionData, tmpstr, &lpData, &nQuerySize))
		{
			break;
		}
		RetStr = (char*)lpData;
		bSuccess = TRUE;

	} while (0);
	// ���ٻ�����
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
		// �ж�ϵͳ�ܷ������ָ���ļ��İ汾��Ϣ
		DWORD dwDataSize = GetFileVersionInfoSizeA((LPCSTR)szModuleName.c_str(), &dwHandle);
		if (dwDataSize == 0)
			break;

		m_lpVersionData = new (std::nothrow) BYTE[dwDataSize];// ���仺����
		if (NULL == m_lpVersionData)
			break;

		// ������Ϣ
		if (!GetFileVersionInfoA((LPCSTR)szModuleName.c_str(), NULL, dwDataSize, (void*)m_lpVersionData))
			break;

		UINT nQuerySize;
		DWORD* pTransTable;
		// ��������
		if (!VerQueryValueA(m_lpVersionData, "\\VarFileInfo\\Translation", (void**)&pTransTable, &nQuerySize))
			break;

		m_dwLangCharset = MAKELONG(HIWORD(pTransTable[0]), LOWORD(pTransTable[0]));
		if (m_lpVersionData == NULL)
			break;

		tmpstr = new (std::nothrow) CHAR[128];// ���仺����
		if (NULL == tmpstr)
			break;

		sprintf_s(tmpstr, 128, "\\StringFileInfo\\%08lx\\%s", m_dwLangCharset, ValueName.c_str());
		printf("tmpstr:%s\n",tmpstr);
		LPVOID lpData;

		// ���ô˺�����ѯǰ��Ҫ�����ε��ú���GetFileVersionInfoSize��GetFileVersionInfo
		if (::VerQueryValueA((void*)m_lpVersionData, tmpstr, &lpData, &nQuerySize))
			RetStr = (char*)lpData;
		

		bSuccess = TRUE;
	} while (FALSE);

	// ���ٻ�����
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

//��ȡ�ļ�˵��
bool GetFileDescription(const std::string& szModuleName, std::string& RetStr)
{
	return QueryValue("FileDescription", szModuleName, RetStr);
};

//��ȡ�ļ��汾
bool GetFileVersion(const std::string& szModuleName, std::string& RetStr)
{
	return QueryValue("FileVersion", szModuleName, RetStr);
};

//��ȡ��˾����
bool GetCompanyName(const std::string& szModuleName, std::string& RetStr)
{
	return QueryValue("CompanyName", szModuleName, RetStr);
};

//��ȡ��Ʒ����
bool GetProductName(const std::string& szModuleName, std::string& RetStr)
{
	return QueryValue("ProductName", szModuleName, RetStr);
};

//��ȡ��Ʒ�汾
bool GetProductVersion(const std::string& szModuleName, std::string& RetStr)
{
	return QueryValue("ProductVersion", szModuleName, RetStr);
};

//��ȡ��Ȩ
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

#include "oleacc.h"

bool EnumIAccessible(IAccessible* pAcc)
{
	long nCnt, nOutCnt = 0;
	pAcc->get_accChildCount(&nCnt);
	
	VARIANT varFlag = {}; VariantInit(&varFlag); varFlag.vt = VT_I4; 
	LPVARIANT var_arr = nullptr;
	IAccessible* pConChild = nullptr;
	var_arr = (LPVARIANT)malloc(nCnt * sizeof(VARIANT));
	AccessibleChildren(pAcc, 0, nCnt, var_arr, &nOutCnt);
	std::cout << "========childCnt:" << nCnt << ", " << nOutCnt << "============" << std::endl;
	for (int i = 0; i < nOutCnt; i++)
	{
		if (var_arr[i].vt != VT_DISPATCH)
		{
			std::cout << "val_type:" << var_arr[i].vt << std::endl;
			continue;
		}

		var_arr[i].pdispVal->QueryInterface(IID_IAccessible, (void**)&pConChild);

		if (pConChild != nullptr)
		{
			BSTR ptext = NULL;
			pConChild->get_accName(varFlag, &ptext);
			if (ptext != nullptr)
			{
				std::wcout << L"Name:" << ptext << std::endl;
			}
			EnumIAccessible(pConChild);
		}
	}
	return true;
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	DWORD dwProcessId;
	GetWindowThreadProcessId(hWnd, &dwProcessId);

	char clazz1[MAX_PATH] = { 0 };
	GetClassNameA(hWnd, clazz1, MAX_PATH);
	std::string className = "Chrome_WidgetWin_1";

	


	if (dwProcessId == (DWORD)lParam)
	{
		char title[MAX_PATH] = { 0 };
		GetWindowTextA(hWnd, title, MAX_PATH);
		std::cout << (uintptr_t)hWnd << ", title: " << title << std::endl;

		char clazz[MAX_PATH] = { 0 };
		GetClassNameA(hWnd, clazz, MAX_PATH);
		std::cout << (uintptr_t)hWnd << ", clazz: " << clazz << std::endl;

		if (false)
		{
			IAccessible* pAccessible = nullptr;
			LRESULT res = AccessibleObjectFromWindow(hWnd, 0, IID_IAccessible, (void**)&pAccessible);
			if (res != S_OK)
			{
				std::cout << "AccessibleObjectFromWindow failed: " << GetLastError() << std::endl;
			}
			else
			{
				EnumIAccessible(pAccessible);
			}
		}
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
//		// �ж�ϵͳ�ܷ������ָ���ļ��İ汾��Ϣ
//		DWORD dwDataSize = GetFileVersionInfoSizeA((LPCSTR)szModuleName.c_str(), &dwHandle);
//		if (dwDataSize == 0)
//		{
//			CGlobal::OutputA("QueryFileAttrInfo(), GetFileVersionInfoSizeA Failed:%x szModuleName:%s CurTid:%d Line:%d File:%s",
//				GetLastError(), szModuleName.c_str(), GetCurTid(), __LINE__, __FILE__);
//			break;
//		}
//
//
//		m_lpVersionData = new (std::nothrow) BYTE[dwDataSize];// ���仺����
//		if (NULL == m_lpVersionData)
//			break;
//
//		// ������Ϣ
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
//		tmpstr = new (std::nothrow) CHAR[128];// ���仺����
//		if (NULL == tmpstr)
//			break;
//		sprintf_s(tmpstr, 128, "\\StringFileInfo\\%08lx\\%s", m_dwLangCharset, infoName.c_str());
//		LPVOID lpData;
//
//		// ���ô˺�����ѯǰ��Ҫ�����ε��ú���GetFileVersionInfoSize��GetFileVersionInfo
//		if (::VerQueryValueA((void*)m_lpVersionData, tmpstr, &lpData, &nQuerySize))
//			RetStr = (char*)lpData;
//
//		bSuccess = TRUE;
//	} while (FALSE);
//
//	// ���ٻ�����
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
//		// �ж�ϵͳ�ܷ������ָ���ļ��İ汾��Ϣ
//		DWORD dwDataSize = GetFileVersionInfoSizeW((LPCWSTR)szModuleName.c_str(), &dwHandle);
//		if (dwDataSize == 0)
//			break;
//
//		m_lpVersionData = new (std::nothrow) BYTE[dwDataSize];// ���仺����
//		if (NULL == m_lpVersionData)
//			break;
//
//		// ������Ϣ
//		if (!GetFileVersionInfoW((LPCWSTR)szModuleName.c_str(), NULL, dwDataSize, (void*)m_lpVersionData))
//			break;
//
//		UINT nQuerySize;
//		DWORD* pTransTable;
//		// ��������
//		if (!VerQueryValueA(m_lpVersionData, "\\VarFileInfo\\Translation", (void**)&pTransTable, &nQuerySize))
//			break;
//
//		m_dwLangCharset = MAKELONG(HIWORD(pTransTable[0]), LOWORD(pTransTable[0]));
//		if (m_lpVersionData == NULL)
//			break;
//
//		tmpstr = new (std::nothrow) CHAR[128];// ���仺����
//		if (NULL == tmpstr)
//			break;
//
//		sprintf_s(tmpstr, 128, "\\StringFileInfo\\%08lx\\%s", m_dwLangCharset, infoName.c_str());
//		LPVOID lpData;
//		// ���ô˺�����ѯǰ��Ҫ�����ε��ú���GetFileVersionInfoSize��GetFileVersionInfo
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
//	// ���ٻ�����
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



#include "shellapi.h"
#include "psapi.h"

BOOL __stdcall GetWindowHandle(HWND hWnd, LPARAM lParam)
{
	DWORD dwProcessId;
	GetWindowThreadProcessId(hWnd, &dwProcessId);

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, dwProcessId);

	char file_path[MAX_PATH] = { 0 };
	GetProcessImageFileNameA(hProcess, file_path, MAX_PATH);

	std::string strFilePath = file_path;
	if (strFilePath.empty())
	{
		return TRUE;
	}
	std::string strFileName = strFilePath.substr(strFilePath.find_last_of("\\") + 1, strFilePath.length());
	if (strFileName != (char*)lParam)
	{
		return TRUE;
	}

	std::cout << "fileName: " << strFileName << ", ";

	char text[MAX_PATH] = { 0 };
	GetWindowTextA(hWnd, text, MAX_PATH);
	
	char className[MAX_PATH] = { 0 };
	GetClassNameA(hWnd, className, MAX_PATH);

	std::cout << "title: " << text << ", class: " << className << std::endl;

	return TRUE;
}


//int main(int args, char* argv[])
//{
//	if (args < 2)
//	{
//		return 1;
//	}
//
//	char* exe_name = argv[1];
//	std::cout << "exe_name:" << exe_name << std::endl;
//	EnumWindows(GetWindowHandle, (LPARAM)exe_name);
//}

#define EASY_CALL(proc, ...) proc##_1 == 0 ? proc(__VA_ARGS__) : proc##_1(__VA_ARGS__);

int add(int a, int b)
{
	printf("add");
	return a + b;
}

int minus(int a, int b)
{
	printf("minus");
	return a - b;
}

using add_type = int (*)(int, int);

template <typename T>
extern int test_typename(T value);


int main()
{
	test_typename(1);
}