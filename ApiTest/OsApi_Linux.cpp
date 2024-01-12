#include "OsApi.h"
#include "Global.h"
#include <regex>
#include <signal.h>


NamedEvent::NamedEvent(bool bAutoReset)
{
    m_pImpl = NULL;
}

NamedEvent::NamedEvent()
{
    m_pImpl = NULL;
}

NamedEvent::NamedEvent(const std::string& name)
{
    m_pImpl = NULL;
}

NamedEvent::~NamedEvent()
{
    m_pImpl = NULL;
}

void NamedEvent::set()
{
}

void NamedEvent::reset()
{
}

void NamedEvent::wait()
{
}

void NamedEvent::wait(long milliseconds)
{
}

bool NamedEvent::tryWait(long milliseconds)
{
    return false;
}


void OutputStringA(IN const char* pStr)
{
    printf("%s", pStr);
}

void OutputStringW(IN const wchar_t* pStr)
{
    wprintf(L"%s", pStr);
}

void GetAllProcessInSystem(OUT std::list<unsigned long>& listPid)
{
    listPid.clear();
}

bool GetProcessPathUnicode(IN int nPid, OUT std::wstring& strPath)
{
    bool bRet = false;
    char szPath[PATH_MAX] = { 0 };
    char szProcFilePath[64] = { 0 };

    sprintf(szProcFilePath, "/proc/%d/exe", nPid);
    size_t wRet = readlink(szProcFilePath, szPath, PATH_MAX);


    if (0 < wRet && wRet < PATH_MAX)
    {
        std::string strRet = szPath;
        strPath = s2ws(strRet);
        bRet = true;
    }

    return bRet;
}

std::wstring GetCurProcessPathUnicode()
{
    std::wstring strRetW = L"";
    GetProcessPathUnicode(getpid(), strRetW);

    return strRetW;
}

std::wstring GetModulePathByAddr(int nPid, void* pAddr)
{
    std::wstring strRetW = L"";
    char szMapFilePath[64] = { 0 };
    char cAddr[64] = { 0 };
    ifstream ifs;

    do
    {

        sprintf(cAddr, "%p", pAddr);
        string strAddr = cAddr;
        int nPos = strAddr.find("0x");
        if (nPos != -1)
        {
            strAddr = strAddr.substr(nPos + 2);
        }

        sprintf(szMapFilePath, "/proc/%d/maps", nPid);
        ifs.open(szMapFilePath, ios::in);
        if (!ifs.is_open())
        {
            CGlobal::OutputA("GetModulePathByAddr(), ifs.is_open failed! nPid:%d pAddr:%p Line:%d File:%s \n",
                nPid, pAddr, __LINE__, __FILE__);
            break;
        }

        string strBuf;
        regex regPattern = regex("(\\S+)-(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s*");
        smatch regResult;
        while (getline(ifs, strBuf))
        {
            bool bIsMatch = regex_match(strBuf, regResult, regPattern);
            if (!bIsMatch)
            {
                continue;
            }
            string strAddrStart = regResult[1];
            string strAddrEnd = regResult[2];
            if (strAddrStart <= strAddr && strAddr <= strAddrEnd)
            {
                strRetW = s2ws(regResult[7]);
                break;
            }
        }
    } while (0);

    ifs.close();

    return strRetW;
}

std::wstring GetCurModulePathUnicode()
{
    return GetModulePathByAddr(getpid(), (void*)GetCurModulePathUnicode);
}

bool CreateUniqueInstance(IN const std::wstring& strInstanceNameW, OUT void** ppContext)
{
    bool bRet = false;

    //do
    //{
    //    if (strInstanceNameW.empty() || NULL == ppContext)
    //    {
    //        CGlobal::OutputW(L"In CreateUniqueInstance(), param error! strInstanceNameW:%s ppContext:%p",
    //            strInstanceNameW.c_str(), ppContext);
    //        break;
    //    }

    //    {
    //        HANDLE hMutex = ::CreateMutexW(NULL, TRUE, strInstanceNameW.c_str());
    //        if (NULL != hMutex)
    //        {
    //            if (ERROR_ALREADY_EXISTS == ::GetLastError()) // ÒÑ¾­´æÔÚ
    //            {
    //                ::CloseHandle(hMutex);
    //            }
    //            else
    //            {
    //                *ppContext = hMutex;
    //                bRet = true;
    //            }
    //        }
    //        else
    //        {
    //            DWORD dwErr = ::GetLastError();
    //            CGlobal::OutputW(L"In CreateUniqueInstance(), CreateMutexW() failed! dwErr:%d %#x strInstanceNameW:%s",
    //                dwErr, dwErr, strInstanceNameW.c_str());
    //        }
    //    }

    //} while (0);

    return bRet;
}

void ReleaseUniqueInstance(IN void* pContext)
{
    //if (NULL != pContext)
    //{
    //    ::CloseHandle((HANDLE)pContext);
    //}
}

bool CreateProcessAsCurrentUser(IN const std::string& strCmdlineUtf8, OUT unsigned long& nNewProcessId)
{
    return false;
}