
#include "Global.h"

#include "OsApi.h"

#define MaxFormatStrNum						512				// 最大格式化字符数

CGlobal::CGlobal(void)
{
}

CGlobal::~CGlobal(void)
{
}

bool CGlobal::Init()
{
	CGlobal::OutputA("CGlobal::Init(), GetCurProcessPathUTF8:%s GetCurModulePathUTF8:%s Line:%d File:%s \r\n",
		GetCurProcessPathUTF8().c_str(),
		GetCurModulePathUTF8().c_str(),
		__LINE__, __FILE__);

	GetAesCipher(); // 初始化加解密

	return true;
}

void CGlobal::UnInit()
{
	CGlobal::OutputA("CGlobal::UnInit(), Line:%d File:%s \r\n", __LINE__, __FILE__);
}

void CGlobal::FormatA(int nArgSize, const char* lpFormat, va_list& argList, std::string& strRes)
{
	return;

	int nTemp, nBufLen = MaxFormatStrNum;
	char szBuf[MaxFormatStrNum + 8] = { 0 };

	// 在mac下，函数vsnprintf使用argList多次会内存异常
	//int nArgSize = vsnprintf(NULL, 0, lpFormat, argList); // 计算参数所需字符个数

	char* pBuf = szBuf;
	bool bNewBuf = false;

	//_snprintf_s(pBuf, nBufLen, _TRUNCATE, "[%s] ", CBobOutput::m_strModuleName.c_str());	

	tm tmLocal;
	time_t tTime;
	time(&tTime);	
	localtime_s(&tmLocal, &tTime);
	snprintf(pBuf, nBufLen, "[%04d-%02d-%02d %02d:%02d:%02d] ",
		tmLocal.tm_year + 1900, tmLocal.tm_mon + 1, tmLocal.tm_mday, tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);

	//LocalDateTime ldtCurTime;
	////_snprintf_s(pBuf, nBufLen, _TRUNCATE, "[%04d-%02d-%02d %02d:%02d:%02d] ",
	//snprintf(pBuf, nBufLen, "[%04d-%02d-%02d %02d:%02d:%02d] ",
	//	ldtCurTime.year(), ldtCurTime.month(), ldtCurTime.day(), ldtCurTime.hour(), ldtCurTime.minute(), ldtCurTime.second());

	//SYSTEMTIME st;
	//::GetLocalTime(&st);
	//_snprintf_s(pBuf, nBufLen, _TRUNCATE, "[%04d-%02d-%02d %02d:%02d:%02d] ",
	//	st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	nTemp = (int)strlen(pBuf);
	if (nArgSize + nTemp >= nBufLen)
	{
		nBufLen = nArgSize + nTemp + 8;

		try
		{
			pBuf = new char[nBufLen];
		}
		catch (...)
		{
		}

		if (NULL != pBuf)
		{
			bNewBuf = true;
		}

		if (bNewBuf)
		{
			memcpy(pBuf, szBuf, (MaxFormatStrNum + 8) * sizeof(char));
		}
		else
		{
			OutputStringA("\r\n CGlobal::FormatA(), new buffer failed! \r\n");
			return;
		}
	}

	nBufLen -= nTemp;
	//nTemp = _vsnprintf_s(pBuf + nTemp, nBufLen, _TRUNCATE, lpFormat, argList);
	nTemp = vsnprintf(pBuf + nTemp, nBufLen, lpFormat, argList);
	if (0 < nTemp && nTemp < nBufLen)
	{
		strRes = pBuf;
	}

	if (bNewBuf)
	{
		delete[] pBuf;
	}
}

