
#include "OsApi.h"

#include "Global.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <userenv.h>
#include <windows.h>
#include <tlhelp32.h>
#include <Aclapi.h>

#include <wincrypt.h>
#pragma comment( lib, "Crypt32.lib" )
#pragma comment(lib, "Userenv.lib")

#include <WtsApi32.h>
#pragma comment( lib, "WtsApi32.lib" )

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

#define STATUS_INFO_LENGTH_MISMATCH      ((NTSTATUS)0xC0000004L)

#define STATUS_UNSUCCESSFUL              ((NTSTATUS)0xC0000001L)

//
// System Information Classes.
//
typedef enum _SYSTEM_INFORMATION_CLASS
{
	SystemBasicInformation,//0
	SystemProcessorInformation,//1
	SystemPerformanceInformation,//2
	SystemTimeOfDayInformation,//3
	SystemPathInformation,//4
	SystemProcessInformation,//5               
	SystemCallCountInformation,//6
	SystemDeviceInformation,//7
	SystemProcessorPerformanceInformation,//8
	SystemFlagsInformation,//9
	SystemCallTimeInformation,//10
	SystemModuleInformation,//11
	SystemLocksInformation,//12
	SystemStackTraceInformation,//13
	SystemPagedPoolInformation,//14
	SystemNonPagedPoolInformation,//15
	SystemHandleInformation,//16
	SystemObjectInformation,//17
	SystemPageFileInformation,//18
	SystemVdmInstemulInformation,//19
	SystemVdmBopInformation,//20
	SystemFileCacheInformation,//21
	SystemPoolTagInformation,//22
	SystemInterruptInformation,//23
	SystemDpcBehaviorInformation,//24
	SystemFullMemoryInformation,//25
	SystemLoadGdiDriverInformation,//26
	SystemUnloadGdiDriverInformation,//27
	SystemTimeAdjustmentInformation,//28
	SystemSummaryMemoryInformation,//29
	SystemMirrorMemoryInformation,//30
	SystemPerformanceTraceInformation,//31
	SystemObsolete0,//32
	SystemExceptionInformation,//33
	SystemCrashDumpStateInformation,//34
	SystemKernelDebuggerInformation,//35
	SystemContextSwitchInformation,//36
	SystemRegistryQuotaInformation,//37
	SystemExtendServiceTableInformation,//38
	SystemPrioritySeperation,//39
	SystemVerifierAddDriverInformation,//40
	SystemVerifierRemoveDriverInformation,//41
	SystemProcessorIdleInformation,//42
	SystemLegacyDriverInformation,//43
	SystemCurrentTimeZoneInformation,//44
	SystemLookasideInformation,//45
	SystemTimeSlipNotification,//46
	SystemSessionCreate,//47
	SystemSessionDetach,//48
	SystemSessionInformation,//49
	SystemRangeStartInformation,//50
	SystemVerifierInformation,//51
	SystemVerifierThunkExtend,//52
	SystemSessionProcessInformation,//53
	SystemLoadGdiDriverInSystemSpace,//54
	SystemNumaProcessorMap,//55
	SystemPrefetcherInformation,//56
	SystemExtendedProcessInformation,//57
	SystemRecommendedSharedDataAlignment,//58
	SystemComPlusPackage,//59
	SystemNumaAvailableMemory,//60
	SystemProcessorPowerInformation,//61
	SystemEmulationBasicInformation,//62
	SystemEmulationProcessorInformation,//63
	SystemExtendedHandleInformation,//64
	SystemLostDelayedWriteInformation,//65
	SystemBigPoolInformation,//66
	SystemSessionPoolTagInformation,//67
	SystemSessionMappedViewInformation,//68
	SystemHotpatchInformation,//69
	SystemObjectSecurityMode,//70
	SystemWatchdogTimerHandler,//71
	SystemWatchdogTimerInformation,//72
	SystemLogicalProcessorInformation,//73
	SystemWow64SharedInformation,//74
	SystemRegisterFirmwareTableInformationHandler,//75
	SystemFirmwareTableInformation,//76
	SystemModuleInformationEx,//77
	SystemVerifierTriageInformation,//78
	SystemSuperfetchInformation,//79
	SystemMemoryListInformation,//80
	SystemFileCacheInformationEx,//81
	MaxSystemInfoClass//82  
} SYSTEM_INFORMATION_CLASS;

typedef struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING;

typedef UNICODE_STRING* PUNICODE_STRING;
typedef const UNICODE_STRING* PCUNICODE_STRING;

typedef LONG KPRIORITY;

typedef struct _CLIENT_ID
{
	HANDLE UniqueProcess;
	HANDLE UniqueThread;
} CLIENT_ID;

typedef struct _SYSTEM_THREAD_INFORMATION
{
	LARGE_INTEGER Reserved1[3];
	ULONG Reserved2;
	PVOID StartAddress;
	CLIENT_ID ClientId;
	KPRIORITY Priority;
	LONG BasePriority;
	ULONG Reserved3;
	ULONG ThreadState;
	ULONG WaitReason;
} SYSTEM_THREAD_INFORMATION, * PSYSTEM_THREAD_INFORMATION;

typedef struct _SYSTEM_PROCESS_INFORMATION
{
	ULONG NextEntryOffset;
	ULONG NumberOfThreads;
	LARGE_INTEGER WorkingSetPrivateSize; // since VISTA
	ULONG HardFaultCount; // since WIN7
	ULONG NumberOfThreadsHighWatermark; // since WIN7
	ULONGLONG CycleTime; // since WIN7
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	UNICODE_STRING ImageName;
	KPRIORITY BasePriority;
	HANDLE UniqueProcessId;
	HANDLE InheritedFromUniqueProcessId;
	ULONG HandleCount;
	ULONG SessionId;
	ULONG_PTR UniqueProcessKey; // since VISTA (requires SystemExtendedProcessInformation)
	SIZE_T PeakVirtualSize;
	SIZE_T VirtualSize;
	ULONG PageFaultCount;
	SIZE_T PeakWorkingSetSize;
	SIZE_T WorkingSetSize;
	SIZE_T QuotaPeakPagedPoolUsage;
	SIZE_T QuotaPagedPoolUsage;
	SIZE_T QuotaPeakNonPagedPoolUsage;
	SIZE_T QuotaNonPagedPoolUsage;
	SIZE_T PagefileUsage;
	SIZE_T PeakPagefileUsage;
	SIZE_T PrivatePageCount;
	LARGE_INTEGER ReadOperationCount;
	LARGE_INTEGER WriteOperationCount;
	LARGE_INTEGER OtherOperationCount;
	LARGE_INTEGER ReadTransferCount;
	LARGE_INTEGER WriteTransferCount;
	LARGE_INTEGER OtherTransferCount;
	SYSTEM_THREAD_INFORMATION Threads[1];
} SYSTEM_PROCESS_INFORMATION, * PSYSTEM_PROCESS_INFORMATION;


