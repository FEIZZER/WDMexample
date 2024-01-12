
#pragma once

#include "AppHeader.h"


class CGlobal
{
public:
	CGlobal(void);
	virtual ~CGlobal(void);

	// 初始化
	static bool Init();

	// 反初始化
	static void UnInit();

	// 输出调试信息
	static void OutputA(const char* lpFormat, ...);
	static void OutputW(const wchar_t* lpFormat, ...);

	// 字符串格式化
	static void FormatA(int nArgSize, const char* lpFormat, va_list& argList, std::string& strRes);
	static void FormatW(const wchar_t* lpFormat, va_list& argList, std::wstring& strRes);
};

bool ReadFileData(IN const std::string& strFilePathUtf8, OUT std::string& strRes);
bool WriteFileData(IN const std::string& strFilePathUtf8, IN std::string& strData);
std::string ws2s(const std::wstring& ws);
std::string ws2s(wchar_t* pStr);
std::wstring s2ws(const std::string& s);
std::wstring s2ws(char* pStr);
std::string UnicodeToUTF8(const std::wstring& strUnicode);
std::wstring UTF8ToUnicode(const std::string& strUtf8);
bool IsTmZero(const struct tm& tmTime);

unsigned int GetCurTid();

/*****************************************************************************************/
// 结束进程。
// 参数:
//		nPid[IN]-进程id
// 返回值: 
/****************************************************************************************/
void TerminateProcessEx(IN unsigned int nPid);

// 转换strIp为有效ip。
std::string GetValidIp(IN const std::string& strIp);

//启动指定服务
bool StartService(IN const std::string& strServiceName);

//停止指定服务
bool StopService(IN const std::string& strServiceName);

//等待事件
bool NamedEventWait(IN const std::string& strEventName);

//释放事件
bool NamedEventRelease(IN const std::string& strEventName);

// 生成随机文件路径，strDir必须确保尾部有斜杠
void GenRandomFilePath(IN const std::string& strDir, IN const std::string& strFileSuffix, OUT std::string& strRes);

// 返回当前进程的路径（UTF-8格式）
std::string& GetCurProcessPathUTF8();

// 返回当前模块的路径（UTF-8格式）
std::string& GetCurModulePathUTF8();

// 返回文件后缀名，例如：传入 "c:\\test.exe" 返回 ".exe"
std::string GetFileExtensionFromPath(std::string& strPath);

// 返回文件名，例如："Global.cpp"
std::string GetFileNameFromPath(std::string& strPath);

// 返回目录，例如："D:\\UES_REL_R4_DLP\\agent"，无反斜杠结尾
std::string GetFileDirFromPath(IN std::string& strPath);

// 递归创建目录
bool CreateDirectories(IN const std::string& strPathUtf8);

void* GetAesCipher(); // 获取aes加密实例
bool AesEncryptString(IN const std::string& strSrc, OUT std::string& strRes); // aes加密，加密结果是BASE64字符串
bool AesDecryptString(IN const std::string& strSrc, OUT std::string& strRes); // aes解密