void CGlobal::FormatW(const wchar_t* lpFormat, va_list& argList, std::wstring& strRes)
{
	return;

	int nTemp, nBufLen = MaxFormatStrNum;
	wchar_t szBuf[MaxFormatStrNum + 8] = { 0 };
	//int nArgSize = _vsnwprintf(NULL, 0, lpFormat, argList); // 计算参数所需字符个数
	int nArgSize = vswprintf(NULL, 0, lpFormat, argList); // 计算参数所需字符个数

	wchar_t* pBuf = szBuf;
	bool bNewBuf = false;

	//_snwprintf_s(pBuf, nBufLen, _TRUNCATE, L"[%s] ", CBobOutput::m_strModuleNameW.c_str());

	tm tmLocal;
	time_t tTime;
	time(&tTime);
	localtime_s(&tmLocal, &tTime);
	swprintf(pBuf, nBufLen, L"[%04d-%02d-%02d %02d:%02d:%02d] ",
		tmLocal.tm_year + 1900, tmLocal.tm_mon + 1, tmLocal.tm_mday, tmLocal.tm_hour, tmLocal.tm_min, tmLocal.tm_sec);

	//LocalDateTime ldtCurTime;
	////_snwprintf_s(pBuf, nBufLen, _TRUNCATE, L"[%04d-%02d-%02d %02d:%02d:%02d] ",
	//swprintf(pBuf, nBufLen, L"[%04d-%02d-%02d %02d:%02d:%02d] ",
	//	ldtCurTime.year(), ldtCurTime.month(), ldtCurTime.day(), ldtCurTime.hour(), ldtCurTime.minute(), ldtCurTime.second());

	//SYSTEMTIME st;
	//::GetLocalTime(&st);
	//_snwprintf_s(pBuf, nBufLen, _TRUNCATE, L"[%04d-%02d-%02d %02d:%02d:%02d] ",
	//	st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	nTemp = (int)wcslen(pBuf);
	if (nArgSize + nTemp >= nBufLen)
	{
		nBufLen = nArgSize + nTemp + 8;

		try
		{
			pBuf = new wchar_t[nBufLen];
		}
		catch (...)
		{
		}

		if (NULL != pBuf)
		{
			bNewBuf = true;
		}

		if (bNewBuf)
		{
			memcpy(pBuf, szBuf, (MaxFormatStrNum + 8) * sizeof(wchar_t));
		}
		else
		{
			OutputStringW(L"\r\n CGlobal::FormatW(), new buffer failed! \r\n");
			return;
		}
	}

	nBufLen -= nTemp;
	//nTemp = _vsnwprintf_s(pBuf + nTemp, nBufLen, _TRUNCATE, lpFormat, argList);
	nTemp = vswprintf(pBuf + nTemp, nBufLen, lpFormat, argList);
	if (0 < nTemp && nTemp < nBufLen)
	{
		strRes = pBuf;
	}

	if (bNewBuf)
	{
		delete[] pBuf;
	}
}

void CGlobal::OutputA(const char* lpFormat, ...)
{
	return;

	if (NULL == lpFormat)
	{
		return;
	}

	va_list argList;

	va_start(argList, lpFormat);
	int nArgSize = vsnprintf(NULL, 0, lpFormat, argList); // 计算参数所需字符个数
	va_end(argList);

	va_start(argList, lpFormat);
	if (NULL != argList)
	{
		std::string strRes = "";
		CGlobal::FormatA(nArgSize, lpFormat, argList, strRes);
		if (!strRes.empty())
		{
			OutputStringA(strRes.c_str());
		}
	}

	va_end(argList);
}

void CGlobal::OutputW(const wchar_t* lpFormat, ...)
{
	return;

	if (NULL == lpFormat)
	{
		return;
	}

	va_list argList;
	va_start(argList, lpFormat);
	if (NULL != argList)
	{
		std::wstring strRes = L"";
		CGlobal::FormatW(lpFormat, argList, strRes);
		if (!strRes.empty())
		{
			OutputStringW(strRes.c_str());
		}
	}

	va_end(argList);
}