/**
 * Gets a pointer to the first process information structure in a buffer returned by
 * PhEnumProcesses().
 *
 * \param Processes A pointer to a buffer returned by PhEnumProcesses().
 */
#define PH_FIRST_PROCESS(Processes) ((PSYSTEM_PROCESS_INFORMATION)(Processes))

 /**
  * Gets a pointer to the process information structure after a given structure.
  *
  * \param Process A pointer to a process information structure.
  *
  * \return A pointer to the next process information structure, or NULL if there are no more.
  */
#define PH_NEXT_PROCESS(Process) ( \
    ((PSYSTEM_PROCESS_INFORMATION)(Process))->NextEntryOffset ? \
    (PSYSTEM_PROCESS_INFORMATION)((PCHAR)(Process) + \
    ((PSYSTEM_PROCESS_INFORMATION)(Process))->NextEntryOffset) : \
    NULL \
    )


class NamedEventImpl
{
public:
	NamedEventImpl(bool bAutoReset)
	{
		_event = CreateEventW(NULL, bAutoReset ? FALSE : TRUE, FALSE, NULL);
		if (!_event)
		{
			throw boost::format("cannot create event! bAutoReset:%d") % bAutoReset;
		}

		_name = "";
		_uname = L"";
	}

	NamedEventImpl(const std::string& name)
	{
		//UnicodeConverter::toUTF16(_name, _uname);

		//_uname = boost::locale::conv::to_utf<wchar_t>(_name, "gbk");

		// 无法打开文件“libboost_locale-vc142-mt-s-x64-1_77.lib”
		// 暂用s2ws代替
		_uname = s2ws(name);

		_event = CreateEventW(NULL, FALSE, FALSE, _uname.c_str());
		if (!_event)
		{
			DWORD dwRetVal = GetLastError();
			throw boost::format("cannot create named event! Error:%d _name:%s") % dwRetVal % _name;
		}
	}

	~NamedEventImpl()
	{
		if (NULL != _event)
		{
			CloseHandle(_event);
			_event = NULL;
		}
	}

	void setImpl()
	{
		if (!SetEvent(_event))
		{
			throw boost::format("cannot signal named event! _name:%s") % _name;
		}
	}

	void resetImpl()
	{
		if (!ResetEvent(_event))
		{
			throw boost::format("cannot reset named event! _name:%s") % _name;
		}
	}

	void waitImpl()
	{
		switch (WaitForSingleObject(_event, INFINITE))
		{
		case WAIT_OBJECT_0:
			return;
		default:
			throw boost::format("wait for named event failed! _name:%s") % _name;
		}
	}

	bool waitImpl(long milliseconds)
	{
		switch (WaitForSingleObject(_event, milliseconds + 1))
		{
		case WAIT_TIMEOUT:
			return false;
		case WAIT_OBJECT_0:
			return true;
		default:
			throw boost::format("wait for named event failed! _name:%s") % _name;
		}
	}

private:
	std::string  _name;
	std::wstring _uname;
	HANDLE      _event;
};

NamedEvent::NamedEvent(bool bAutoReset)
{
	m_pImpl = new NamedEventImpl(bAutoReset);
}

NamedEvent::NamedEvent()
{
	m_pImpl = new NamedEventImpl(true);
}

NamedEvent::NamedEvent(const std::string& name)
{
	m_pImpl = new NamedEventImpl(name);
}

NamedEvent::~NamedEvent()
{
	if (NULL != m_pImpl)
	{
		delete ((NamedEventImpl*)m_pImpl);
		m_pImpl = NULL;
	}
}

void NamedEvent::set()
{
	if (NULL != m_pImpl)
	{
		((NamedEventImpl*)m_pImpl)->setImpl();
	}
}

void NamedEvent::reset()
{
	if (NULL != m_pImpl)
	{
		((NamedEventImpl*)m_pImpl)->resetImpl();
	}
}

void NamedEvent::wait()
{
	if (NULL != m_pImpl)
	{
		((NamedEventImpl*)m_pImpl)->waitImpl();
	}
}

void NamedEvent::wait(long milliseconds)
{
	if (NULL != m_pImpl)
	{
		if (!((NamedEventImpl*)m_pImpl)->waitImpl(milliseconds))
		{
			throw boost::format("NamedEvent::wait() failed! milliseconds:%d") % milliseconds;
		}
	}
}

bool NamedEvent::tryWait(long milliseconds)
{
	if (NULL != m_pImpl)
	{
		return ((NamedEventImpl*)m_pImpl)->waitImpl(milliseconds);
	}

	return false;
}


void OutputStringA(IN const char* pStr)
{
	::OutputDebugStringA(pStr);
}

void OutputStringW(IN const wchar_t* pStr)
{
	::OutputDebugStringW(pStr);
}

typedef NTSTATUS(WINAPI* _NtQuerySystemInformation)(
	IN          SYSTEM_INFORMATION_CLASS SystemInformationClass,
	IN OUT      PVOID SystemInformation,
	IN          ULONG SystemInformationLength,
	OUT			PULONG ReturnLength);

bool GetProcessInfoEx(IN int nPid, OUT __int64& nWorkingSetPrivateSizeBytes, OUT __int64& nWorkingSetSizeBytes)
{
	bool bRet = false;
	ULONG ulReturnLength;
	int nNumberOfBytes = 0x4000;
	PVOID pProcessInformation = NULL;
	NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
	PSYSTEM_PROCESS_INFORMATION pProcessInfo;
	_NtQuerySystemInformation pNtQuerySystemInformation = (_NtQuerySystemInformation)GetProcAddress(GetModuleHandleW(L"Ntdll.dll"), "NtQuerySystemInformation");

	if (NULL == pNtQuerySystemInformation || 0 >= nPid)
	{
		CGlobal::OutputA("GetProcessInfoEx(), param error! pNtQuerySystemInformation:%p nPid:%d Line:%d File:%s \r\n",
			pNtQuerySystemInformation, nPid, __LINE__, __FILE__);
		return bRet;
	}

	do
	{
		if (NULL != pProcessInformation)
		{
			delete[] pProcessInformation;
			pProcessInformation = NULL;
		}

		pProcessInformation = new BYTE[nNumberOfBytes];
		if (NULL == pProcessInformation)
		{
			CGlobal::OutputA("GetProcessInfoEx(), new BYTE[%d] error! Line:%d File:%s \r\n",
				nNumberOfBytes, __LINE__, __FILE__);
			break;
		}
		ZeroMemory(pProcessInformation, nNumberOfBytes);

		ntStatus = pNtQuerySystemInformation(SystemProcessInformation, pProcessInformation, nNumberOfBytes, &ulReturnLength);
		if (!NT_SUCCESS(ntStatus) && (STATUS_INFO_LENGTH_MISMATCH != ntStatus))
		{
			CGlobal::OutputA("GetProcessInfoEx(), pNtQuerySystemInformation() failed! ntStatus:%#x Line:%d File:%s \r\n",
				(ULONG)ntStatus, __LINE__, __FILE__);
			break;
		}

		nNumberOfBytes += nNumberOfBytes;

	} while (!NT_SUCCESS(ntStatus));

	if (NT_SUCCESS(ntStatus))
	{
		pProcessInfo = PH_FIRST_PROCESS(pProcessInformation);

		do
		{
			if (nPid == (int)pProcessInfo->UniqueProcessId)
			{
				nWorkingSetPrivateSizeBytes = (__int64)pProcessInfo->WorkingSetPrivateSize.QuadPart;
				nWorkingSetSizeBytes = (__int64)pProcessInfo->WorkingSetSize;
				bRet = true;
				break;
			}

			pProcessInfo = PH_NEXT_PROCESS(pProcessInfo);

		} while (NULL != pProcessInfo);
	}

	if (NULL != pProcessInformation)
	{
		delete[] pProcessInformation;
	}

	return bRet;
}

