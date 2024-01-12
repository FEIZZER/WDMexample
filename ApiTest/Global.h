
#pragma once

#include "AppHeader.h"


class CGlobal
{
public:
	CGlobal(void);
	virtual ~CGlobal(void);

	// ��ʼ��
	static bool Init();

	// ����ʼ��
	static void UnInit();

	// ���������Ϣ
	static void OutputA(const char* lpFormat, ...);
	static void OutputW(const wchar_t* lpFormat, ...);

	// �ַ�����ʽ��
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
// �������̡�
// ����:
//		nPid[IN]-����id
// ����ֵ: 
/****************************************************************************************/
void TerminateProcessEx(IN unsigned int nPid);

// ת��strIpΪ��Чip��
std::string GetValidIp(IN const std::string& strIp);

//����ָ������
bool StartService(IN const std::string& strServiceName);

//ָֹͣ������
bool StopService(IN const std::string& strServiceName);

//�ȴ��¼�
bool NamedEventWait(IN const std::string& strEventName);

//�ͷ��¼�
bool NamedEventRelease(IN const std::string& strEventName);

// ��������ļ�·����strDir����ȷ��β����б��
void GenRandomFilePath(IN const std::string& strDir, IN const std::string& strFileSuffix, OUT std::string& strRes);

// ���ص�ǰ���̵�·����UTF-8��ʽ��
std::string& GetCurProcessPathUTF8();

// ���ص�ǰģ���·����UTF-8��ʽ��
std::string& GetCurModulePathUTF8();

// �����ļ���׺�������磺���� "c:\\test.exe" ���� ".exe"
std::string GetFileExtensionFromPath(std::string& strPath);

// �����ļ��������磺"Global.cpp"
std::string GetFileNameFromPath(std::string& strPath);

// ����Ŀ¼�����磺"D:\\UES_REL_R4_DLP\\agent"���޷�б�ܽ�β
std::string GetFileDirFromPath(IN std::string& strPath);

// �ݹ鴴��Ŀ¼
bool CreateDirectories(IN const std::string& strPathUtf8);

void* GetAesCipher(); // ��ȡaes����ʵ��
bool AesEncryptString(IN const std::string& strSrc, OUT std::string& strRes); // aes���ܣ����ܽ����BASE64�ַ���
bool AesDecryptString(IN const std::string& strSrc, OUT std::string& strRes); // aes����