bool ReadFileData(IN const std::string& strFilePathUtf8, OUT std::string& strRes)
{
	bool bRet = false;

	if (strFilePathUtf8.empty())
	{
		return bRet;
	}

	strRes = "";

	try
	{

#if defined(UHS_OS_WIN)

		std::wstring strPathUnicode = UTF8ToUnicode(strFilePathUtf8); // 需要将utf8编码转为Unicode编码，微软的函数才能正常调用
		std::ifstream fs(strPathUnicode, std::ios_base::binary);

#else

		std::ifstream fs(strFilePathUtf8, std::ios_base::binary);

#endif

		fs.seekg(std::ios_base::beg, std::ios_base::end);
		auto size = fs.tellg();
		fs.seekg(std::ios_base::beg);
		strRes.resize(static_cast<size_t>(size));
		fs.read(&strRes[0], size);
		fs.close();

		bRet = true;
	}
	catch (...)
	{
	}

	return bRet;
}

bool WriteFileData(IN const std::string& strFilePathUtf8, IN std::string& strData)
{
	bool bRet = false;

	if (strFilePathUtf8.empty())
	{
		return bRet;
	}

	try
	{

#if defined(UHS_OS_WIN)

		std::wstring strPathUnicode = UTF8ToUnicode(strFilePathUtf8); // 需要将utf8编码转为Unicode编码，微软的函数才能正常调用
		std::ofstream ofs(strPathUnicode, std::ios_base::binary);

#else

		std::ofstream ofs(strFilePathUtf8, std::ios_base::binary);

#endif

		ofs << strData;
		ofs.close();

		bRet = true;
	}
	catch (...)
	{
	}

	return bRet;
}

std::string ws2s(const std::wstring& ws)
{
	if (ws.empty())
		return "";

	//size_t nReturnValue;
	std::string curLocale = setlocale(LC_ALL, NULL);
	setlocale(LC_ALL, "chs");

	const wchar_t* _source = ws.c_str();
	size_t _dsize = sizeof(wchar_t) * (ws.size() + 1);
	char* _dest = new char[_dsize + 8];
	memset(_dest, 0x0, _dsize + 8);
	//wcstombs_s(&nReturnValue, _dest, _dsize + 4, _source, _dsize);
	wcstombs(_dest, _source, _dsize);
	std::string result = _dest;
	delete[] _dest;

	setlocale(LC_ALL, curLocale.c_str());

	return result;
}

std::string ws2s(wchar_t* pStr)
{
	if (NULL == pStr)
		return "";

	return ws2s(std::wstring(pStr));
}

std::wstring s2ws(const std::string& s)
{
	if (s.empty())
		return L"";

	//size_t nReturnValue;
	std::string curLocale = setlocale(LC_ALL, NULL);
	setlocale(LC_ALL, "chs");

	const char* _source = s.c_str();
	size_t _dsize = s.size() + 1;
	wchar_t* _dest = new wchar_t[_dsize + 4];
	memset(_dest, 0x0, (_dsize + 4) * sizeof(wchar_t));
	//mbstowcs_s(&nReturnValue, _dest, _dsize + 2, _source, _dsize);
	mbstowcs(_dest, _source, _dsize);
	std::wstring result = _dest;
	delete[] _dest;

	setlocale(LC_ALL, curLocale.c_str());

	return result;
}

std::wstring s2ws(char* pStr)
{
	if (NULL == pStr)
		return L"";

	return s2ws(std::string(pStr));
}

std::string UnicodeToUTF8(const std::wstring& strUnicode)
{
	std::string ret;

	try
	{
		std::wstring_convert< std::codecvt_utf8<wchar_t> > wcv;
		ret = wcv.to_bytes(strUnicode);
	}
	catch (...)
	{
		ret = "";
	}

	return ret;
}

std::wstring UTF8ToUnicode(const std::string& strUtf8)
{
	std::wstring ret;

	try
	{
		std::wstring_convert< std::codecvt_utf8<wchar_t> > wcv;
		ret = wcv.from_bytes(strUtf8);
	}
	catch (...)
	{
		ret = L"";
	}

	return ret;
}

bool IsTmZero(const struct tm& tmTime)
{
	if (0 == tmTime.tm_sec &&
		0 == tmTime.tm_min &&
		0 == tmTime.tm_hour &&
		0 == tmTime.tm_mday &&
		0 == tmTime.tm_mon &&
		0 == tmTime.tm_year &&
		0 == tmTime.tm_wday &&
		0 == tmTime.tm_yday &&
		0 == tmTime.tm_isdst)
	{
		return true;
	}

	return false;
}