std::wstring GetCurProcessPathUnicode()
{
	std::wstring strRet = L"";

	WCHAR szPath[MAX_PATH + 4] = { 0 };
	DWORD dwRet = ::GetModuleFileNameW(NULL, szPath, MAX_PATH);
	DWORD dwErr = ::GetLastError();
	if (0 != dwRet && ERROR_INSUFFICIENT_BUFFER != dwErr)
	{
		strRet = szPath;
	}

	return strRet;
}

std::wstring GetModulePathByAddr(int nPid, void* pAddr)
{
	DWORD dwErr;
	MODULEENTRY32W pe32;
	std::wstring strRetW = L"";
	HANDLE hProcessSnap = INVALID_HANDLE_VALUE;

	do
	{
		if (0 >= nPid || NULL == pAddr)
		{
			CGlobal::OutputA("GetModulePathByAddr(), param error! nPid:%d pAddr:%p Line:%d File:%s \r\n",
				nPid, pAddr, __LINE__, __FILE__);
			break;
		}

		hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, nPid);
		if (hProcessSnap == INVALID_HANDLE_VALUE)
		{
			dwErr = ::GetLastError();
			CGlobal::OutputA("GetModulePathByAddr(), CreateToolhelp32Snapshot() failed! nPid:%d dwErr:%d Line:%d File:%s \r\n",
				nPid, dwErr, __LINE__, __FILE__);
			break;
		}

		pe32.dwSize = sizeof(MODULEENTRY32);
		if (!::Module32FirstW(hProcessSnap, &pe32))
		{
			dwErr = ::GetLastError();
			CGlobal::OutputA("GetModulePathByAddr(), Module32First() failed! nPid:%d dwErr:%d Line:%d File:%s \r\n",
				nPid, dwErr, __LINE__, __FILE__);
			break;
		}

		do
		{
			if (pe32.modBaseAddr <= pAddr && pAddr <= pe32.modBaseAddr + pe32.modBaseSize)
			{
				strRetW = pe32.szExePath;
				break;
			}

		} while (::Module32NextW(hProcessSnap, &pe32));

	} while (0);

	if (INVALID_HANDLE_VALUE != hProcessSnap)
	{
		::CloseHandle(hProcessSnap);
	}

	return strRetW;
}

std::wstring GetCurModulePathUnicode()
{
	return GetModulePathByAddr(::GetCurrentProcessId(), GetCurModulePathUnicode);
}

bool Is64bitSystem()
{
	SYSTEM_INFO si;
	::GetNativeSystemInfo(&si);
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
	{
		return true;
	}
	else
	{
		return false;
	}

	//// 32位程序在32位系统下，Environment::osArchitecture()返回 "IA32"
	//// 32位程序在32位系统下，Is64bitSystem()返回 false
	//
	//// 32位程序在64位系统下，Environment::osArchitecture()返回 "IA32"
	//// 32位程序在64位系统下，Is64bitSystem()返回 true
	//
	//// 64位程序在64位系统下，Environment::osArchitecture()返回 "AMD64"
	//// 64位程序在64位系统下，Is64bitSystem()返回 true
}

//bool GetParentPid(IN int nPid, OUT int& nParPid)
//{
//	bool bRet = false;
//	PROCESSENTRY32 pe32;
//	HANDLE hProcessSnap = INVALID_HANDLE_VALUE;
//
//	do
//	{
//		if (0 >= nPid)
//		{
//			CGlobal::OutputA("GetParentPid(), param error! nPid:%d Line:%d File:%s \r\n",
//				nPid, __LINE__, __FILE__);
//			break;
//		}
//
//		hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
//		if (INVALID_HANDLE_VALUE == hProcessSnap)
//		{
//			DWORD dwErr = ::GetLastError();
//			CGlobal::OutputA("GetParentPid(), CreateToolhelp32Snapshot() failed! nPid:%d dwErr:%d Line:%d File:%s \r\n",
//				nPid, dwErr, __LINE__, __FILE__);
//			break;
//		}
//
//		pe32.dwSize = sizeof(PROCESSENTRY32);
//		if (!::Process32First(hProcessSnap, &pe32))
//		{
//			DWORD dwErr = ::GetLastError();
//			CGlobal::OutputA("GetParentPid(), Process32First() failed! nPid:%d dwErr:%d Line:%d File:%s \r\n",
//				nPid, dwErr, __LINE__, __FILE__);
//			break;
//		}
//
//		do
//		{
//			if (nPid == (int)pe32.th32ProcessID)
//			{
//				nParPid = pe32.th32ParentProcessID;
//				bRet = true;
//				break;
//			}
//
//		} while (::Process32Next(hProcessSnap, &pe32));
//
//	} while (0);
//
//	if (INVALID_HANDLE_VALUE != hProcessSnap)
//	{
//		::CloseHandle(hProcessSnap);
//	}
//
//	return bRet;
//}

void FindProcessByName(IN const std::wstring& strName, OUT std::list<int>& listPid)
{
	PROCESSENTRY32 pe32;
	HANDLE hProcessSnap = INVALID_HANDLE_VALUE;

	listPid.clear();

	do
	{
		if (strName.empty())
		{
			CGlobal::OutputA("FindProcessByName(), param error! \r\n");
			break;
		}

		hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE == hProcessSnap)
		{
			DWORD dwErr = ::GetLastError();
			CGlobal::OutputW(L"FindProcessByName(), CreateToolhelp32Snapshot() failed! strName:%s dwErr:%d \r\n",
				strName.c_str(), dwErr);
			break;
		}

		pe32.dwSize = sizeof(PROCESSENTRY32);
		if (!::Process32First(hProcessSnap, &pe32))
		{
			DWORD dwErr = ::GetLastError();
			CGlobal::OutputW(L"FindProcessByName(), Process32First() failed! strName:%s dwErr:%d \r\n",
				strName.c_str(), dwErr);
			break;
		}

		do
		{
			if (0 == _wcsicmp(strName.c_str(), pe32.szExeFile))
			{
				listPid.push_back(pe32.th32ProcessID);
			}

		} while (::Process32Next(hProcessSnap, &pe32));

	} while (0);

	if (INVALID_HANDLE_VALUE != hProcessSnap)
	{
		::CloseHandle(hProcessSnap);
	}
}

