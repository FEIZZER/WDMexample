
#pragma once

// OsApi.h : header file
//

#include "AppHeader.h"

// 事件类：用于数据同步、进程阻塞/进程退出等场景
class NamedEvent
{
public:
	NamedEvent(bool bAutoReset);
	/// Creates a unnamed event object.
	/// bAutoReset=false：a manual event object.Need use reset function to set the event state to nonsignaled.
	///	bAutoReset=true：a auto reset event object.

	NamedEvent();
	/// Creates a unnamed auto reset event object.

	NamedEvent(const std::string& name);
	/// Creates or opens a named event object.

	~NamedEvent();
	/// Destroys the event.

	void set();
	/// Signals the event.
	/// The one thread or process waiting for the event
	/// can resume execution.

	void reset();
	/// Resets the event to unsignalled state.

	void wait();
	/// Waits for the event to become signalled.

	void wait(long milliseconds);
	/// Waits for the event to become signalled.
	/// Throws a TimeoutException if the event
	/// does not become signalled within the specified
	/// time interval.

	bool tryWait(long milliseconds);
	/// Waits for the event to become signalled.
	/// Returns true if the event
	/// became signalled within the specified
	/// time interval, false otherwise.

private:

	void* m_pImpl;
};

// 输出调试信息
void OutputStringA(IN const char* pStr);
void OutputStringW(IN const wchar_t* pStr);

/**************************************************************/
// 获取进程信息
// 参数:
//		nPid[IN]-进程id
//		nWorkingSetPrivateSizeBytes[OUT]-进程私有工作集，单位：字节
//		nWorkingSetSizeBytes[OUT]-进程工作集，单位：字节
// 返回值: 
//		true-成功  false-失败
/**************************************************************/
bool GetProcessInfoEx(IN int nPid, OUT long long& nWorkingSetPrivateSizeBytes, OUT long long& nWorkingSetSizeBytes);

// 返回当前进程的路径（Unicode格式）
std::wstring GetCurProcessPathUnicode();

// 返回当前模块的路径（Unicode格式）
std::wstring GetCurModulePathUnicode();

/**************************************************************/
// 当前系统是否为64位
// 参数:
// 返回值: 
//		true-是64位  false-不是64位
/**************************************************************/
bool Is64bitSystem();

/**************************************************************/
// 获取进程的父进程id
// 参数:
//		nPid[IN]-进程id
//		nParPid[OUT]-保存父进程id
// 返回值: 
//		true-成功  false-失败
/**************************************************************/
//bool GetParentPid(IN int nPid, OUT int& nParPid);

/**************************************************************/
// 通过进程名找到进程。
// 参数:
//		strName[IN]-进程名
//		listPid[OUT]-保存进程id
// 返回值: 
/**************************************************************/
void FindProcessByName(IN const std::wstring& strName, OUT std::list<int>& listPid);

/**************************************************************/
// 获取系统中的所有进程。
// 参数:
//		listPid[OUT]-保存结果
// 返回值: 
/**************************************************************/
void GetAllProcessInSystem(OUT std::list<unsigned long>& listPid);

/**************************************************************/
// 杀掉进程的所有后代进程。
// 参数:
//		nPid[IN]-进程id
// 返回值: 
/**************************************************************/
void KillAllDescendantProcess(IN int nPid);

/**************************************************************/
// 在当前用户下创建进程
// 参数:
//		strCmdlineUtf8[IN]-命令行，utf-8格式
//		nNewProcessId[OUT]-返回新进程id
// 返回值: 
//		true-成功  false-失败
/**************************************************************/
bool CreateProcessAsCurrentUser(IN const std::string& strCmdlineUtf8, OUT unsigned long& nNewProcessId);

/**************************************************************/
// 删除目录及目录下的所有文件。
// 参数:
//		strDirPathUnicode[IN]-目录路径
// 返回值: 
//		true-成功  false-失败
/**************************************************************/
//bool DeleteDir(IN std::wstring& strDirPathUnicode);

/**************************************************************/
// 获取进程的路径（Unicode格式）
// 参数:
//		nPid[IN]-进程id
//		strPath[OUT]-保存进程路径
// 返回值: 
//		true-成功  false-失败
/**************************************************************/
bool GetProcessPathUnicode(IN int nPid, OUT std::wstring& strPath);

/**************************************************************/
// 提权
// 参数:
// 返回值: 
//		true-成功  false-失败
/**************************************************************/
bool EnableDebugPriv();

/**************************************************************/
// 获取文件签名信息的显示名
// 参数:
//		strPathUnicode[IN]-文件路径（Unicode格式）
// 返回值: 
//		签名信息的显示名（Unicode格式）
/**************************************************************/
std::wstring GetFileSignerDispName(IN const std::wstring& strPathUnicode);

/**************************************************************/
// 获取文件属性时间
// 参数:
//		strFilePathUnicode[IN]-文件路径（Unicode格式）
//		strCreateTime[OUT]-文件创建时间
//		strModifyTime[OUT]-文件修改时间
//		strAccessTime[OUT]-文件访问时间
// 返回值: 
//		true-成功  false-失败
/**************************************************************/
//bool GetFileAttributeTime(IN const std::wstring& strFilePathUnicode,
//	OUT std::string& strCreateTime,
//	OUT std::string& strModifyTime,
//	OUT std::string& strAccessTime);

/**************************************************************/
// 设置有名字对象的安全信息
// 参数:
//		strName[IN]-对象名
// 返回值: 
//		true-成功  false-失败
/**************************************************************/
bool SetNamedObjectSecurity(IN const std::string& strName);

/**************************************************************/
// 创建唯一实例。
// 参数:
//		strInstanceNameW[IN]-实例名
//		ppContext[OUT]-保存实例上下文
// 返回值: 
//		true-成功  false-失败
/**************************************************************/
bool CreateUniqueInstance(IN const std::wstring& strInstanceNameW, OUT void** ppContext);

/**************************************************************/
// 释放唯一实例。
// 参数:
//		pContext[IN]-实例上下文
// 返回值: 
/**************************************************************/
void ReleaseUniqueInstance(IN void* pContext);

/**************************************************************/
// 获得在指定目录下运行的进程的进程ID。
// 参数:
//		strDir[IN]-目录
//		vecPid[OUT]-后代进程数组
// 返回值: 
/**************************************************************/
void GetAllProcessInDir(IN const std::string& strDir, OUT std::vector<int>& vecPid);


/**************************************************************/
// 校验文件签名是否有效
// 参数:
//		strPathUtf8[IN]-文件路径（Utf8格式）
// 返回值: 
//		true-有效  false-无效
/**************************************************************/
//bool ValidFileSign(IN const std::string& strPathUtf8);