unsigned int GetCurTid()
{
	//return boost::this_thread::get_id();

	std::thread::id nCurTid = std::this_thread::get_id();
	return (*((unsigned int*)(&nCurTid)));
}

void TerminateProcessEx(IN unsigned int nPid)
{
	if (0 >= nPid)
	{
		return;
	}

	try
	{
		::boost::process::detail::api::pid_t pid = nPid;
		boost::process::child process{ pid };

		for (int i = 0; i < 10; i++) // 等待进程结束
		{
			if (!process.running())
			{
				break;
			}

			boost::this_thread::sleep(boost::posix_time::milliseconds(200)); // 睡眠200毫秒
		}

		if (process.running())
		{
			process.terminate();
		}
	}
	catch (...)
	{
	}
}

std::string GetValidIp(IN const std::string& strIp)
{
	// 如果sever在0.0.0.0上监听，则本机中的client可以通过127.0.0.1和本机真实ip（例如192.168.12.134）连上server，
	// 其它电脑中的client可以通过真实ip（例如192.168.12.134）连上server。
	if (0 == strIp.compare("0.0.0.0"))
	{
		return std::string("127.0.0.1");
	}

	return strIp;
}

bool StartService(IN const std::string& strServiceName)
{
	bool bRet = false;

//#if defined(UHS_OS_WIN)
//
//	if (strServiceName.empty())
//	{
//		return bRet;
//	}
//
//	try
//	{
//		Poco::Util::WinService clsService(strServiceName);
//		clsService.start(); // 启动服务
//
//		bRet = true;
//	}
//	catch (Poco::Exception& exc)
//	{
//		CGlobal::OutputA("StartService(%s), Exception:{%s} CurTid:%d Line:%d File:%s \r\n",
//			strServiceName.c_str(), exc.displayText().c_str(), Thread::currentTid(), __LINE__, __FILE__);
//	}
//
//#else
//
//	bRet = true;
//
//#endif

	return bRet;
}

bool StopService(IN const std::string& strServiceName)
{
	bool bRet = false;

//#if defined(UHS_OS_WIN)
//
//	if (strServiceName.empty())
//	{
//		return bRet;
//	}
//
//	try
//	{
//		Poco::Util::WinService clsService(strServiceName);
//		clsService.stop(); // 停止服务
//
//		bRet = true;
//	}
//	catch (Poco::Exception& exc)
//	{
//		CGlobal::OutputA("StopService(%s), Exception:{%s} CurTid:%d Line:%d File:%s \r\n",
//			strServiceName.c_str(), exc.displayText().c_str(), Thread::currentTid(), __LINE__, __FILE__);
//	}
//
//#else
//
//	bRet = true;
//
//#endif

	return bRet;
}

bool NamedEventWait(IN const std::string& strEventName)
{
	bool bRet = false;

	do
	{
		if (strEventName.empty())
		{
			CGlobal::OutputA("In NamedEventWait(), strEventName is empty! CurTid:%d Line:%d File:%s \r\n",
				GetCurTid(), __LINE__, __FILE__);
			break;
		}

		try
		{
			NamedEvent terminator(strEventName);
			terminator.wait(); // 等待退出通知

			bRet = true;
		}
		catch (...)
		{
			CGlobal::OutputA("In NamedEventWait(), strEventName:%s Exception! CurTid:%d Line:%d File:%s \r\n",
				strEventName.c_str(), GetCurTid(), __LINE__, __FILE__);
		}

	} while (0);

	return bRet;
}