void GetAllProcessInSystem(OUT std::list<unsigned long>& listPid)
{
	PROCESSENTRY32 pe32;
	HANDLE hProcessSnap = INVALID_HANDLE_VALUE;

	listPid.clear();

	do
	{
		hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE == hProcessSnap)
		{
			DWORD dwErr = ::GetLastError();
			CGlobal::OutputW(L"GetAllProcessInSystem(), CreateToolhelp32Snapshot() failed! dwErr:%d \r\n", dwErr);
			break;
		}

		pe32.dwSize = sizeof(PROCESSENTRY32);
		if (!::Process32First(hProcessSnap, &pe32))
		{
			DWORD dwErr = ::GetLastError();
			CGlobal::OutputW(L"GetAllProcessInSystem(), Process32First() failed! dwErr:%d \r\n", dwErr);
			break;
		}

		do
		{
			listPid.push_back(pe32.th32ProcessID);

		} while (::Process32Next(hProcessSnap, &pe32));

	} while (0);

	if (INVALID_HANDLE_VALUE != hProcessSnap)
	{
		::CloseHandle(hProcessSnap);
	}
}

void KillAllDescendantProcess(IN int nPid)
{
	PROCESSENTRY32 pe32;
	std::list<int> listPid;
	std::map<int, int> mapPidToParPid;
	HANDLE hProcessSnap = INVALID_HANDLE_VALUE;

	if (0 >= nPid)
	{
		return;
	}

	do
	{
		hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE == hProcessSnap)
		{
			DWORD dwErr = ::GetLastError();
			CGlobal::OutputW(L"KillAllDescendantProcess(), CreateToolhelp32Snapshot() failed! nPid:%d dwErr:%d \r\n", nPid, dwErr);
			break;
		}

		pe32.dwSize = sizeof(PROCESSENTRY32);
		if (!::Process32First(hProcessSnap, &pe32))
		{
			DWORD dwErr = ::GetLastError();
			CGlobal::OutputW(L"KillAllDescendantProcess(), Process32First() failed! nPid:%d dwErr:%d \r\n", nPid, dwErr);
			break;
		}

		do
		{
			listPid.push_back(pe32.th32ProcessID);
			mapPidToParPid[pe32.th32ProcessID] = pe32.th32ParentProcessID;

		} while (::Process32Next(hProcessSnap, &pe32));

	} while (0);

	if (INVALID_HANDLE_VALUE != hProcessSnap)
	{
		::CloseHandle(hProcessSnap);
	}

	for (std::list<int>::iterator it = listPid.begin(); it != listPid.end(); it++)
	{
		int nTemp = *it;
		while (true)
		{
			std::map<int, int>::iterator the = mapPidToParPid.find(nTemp);
			if (the != mapPidToParPid.end())
			{
				if (the->second == nPid)
				{
					HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, *it);
					if (NULL != hProcess)
					{
						if (!::TerminateProcess(hProcess, 0))
						{
							DWORD dwErr = ::GetLastError();
							CGlobal::OutputA("KillAllDescendantProcess(), TerminateProcess() failed! nPid:%d dwErr:%d Line:%d File:%s \r\n",
								*it, dwErr, __LINE__, __FILE__);
						}

						::CloseHandle(hProcess);
					}
					else
					{
						DWORD dwErr = ::GetLastError();
						CGlobal::OutputA("KillAllDescendantProcess(), OpenProcess() failed! nPid:%d dwErr:%d Line:%d File:%s \r\n",
							*it, dwErr, __LINE__, __FILE__);
					}

					break;
				}
				else
				{
					nTemp = the->second; // 从父进程开始查找
					if (0 >= nTemp)
					{
						break;
					}
				}
			}
			else
			{
				break;
			}
		}
	}
}

//bool DeleteDir(IN std::wstring& strDirPathUnicode)
//{
//	bool bRet = false;
//
//	if (strDirPathUnicode.empty())
//	{
//		return bRet;
//	}
//
//	strDirPathUnicode.erase(strDirPathUnicode.find_last_not_of(L"\\") + 1); // 去掉尾部的所有"\\"
//	std::wstring strTemp, strDirFind = strDirPathUnicode + L"\\*.*";
//
//	WIN32_FIND_DATAW struFind;
//	HANDLE hFileFind = ::FindFirstFileW(strDirFind.c_str(), &struFind);
//	if (INVALID_HANDLE_VALUE == hFileFind)
//	{
//		DWORD dwErr = ::GetLastError();
//		CGlobal::OutputW(L"DeleteDir(), FindFirstFileW() failed! dwErr:%d strDirFind:%s \r\n", dwErr, strDirFind.c_str());
//		return bRet;
//	}
//
//	do
//	{
//		if (struFind.cFileName[0] == L'.')
//		{
//
//		}
//		else if (struFind.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
//		{
//			strTemp = strDirPathUnicode + L"\\" + struFind.cFileName;
//			DeleteDir(strTemp);
//		}
//		else
//		{
//			strTemp = strDirPathUnicode + L"\\" + struFind.cFileName;
//			if (!::DeleteFile(strTemp.c_str()))
//			{
//				DWORD dwErr = ::GetLastError();
//				CGlobal::OutputW(L"DeleteDir(), DeleteFile() failed! dwErr:%d strFilePath:%s \r\n", dwErr, strTemp.c_str());
//			}
//
//		}
//	} while (::FindNextFileW(hFileFind, &struFind));
//
//	::FindClose(hFileFind);
//
//	if (!::RemoveDirectoryW(strDirPathUnicode.c_str()))
//	{
//		DWORD dwErr = ::GetLastError();
//		CGlobal::OutputW(L"DeleteDir(), RemoveDirectoryW() failed! dwErr:%d strDirPathUnicode:%s \r\n", dwErr, strDirPathUnicode.c_str());
//	}
//	else
//	{
//		bRet = true;
//	}	
//
//	return bRet;
//}

bool GetProcessPathUnicode(IN int nPid, OUT std::wstring& strPath)
{
	bool bRet = false;
	HANDLE hProcess = NULL;
	DWORD dwSize = MAX_PATH;
	WCHAR szPath[MAX_PATH + 4] = { 0 };

	do
	{
		if (0 >= nPid)
		{
			CGlobal::OutputA("GetProcessPathUnicode(), param error! nPid:%d Line:%d File:%s \r\n",
				nPid, __LINE__, __FILE__);
			break;
		}

		hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, nPid);
		if (NULL == hProcess)
		{
			DWORD dwErr = ::GetLastError();
			CGlobal::OutputA("GetProcessPathUnicode(), OpenProcess() failed! nPid:%d dwErr:%d Line:%d File:%s \r\n",
				nPid, dwErr, __LINE__, __FILE__);
			break;
		}

		if (!::QueryFullProcessImageNameW(hProcess, 0, szPath, &dwSize))
		{
			DWORD dwErr = ::GetLastError();
			CGlobal::OutputA("GetProcessPathUnicode(), QueryFullProcessImageNameW() failed! nPid:%d dwErr:%d Line:%d File:%s \r\n",
				nPid, dwErr, __LINE__, __FILE__);
			break;
		}

		strPath = szPath;
		bRet = true;

	} while (0);

	if (NULL != hProcess)
	{
		::CloseHandle(hProcess);
	}

	return bRet;
}

BOOL AdjustPrivileges(IN LPCTSTR szPrivilege)
{
	BOOL bRet = FALSE;
	HANDLE hToken = NULL;
	TOKEN_PRIVILEGES tkp;
	LUID sedebugnameValue;

	do
	{
		if (NULL == szPrivilege)
		{
			break;
		}

		if (!::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		{
			DWORD dwErr = ::GetLastError();
			CGlobal::OutputA("AdjustPrivileges(), OpenProcessToken() failed! dwErr:%d Line:%d File:%s \r\n",
				dwErr, __LINE__, __FILE__);
			break;
		}

		if (!::LookupPrivilegeValue(NULL, szPrivilege, &sedebugnameValue))
		{
			DWORD dwErr = ::GetLastError();
			CGlobal::OutputA("AdjustPrivileges(), LookupPrivilegeValue() failed! dwErr:%d Line:%d File:%s \r\n",
				dwErr, __LINE__, __FILE__);
			break;
		}

		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Luid = sedebugnameValue;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (!::AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
		{
			DWORD dwErr = ::GetLastError();
			CGlobal::OutputA("AdjustPrivileges(), AdjustTokenPrivileges() failed! dwErr:%d Line:%d File:%s \r\n",
				dwErr, __LINE__, __FILE__);
			break;
		}

		bRet = TRUE;

	} while (0);

	if (NULL != hToken)
	{
		::CloseHandle(hToken);
	}

	return bRet;
}

bool EnableDebugPriv()
{
	if (!AdjustPrivileges(L"SeDebugPrivilege")) // 提权
	{
		CGlobal::OutputA("EnableDebugPriv(), AdjustPrivileges() failed! Line:%d File:%s", __LINE__, __FILE__);
		return false;
	}

	return true;
}

std::wstring GetFileSignerDispName(IN const std::wstring& strPathUnicode)
{
	DWORD           dwEncoding;
	HCERTSTORE      hCertStoreHandle = NULL;
	HCRYPTMSG       hAuthCryptMsg = NULL;
	std::wstring	strDispName = L"";
	PCMSG_SIGNER_INFO pSignerInfo = NULL;
	PCCERT_CONTEXT  pCurrContext = NULL;
	WCHAR* lpszSubjectName = NULL;

	do
	{
		if (!CryptQueryObject(
			CERT_QUERY_OBJECT_FILE,
			strPathUnicode.c_str(),
			CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED,
			CERT_QUERY_FORMAT_FLAG_BINARY,
			0,
			&dwEncoding,
			NULL,
			NULL,
			&hCertStoreHandle,
			&hAuthCryptMsg,
			NULL))
		{
			DWORD dwErr = GetLastError();
			CGlobal::OutputW(L"GetFileSignerDispName(), CryptQueryObject() failed! dwErr:%d %#x strPathUnicode:%s", dwErr, dwErr, strPathUnicode.c_str());
			break;
		}

		dwEncoding = 0;
		CryptMsgGetParam(hAuthCryptMsg, CMSG_SIGNER_INFO_PARAM, 0, NULL, &dwEncoding);
		if (0 >= dwEncoding)
		{
			DWORD dwErr = GetLastError();
			CGlobal::OutputW(L"GetFileSignerDispName(), CryptMsgGetParam() failed! dwErr:%d %#x strPathUnicode:%s", dwErr, dwErr, strPathUnicode.c_str());
			break;
		}

		pSignerInfo = (PCMSG_SIGNER_INFO)LocalAlloc(LPTR, dwEncoding);
		if (NULL == pSignerInfo)
		{
			DWORD dwErr = GetLastError();
			CGlobal::OutputW(L"GetFileSignerDispName(), LocalAlloc() failed! dwErr:%d %#x strPathUnicode:%s", dwErr, dwErr, strPathUnicode.c_str());
			break;
		}

		if (!CryptMsgGetParam(hAuthCryptMsg, CMSG_SIGNER_INFO_PARAM, 0, pSignerInfo, &dwEncoding))
		{
			DWORD dwErr = GetLastError();
			CGlobal::OutputW(L"GetFileSignerDispName(), CryptMsgGetParam() failed! dwErr:%d %#x strPathUnicode:%s", dwErr, dwErr, strPathUnicode.c_str());
			break;
		}

		pCurrContext = CertFindCertificateInStore(hCertStoreHandle,
			X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
			0,
			CERT_FIND_ISSUER_NAME,
			(PVOID)&pSignerInfo->Issuer,
			NULL);
		if (NULL == pCurrContext)
		{
			DWORD dwErr = GetLastError();
			CGlobal::OutputW(L"GetFileSignerDispName(), CertFindCertificateInStore() failed! dwErr:%d %#x strPathUnicode:%s", dwErr, dwErr, strPathUnicode.c_str());
			break;
		}

		dwEncoding = CertGetNameStringW(pCurrContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, NULL, NULL, 0);
		if (0 >= dwEncoding)
		{
			DWORD dwErr = GetLastError();
			CGlobal::OutputW(L"GetFileSignerDispName(), CertGetNameStringW() failed! dwErr:%d %#x strPathUnicode:%s", dwErr, dwErr, strPathUnicode.c_str());
			break;
		}

		lpszSubjectName = (LPTSTR)LocalAlloc(LPTR, dwEncoding * sizeof(WCHAR));
		if (NULL == lpszSubjectName)
		{
			DWORD dwErr = GetLastError();
			CGlobal::OutputW(L"GetFileSignerDispName(), LocalAlloc() failed! dwErr:%d %#x strPathUnicode:%s", dwErr, dwErr, strPathUnicode.c_str());
			break;
		}

		if (0 >= CertGetNameStringW(pCurrContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, NULL, lpszSubjectName, dwEncoding))
		{
			DWORD dwErr = GetLastError();
			CGlobal::OutputW(L"GetFileSignerDispName(), CertGetNameStringW() failed! dwErr:%d %#x strPathUnicode:%s", dwErr, dwErr, strPathUnicode.c_str());
			break;
		}

		strDispName = lpszSubjectName;

	} while (FALSE);

	if (NULL != lpszSubjectName)
	{
		LocalFree(lpszSubjectName);
	}

	if (NULL != pSignerInfo)
	{
		LocalFree(pSignerInfo);
	}

	if (NULL != pCurrContext)
	{
		CertFreeCertificateContext(pCurrContext);
	}

	if (NULL != hAuthCryptMsg)
	{
		CryptMsgClose(hAuthCryptMsg);
	}

	if (NULL != hCertStoreHandle)
	{
		CertCloseStore(hCertStoreHandle, 0);
	}

	return strDispName;
}

//bool GetFileAttributeTime(IN const std::wstring& strFilePathUnicode,
//	OUT std::string& strCreateTime,
//	OUT std::string& strModifyTime,
//	OUT std::string& strAccessTime)
//{
//	bool bRet = false;
//	HANDLE hFile = INVALID_HANDLE_VALUE;
//	FILETIME ftCreate, ftModify, ftAccess;	
//
//	do
//	{
//		hFile = ::CreateFileW(
//			strFilePathUnicode.c_str(),
//			GENERIC_READ,
//			FILE_SHARE_READ,
//			NULL,
//			OPEN_EXISTING,
//			FILE_ATTRIBUTE_NORMAL,
//			NULL);
//		if (INVALID_HANDLE_VALUE == hFile)
//		{
//			DWORD dwErr = GetLastError();
//			CGlobal::OutputW(L"GetFileAttributeTime(), CreateFileW() failed! dwErr:%d %#x strFilePathUnicode:%s", 
//				dwErr, dwErr, strFilePathUnicode.c_str());
//			break;
//		}
//
//		if (!::GetFileTime(hFile, &ftCreate, &ftAccess, &ftModify))
//		{
//			DWORD dwErr = GetLastError();
//			CGlobal::OutputW(L"GetFileAttributeTime(), GetFileTime() failed! dwErr:%d %#x strFilePathUnicode:%s",
//				dwErr, dwErr, strFilePathUnicode.c_str());
//			break;
//		}
//
//		{
//			char szBuf[64] = { 0 };
//			FILETIME ftLocal = { 0 };
//			SYSTEMTIME stTime = { 0 };			
//
//			::FileTimeToLocalFileTime(&ftCreate, &ftLocal);
//			::FileTimeToSystemTime(&ftLocal, &stTime);
//
//			::sprintf_s(szBuf, "%04d/%02d/%02d %02d:%02d:%02d", stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);
//
//			strCreateTime = szBuf;
//		}
//
//		{
//			char szBuf[64] = { 0 };
//			FILETIME ftLocal = { 0 };
//			SYSTEMTIME stTime = { 0 };
//
//			::FileTimeToLocalFileTime(&ftModify, &ftLocal);
//			::FileTimeToSystemTime(&ftLocal, &stTime);
//
//			::sprintf_s(szBuf, "%04d/%02d/%02d %02d:%02d:%02d", stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);
//
//			strModifyTime = szBuf;
//		}
//
//		{
//			char szBuf[64] = { 0 };
//			FILETIME ftLocal = { 0 };
//			SYSTEMTIME stTime = { 0 };
//
//			::FileTimeToLocalFileTime(&ftAccess, &ftLocal);
//			::FileTimeToSystemTime(&ftLocal, &stTime);
//
//			::sprintf_s(szBuf, "%04d/%02d/%02d %02d:%02d:%02d", stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);
//
//			strAccessTime = szBuf;
//		}
//
//		bRet = true;
//
//	} while (0);
//
//	if (INVALID_HANDLE_VALUE != hFile)
//	{
//		::CloseHandle(hFile);
//	}
//
//	return bRet;
//}

bool SetNamedObjectSecurity(IN const std::string& strName)
{
	if (strName.empty())
	{
		return false;
	}

	DWORD dwRet = ::SetNamedSecurityInfoA((char*)strName.c_str(),
		SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, 0, 0, (PACL)NULL, NULL);
	if (ERROR_SUCCESS != dwRet)
	{
		CGlobal::OutputA("In SetNamedObjectSecurity(), SetNamedSecurityInfoA(%s) failed! dwRet:%d %#x",
			strName.c_str(), dwRet, dwRet);
	}

	return (ERROR_SUCCESS == dwRet);
}

bool CreateUniqueInstance(IN const std::wstring& strInstanceNameW, OUT void** ppContext)
{
	bool bRet = false;

	do
	{
		if (strInstanceNameW.empty() || NULL == ppContext)
		{
			CGlobal::OutputW(L"In CreateUniqueInstance(), param error! strInstanceNameW:%s ppContext:%p",
				strInstanceNameW.c_str(), ppContext);
			break;
		}

		{
			HANDLE hMutex = ::CreateMutexW(NULL, TRUE, strInstanceNameW.c_str());
			if (NULL != hMutex)
			{
				if (ERROR_ALREADY_EXISTS == ::GetLastError()) // 已经存在
				{
					::CloseHandle(hMutex);
				}
				else
				{
					*ppContext = hMutex;
					bRet = true;
				}
			}
			else
			{
				DWORD dwErr = ::GetLastError();
				CGlobal::OutputW(L"In CreateUniqueInstance(), CreateMutexW() failed! dwErr:%d %#x strInstanceNameW:%s",
					dwErr, dwErr, strInstanceNameW.c_str());
			}
		}

	} while (0);

	return bRet;
}

void ReleaseUniqueInstance(IN void* pContext)
{
	if (NULL != pContext)
	{
		::CloseHandle((HANDLE)pContext);
	}
}

void GetAllProcessInDir(IN const std::string& strDir, OUT std::vector<int>& vecPid)
{
	PROCESSENTRY32 pe32;
	HANDLE hProcessSnap = INVALID_HANDLE_VALUE;

	std::wstring strDctPcsPathUnicode = L"";
	std::string strDctPcsPathUTF8 = "";

	vecPid.clear();
	do
	{
		hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE == hProcessSnap)
		{
			DWORD dwErr = ::GetLastError();
			CGlobal::OutputA("GetAllProcessInDir(), CreateToolhelp32Snapshot() failed! dwErr:%d \r\n",
				dwErr);
			break;
		}

		pe32.dwSize = sizeof(PROCESSENTRY32);
		if (!::Process32First(hProcessSnap, &pe32))
		{
			DWORD dwErr = ::GetLastError();
			CGlobal::OutputA("GetAllProcessInDir(), Process32First() failed! dwErr:%d \r\n",
				dwErr);
			break;
		}

		//Poco::UInt32 nProcessId = Process::id();

		//do
		//{
		//	if (nProcessId == pe32.th32ProcessID || GetProcessPathUnicode(pe32.th32ProcessID, strDctPcsPathUnicode) == false)
		//	{
		//		continue;
		//	}
		//	try
		//	{
		//		Poco::UnicodeConverter::toUTF8(strDctPcsPathUnicode, strDctPcsPathUTF8);
		//		Path clsPath = strDctPcsPathUTF8;

		//		if (strDctPcsPathUTF8.find(strDir, 0) == 0)
		//		{
		//			vecPid.push_back(pe32.th32ProcessID);

		//			CGlobal::OutputA("GetAllProcessInDir() ok, pid:%d path:{%s}", pe32.th32ProcessID, strDctPcsPathUTF8.c_str());
		//		}
		//	}
		//	catch (Poco::Exception exc)
		//	{
		//		CGlobal::OutputA("GetAllProcessInDir() failed, pid:%d exception:{%s} path:{%s}", pe32.th32ProcessID, exc.displayText().c_str(), strDctPcsPathUTF8.c_str());
		//	}

		//} while (::Process32Next(hProcessSnap, &pe32));

	} while (0);

	if (INVALID_HANDLE_VALUE != hProcessSnap)
	{
		::CloseHandle(hProcessSnap);
	}
}

bool GetAllDiskDriveName(OUT std::vector<std::string>& vecDiskDriveName)
{
	vecDiskDriveName.clear();

	unsigned int uType = 0;
	char szDrive[3] = { 0,':',0 };
	for (char c = 'A'; c <= 'Z'; ++c)
	{
		szDrive[0] = c;
		uType = GetDriveTypeA(szDrive);

		if (uType == DRIVE_FIXED)
		{
			vecDiskDriveName.push_back(szDrive);
		}
	}

	return vecDiskDriveName.size() > 0;
}

//bool ValidFileSign(IN const std::string& strPathUtf8)
//{
//	bool bRet = false;
//	std::wstring strPathUnicode, strDispName = L"";
//
//	do
//	{
//		if (strPathUtf8.empty())
//		{
//			CGlobal::OutputA("ValidFileSign(), strPathUtf8.empty()!");
//			break;
//		}
//
//		Poco::UnicodeConverter::toUTF16(strPathUtf8, strPathUnicode); // 需要将utf8编码转为Unicode编码，微软的函数才能正常调用
//		strDispName = GetFileSignerDispName(strPathUnicode);
//		if (strDispName.empty())
//		{
//			CGlobal::OutputW(L"ValidFileSign(), strDispName.empty()! strPathUnicode:%s", strPathUnicode.c_str());
//			break;
//		}
//
//		bRet = (0 == strDispName.compare(L"上海派拉软件股份有限公司")); // 字符串比较，大小写敏感
//
//	} while (0);
//
//	return bRet;
//}

bool GetCurrentUserSessionId(OUT unsigned long& nSessionId)
{
	bool bRet = false;
	DWORD i, nCount = 0;
	PWTS_SESSION_INFOW pSessInfo = NULL;

	if (::WTSEnumerateSessionsW(WTS_CURRENT_SERVER_HANDLE, 0, 1, &pSessInfo, &nCount))
	{
		for (i = 0; i < nCount; i++)
		{
			DWORD dwSize = 0;
			PWCHAR pLogName = NULL;
			if (::WTSQuerySessionInformationW(WTS_CURRENT_SERVER_HANDLE,
				pSessInfo[i].SessionId,
				WTSUserName,
				&pLogName,
				&dwSize))
			{
				if (0 < dwSize && 0 < wcslen(pLogName))
				{
					CGlobal::OutputW(L"GetCurrentUserSessionId(), valid pLogName:%s SessionId:%d \r\n",
						pLogName, pSessInfo[i].SessionId);

					nSessionId = pSessInfo[i].SessionId;
					bRet = true;
					break;
				}

				CGlobal::OutputW(L"GetCurrentUserSessionId(), pLogName:%s SessionId:%d \r\n",
					pLogName, pSessInfo[i].SessionId);

				::WTSFreeMemory(pLogName);
			}
			else
			{
				DWORD dwErr = ::GetLastError();
				CGlobal::OutputW(L"GetCurrentUserSessionId(), WTSQuerySessionInformationW() failed! SessionId:%d dwErr:%d \r\n",
					pSessInfo[i].SessionId, dwErr);
			}
		}

		::WTSFreeMemory(pSessInfo);
	}
	else
	{
		DWORD dwErr = ::GetLastError();
		CGlobal::OutputW(L"GetCurrentUserSessionId(), WTSEnumerateSessionsW() failed! dwErr:%d \r\n", dwErr);
	}

	return bRet;
}

bool OpenProcessTokenEx(IN DWORD nPid, OUT HANDLE& hProcessToken)
{
	bool bRet = false;

	HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, nPid);
	if (NULL != hProcess)
	{
		if (::OpenProcessToken(hProcess, TOKEN_ALL_ACCESS, &hProcessToken))
		{
			bRet = true;
		}
		else
		{
			DWORD dwErr = ::GetLastError();
			CGlobal::OutputW(L"OpenProcessTokenEx(), OpenProcessToken() failed! nPid:%d dwErr:%d \r\n",
				nPid, dwErr);
		}

		::CloseHandle(hProcess);
	}
	else
	{
		DWORD dwErr = ::GetLastError();
		CGlobal::OutputW(L"OpenProcessTokenEx(), OpenProcess() failed! nPid:%d dwErr:%d \r\n",
			nPid, dwErr);
	}

	return bRet;
}