bool NamedEventRelease(IN const std::string& strEventName)
{
	bool bRet = false;

	do
	{
		if (strEventName.empty())
		{
			CGlobal::OutputA("In NamedEventRelease(), strEventName is empty! CurTid:%d Line:%d File:%s \r\n",
				GetCurTid(), __LINE__, __FILE__);
			break;
		}

		try
		{
			NamedEvent terminator(strEventName);
			terminator.set(); // 通知退出

			bRet = true;
		}
		catch (...)
		{
			CGlobal::OutputA("In NamedEventRelease(), strEventName:%s Exception! CurTid:%d Line:%d File:%s \r\n",
				strEventName.c_str(), GetCurTid(), __LINE__, __FILE__);
		}

	} while (0);

	return bRet;
}

void GenRandomFilePath(IN const std::string& strDir, IN const std::string& strFileSuffix, OUT std::string& strRes)
{
	boost::uuids::uuid uuid = boost::uuids::random_generator()();
	strRes = strDir + boost::uuids::to_string(uuid) + strFileSuffix;
}

std::string& GetCurProcessPathUTF8()
{
	static std::string strRet = "";

	if (strRet.empty())
	{
		strRet = UnicodeToUTF8(GetCurProcessPathUnicode());
	}

	return strRet;
}

std::string& GetCurModulePathUTF8()
{
	static std::string strRet = "";

	if (strRet.empty())
	{
		strRet = UnicodeToUTF8(GetCurModulePathUnicode());
	}

	return strRet;
}

std::string GetFileExtensionFromPath(std::string& strPath)
{
	std::string strRet;

	try
	{
		boost::filesystem::path clsPath(strPath);
		strRet = clsPath.extension().string();
	}
	catch (...)
	{
		strRet = "";
	}

	return strRet;
}

std::string GetFileNameFromPath(std::string& strPath)
{
	std::string strRet;

	try
	{
		boost::filesystem::path clsPath(strPath);
		strRet = clsPath.filename().string();
	}
	catch (...)
	{
		strRet = "";
	}

	return strRet;
}

std::string GetFileDirFromPath(IN std::string& strPath)
{
	std::string strRet = "";

	try
	{
		boost::filesystem::path clsPath(strPath);
		strRet = clsPath.parent_path().string();
	}
	catch (...)
	{
	}

	return strRet;
}

bool CreateDirectories(IN const std::string& strPathUtf8)
{
	bool bRet = true;

	try
	{
		if (!boost::filesystem::exists(strPathUtf8))
		{
			if (!boost::filesystem::create_directories(strPathUtf8))
			{
				bRet = false;
			}
		}
	}
	catch (...)
	{
		bRet = false;
	}

	return bRet;
}

void* GetAesCipher()
{
	//static Cipher::Ptr pCipher = NULL;

	//if (!pCipher)
	//{
	//	std::string password = "{906203EF-6FD7-4CF8-A02C-67D8655B52FA}";
	//	std::string salt("{8A1144EF-9CDE-4B27-9C4E-6032939958C6}");
	//	std::string digest("sha1");
	//	CipherKey key("aes256", password, salt, 100, digest);
	//	pCipher = CipherFactory::defaultFactory().createCipher(key);
	//}

	//return pCipher;

	return NULL;
}

bool AesEncryptString(IN const std::string& strSrc, OUT std::string& strRes)
{
	bool bRet = false;

	if (strSrc.empty())
	{
		return bRet;
	}

	try
	{
		//Cipher::Ptr pCipher = (Cipher::Ptr)GetAesCipher();
		//if (NULL != pCipher)
		//{
		//	strRes = pCipher->encryptString(strSrc, Cipher::ENC_BASE64);
		//	bRet = true;
		//}

		strRes = strSrc;
		bRet = true;
	}
	catch (...)
	{
	}

	return bRet;
}

bool AesDecryptString(IN const std::string& strSrc, OUT std::string& strRes)
{
	bool bRet = false;

	if (strSrc.empty())
	{
		return bRet;
	}

	try
	{
		//Cipher::Ptr pCipher = (Cipher::Ptr)GetAesCipher();
		//if (NULL != pCipher)
		//{
		//	strRes = pCipher->decryptString(strSrc, Cipher::ENC_BASE64);
		//	bRet = true;
		//}

		strRes = strSrc;
		bRet = true;
	}
	catch (...)
	{
	}

	return bRet;
}