HANDLE GetProcessTokenHandle(IN unsigned long nSessionId, IN PCWCH pExeName)
{
	HANDLE hRet = NULL;
	PROCESSENTRY32W pe32;
	DWORD dwProcessSession;
	HANDLE hProcessSnap = INVALID_HANDLE_VALUE;

	do
	{
		if (NULL == pExeName || 0 >= wcslen(pExeName))
		{
			CGlobal::OutputW(L"GetProcessTokenHandle(), invalid pExeName! nSessionId:%d \r\n", nSessionId);
			break;
		}

		// Take a snapshot of all processes in the system.
		hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hProcessSnap == INVALID_HANDLE_VALUE)
		{
			DWORD dwErr = ::GetLastError();
			CGlobal::OutputW(L"GetProcessTokenHandle(), CreateToolhelp32Snapshot() failed! nSessionId:%d dwErr:%d \r\n",
				nSessionId, dwErr);
			break;
		}

		// Set the size of the structure before using it.
		pe32.dwSize = sizeof(PROCESSENTRY32W);

		// Retrieve information about the first process,
		// and exit if unsuccessful
		if (!::Process32FirstW(hProcessSnap, &pe32))
		{
			DWORD dwErr = ::GetLastError();
			CGlobal::OutputW(L"GetProcessTokenHandle(), Process32FirstW() failed! nSessionId:%d dwErr:%d \r\n",
				nSessionId, dwErr);
			break;
		}

		// Now walk the snapshot of processes, and
		// display information about each process in turn
		do
		{
			if (::ProcessIdToSessionId(pe32.th32ProcessID, &dwProcessSession))
			{
				if (dwProcessSession == nSessionId)
				{
					if (0 == _wcsicmp(pExeName, pe32.szExeFile))
					{
						if (!OpenProcessTokenEx(pe32.th32ProcessID, hRet))
						{
							CGlobal::OutputW(L"GetProcessTokenHandle(), OpenProcessTokenEx() failed! nSessionId:%d th32ProcessID:%d pExeName:%s \r\n",
								nSessionId, pe32.th32ProcessID, pExeName);
						}

						break;
					}
				}
			}

		} while (::Process32Next(hProcessSnap, &pe32));

	} while (0);

	if (INVALID_HANDLE_VALUE != hProcessSnap)
	{
		::CloseHandle(hProcessSnap);
	}

	return hRet;
}

bool GetTokenBySessionId(IN unsigned long nSessionId, OUT HANDLE& hProcessToken)
{
	if (!::WTSQueryUserToken(nSessionId, &hProcessToken))
	{
		hProcessToken = GetProcessTokenHandle(nSessionId, L"EXPLORER.EXE");
	}

	return (NULL != hProcessToken);
}

bool GetUserToken(IN unsigned long nSessionId, IN HANDLE hProcessToken, OUT HANDLE& hUserToken)
{
	DWORD nRetLen;
	bool bRet = false;
	TOKEN_PRIVILEGES tp;
	TOKEN_LINKED_TOKEN struToken = { 0 };

	hUserToken = NULL;

	do
	{
		if (::GetTokenInformation(hProcessToken, TokenLinkedToken, &struToken, sizeof(TOKEN_LINKED_TOKEN), &nRetLen))
		{
			hUserToken = struToken.LinkedToken;
		}
		else
		{
			if (!::DuplicateTokenEx(hProcessToken, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &hUserToken))
			{
				DWORD dwErr = ::GetLastError();
				CGlobal::OutputW(L"GetUserToken(), DuplicateTokenEx() failed! nSessionId:%d dwErr:%d \r\n",
					nSessionId, dwErr);
				break;
			}

			// 为hUserToken增加SessionId数据
			if (!::SetTokenInformation(hUserToken, TokenSessionId, (void*)&nSessionId, sizeof(DWORD)))
			{
				DWORD dwErr = ::GetLastError();
				if (dwErr != ERROR_PRIVILEGE_NOT_HELD) //当客户端进程调用本模块的时候会返回ERROR_PRIVILEGE_NOT_HELD(1314)错误号，忽略此错误码
				{
					CGlobal::OutputW(L"GetUserToken(), SetTokenInformation() failed! nSessionId:%d dwErr:%d \r\n",
						nSessionId, dwErr);
					break;
				}
			}

			if (!::LookupPrivilegeValueW(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid))
			{
				DWORD dwErr = ::GetLastError();
				CGlobal::OutputW(L"GetUserToken(), LookupPrivilegeValueW(SE_DEBUG_NAME) failed! nSessionId:%d dwErr:%d \r\n",
					nSessionId, dwErr);
				break;
			}

			tp.PrivilegeCount = 1;
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			if (!::AdjustTokenPrivileges(hUserToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL)) // 为hUserToken启用指定权限
			{
				DWORD dwErr = ::GetLastError();
				CGlobal::OutputW(L"GetUserToken(), AdjustTokenPrivileges() failed! nSessionId:%d dwErr:%d \r\n",
					nSessionId, dwErr);
				break;
			}
		}

		bRet = true;

	} while (0);

	if (!bRet && NULL != hUserToken)
	{
		::CloseHandle(hUserToken);
		hUserToken = NULL;
	}

	return bRet;
}

bool CreateProcessAsSessionId(IN const std::wstring& strCmdline, IN unsigned long nSessionId, OUT unsigned long& nNewProcessId)
{
	bool bRet = false;
	LPVOID pEnv = NULL;
	HANDLE hUserToken = NULL;
	HANDLE hProcessToken = NULL;
	STARTUPINFOW si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	DWORD dwCreationFlags = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE;

	si.cb = sizeof(si);
	si.lpDesktop = (LPWSTR)L"Winsta0\\Default";

	do
	{
		if (strCmdline.empty())
		{
			CGlobal::OutputW(L"CreateProcessAsSessionId(), strCmdline.empty() failed! nSessionId:%d \r\n", nSessionId);
			break;
		}

		if (!GetTokenBySessionId(nSessionId, hProcessToken))
		{
			CGlobal::OutputW(L"CreateProcessAsSessionId(), GetTokenBySessionId() failed! nSessionId:%d \r\n", nSessionId);
			break;
		}

		if (!GetUserToken(nSessionId, hProcessToken, hUserToken))
		{
			CGlobal::OutputW(L"CreateProcessAsSessionId(), GetUserToken() failed! nSessionId:%d \r\n", nSessionId);
			break;
		}

		// Impersonate client to ensure access to executable file.
		if (!::ImpersonateLoggedOnUser(hUserToken))
		{
			CGlobal::OutputW(L"CreateProcessAsSessionId(), ImpersonateLoggedOnUser() failed! nSessionId:%d \r\n", nSessionId);
			break;
		}

		if (::CreateEnvironmentBlock(&pEnv, hUserToken, TRUE))
		{
			dwCreationFlags |= CREATE_UNICODE_ENVIRONMENT;
		}
		else
		{
			pEnv = NULL;
		}

		if (!::CreateProcessAsUserW(
			hUserToken,					// client's access token
			NULL,						// file to execute
			(LPWSTR)strCmdline.c_str(),	// command line
			NULL,						// pointer to process SECURITY_ATTRIBUTES
			NULL,						// pointer to thread SECURITY_ATTRIBUTES
			FALSE,						// handles are not inheritable
			dwCreationFlags,			// creation flags 
			pEnv,						// pointer to new environment block 
			NULL,						// name of current directory 
			&si,						// pointer to STARTUPINFO structure
			&pi))						// receives information about new process		
		{
			DWORD dwErr = ::GetLastError();
			CGlobal::OutputW(L"CreateProcessAsSessionId(), CreateProcessAsUserW() failed! nSessionId:%d dwErr:%d strCmdline:%s \r\n",
				nSessionId, dwErr, strCmdline.c_str());
			break;
		}

		nNewProcessId = pi.dwProcessId;

		if (NULL != pi.hProcess)
		{
			::CloseHandle(pi.hProcess);
		}

		if (NULL != pi.hThread)
		{
			::CloseHandle(pi.hThread);
		}

		bRet = true;

	} while (0);

	if (NULL != pEnv)
	{
		::DestroyEnvironmentBlock(pEnv);
	}

	if (NULL != hUserToken)
	{
		::CloseHandle(hUserToken);
	}

	if (NULL != hProcessToken)
	{
		::CloseHandle(hProcessToken);
	}

	return bRet;
}

bool CreateProcessAsCurrentUser(IN const std::string& strCmdlineUtf8, OUT unsigned long& nNewProcessId)
{
	bool bRet = false;
	std::wstring strCmdline;
	unsigned long nSessionId;

	do
	{
		if (strCmdlineUtf8.empty())
		{
			break;
		}

		strCmdline = UTF8ToUnicode(strCmdlineUtf8); // 需要将utf8编码转为Unicode编码，微软的函数才能正常调用
		if (strCmdline.empty())
		{
			break;
		}

		if (!GetCurrentUserSessionId(nSessionId))
		{
			break;
		}

		bRet = CreateProcessAsSessionId(strCmdline, nSessionId, nNewProcessId);

	} while (0);

	return bRet;
}