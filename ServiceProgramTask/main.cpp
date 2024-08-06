#include "windows.h"
#include <string>
#include "stdio.h"
#include <thread>

struct ServiceContext
{
	ServiceContext() = delete;
	ServiceContext(std::string strSvcName) : m_strSvcName(strSvcName)
	{
		;
	}
	~ServiceContext()
	{
		if (m_hStartedEvent)
			CloseHandle(m_hStartedEvent);

		if (m_hStopEvent)
			CloseHandle(m_hStopEvent);
	}
	std::string           m_strSvcName;
	SERVICE_STATUS_HANDLE m_svcStatusHandle = nullptr; //Service Status Handle
	SERVICE_STATUS        m_svcStatus;                 //Service Name

	//This event used to check whether service worker is started
	HANDLE                m_hStartedEvent = CreateEvent(
		NULL,    // default security attributes
		TRUE,    // manual reset event
		FALSE,   // not signaled
		NULL);   // no name;   

	//This event used to notify service worker to stop
	HANDLE                m_hStopEvent = CreateEvent(
		NULL,    // default security attributes
		TRUE,    // manual reset event
		FALSE,   // not signaled
		NULL);   // no name;      
	DWORD                 m_dwHintTimeout = 5 * 1000;  // 5 seconds, check interval for starting and stopping
	DWORD                 m_dwCheckPoint = 1;          // It's used to co-work check for starting and stopping
};

ServiceContext gCoderSvcCtx("CoderService");
std::wstring ServiceName = L"ServiceTest";


bool ReportSvcStatus(ServiceContext& ctx,
	DWORD dwCurrentState,
	DWORD dwWin32ExitCode)
{
	// Fill in the SERVICE_STATUS structure.
	ctx.m_svcStatus.dwCurrentState = dwCurrentState;
	ctx.m_svcStatus.dwWin32ExitCode = dwWin32ExitCode;
	ctx.m_svcStatus.dwWaitHint = ctx.m_dwHintTimeout;

	if (dwCurrentState == SERVICE_START_PENDING)
		ctx.m_svcStatus.dwControlsAccepted = 0;
	else
		ctx.m_svcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;

	if ((dwCurrentState == SERVICE_RUNNING) ||
		(dwCurrentState == SERVICE_STOPPED))
		ctx.m_svcStatus.dwCheckPoint = 0;
	else
		ctx.m_svcStatus.dwCheckPoint = ctx.m_dwCheckPoint++;

	// Report the status of the service to the SCM.
	return SetServiceStatus(ctx.m_svcStatusHandle, &ctx.m_svcStatus);
}

class CoderWorker
{
public:
	bool Start()
	{
		// You should fill your Worker thread init task here
		// Here just sleep 10 seconds to fake the init time
		std::this_thread::sleep_for(std::chrono::seconds(10));

		//Start the worker process tasks
		int iTmpPara = 1;
		m_thWorker = std::thread(&CoderWorker::Run, this, iTmpPara);

		return true;
	}
	void Stop()
	{
		m_bStop = true;
		if (m_thWorker.joinable())
			m_thWorker.join();

		// Here we just Fake sleep 10 seconds to stop
		std::this_thread::sleep_for(std::chrono::seconds(10));
	}

	void Run(int iPara)
	{
		while (!m_bStop)
		{
			//do task
			//Here just fill your code to do your work
			//just here fake sleep 1 second to do task
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}

private:
	std::thread   m_thWorker;
	volatile bool m_bStop = false;
};



// Here you can also specify other parameters
void CoderServiceWorker(ServiceContext& ctx, DWORD dwNumServicesArgs, LPSTR* lpServiceArgVectors)
{
	// 1. Init and Start worker and notify the it's started
	CoderWorker woker{};
	if (!woker.Start())
		ExitThread(-1);

	SetEvent(ctx.m_hStartedEvent);

	// 2. Wait for the stop event
	DWORD dwCode = WaitForSingleObject(ctx.m_hStopEvent, INFINITE);
	woker.Stop();
	ExitThread(0);
	return;
}

//Service Worker means what your service really do
void StartCoderServiceWorker(DWORD dwNumServicesArgs, LPWSTR* lpServiceArgVectors)
{
	// 1. Start your service worker to handle tasks
	std::thread thWorker(CoderServiceWorker, std::ref(gCoderSvcCtx), dwNumServicesArgs, lpServiceArgVectors);

	// 2. Check the starting status
	while (true)
	{
		DWORD dwCode = WaitForSingleObject(gCoderSvcCtx.m_hStartedEvent, gCoderSvcCtx.m_dwHintTimeout);
		if (WAIT_TIMEOUT == dwCode)
		{
			//Still starting
			ReportSvcStatus(gCoderSvcCtx, SERVICE_START_PENDING, NO_ERROR);
		}
		else if (WAIT_OBJECT_0 == dwCode)
		{
			//Started Finish
			ResetEvent(gCoderSvcCtx.m_hStartedEvent);
			ReportSvcStatus(gCoderSvcCtx, SERVICE_RUNNING, NO_ERROR);
			break;
		}
		else
		{
			//Other errors
			ReportSvcStatus(gCoderSvcCtx, SERVICE_STOPPED, NO_ERROR);
			return;
		}
	}

	// 3. Check whether receive stop event
	while (true)
	{
		DWORD dwCheckTime = 1000; //1 second
		DWORD dwCode = WaitForSingleObject(gCoderSvcCtx.m_hStopEvent, dwCheckTime);
		if (WAIT_TIMEOUT == dwCode)
		{
			//Still runining
			continue;
		}
		else if (WAIT_OBJECT_0 == dwCode)
		{
			//It begin to stop
			// run into Stop pending status
			ReportSvcStatus(gCoderSvcCtx, SERVICE_STOP_PENDING, NO_ERROR);
			break;
		}
		else
		{
			//Other errors
			ReportSvcStatus(gCoderSvcCtx, SERVICE_STOPPED, NO_ERROR);
			return;
		}
	}

	// 4. Check the stopping status
	// If it's stopped then set the status to STOP
	HANDLE hThreadHandle = thWorker.native_handle();
	while (true)
	{
		DWORD dwCode = WaitForSingleObject(hThreadHandle, gCoderSvcCtx.m_dwHintTimeout);
		if (WAIT_TIMEOUT == dwCode)
		{
			//Still stopping
			ReportSvcStatus(gCoderSvcCtx, SERVICE_STOP_PENDING, NO_ERROR);
		}
		else if (WAIT_OBJECT_0 == dwCode)
		{
			// stop Finish
			// Reset the stop event
			ResetEvent(gCoderSvcCtx.m_hStopEvent);
			ReportSvcStatus(gCoderSvcCtx, SERVICE_STOPPED, NO_ERROR);
			break;
		}
		else
		{
			//Other errors
			ReportSvcStatus(gCoderSvcCtx, SERVICE_STOPPED, NO_ERROR);
			return;
		}
	}
	return;
}



void CoderServiceController(DWORD dwControl)
{
	// Handle the requested control code. 
	switch (dwControl)
	{
	case SERVICE_CONTROL_SHUTDOWN:
	case SERVICE_CONTROL_STOP:
		ReportSvcStatus(gCoderSvcCtx, SERVICE_STOP_PENDING, NO_ERROR);
		// Signal the service to stop.
		SetEvent(gCoderSvcCtx.m_hStopEvent);

		return;

	default:
		break;
	}
}

//Each Service binding a ServiceMain
void CoderServiceMain(DWORD dwNumServicesArgs, LPWSTR* lpServiceArgVectors)
{
	// 1. Register Control Handler
	gCoderSvcCtx.m_svcStatusHandle = RegisterServiceCtrlHandler((WCHAR*)ServiceName.c_str(),
		CoderServiceController);

	if (!gCoderSvcCtx.m_svcStatusHandle)
		return;

	// 2.  Set SERVICE_START_PENDING
	gCoderSvcCtx.m_svcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	gCoderSvcCtx.m_svcStatus.dwServiceSpecificExitCode = 0;

	if (!ReportSvcStatus(gCoderSvcCtx, SERVICE_START_PENDING, NO_ERROR))
	{
		ReportSvcStatus(gCoderSvcCtx, SERVICE_STOPPED, NO_ERROR);
		return;
	}

	// 3. your worker start
	// Service woker will work for specified task, if it return means it's stopped
	StartCoderServiceWorker(dwNumServicesArgs, lpServiceArgVectors);
	return;
}



int main()
{
	SERVICE_TABLE_ENTRY svcTableEntry[] = {
		{(WCHAR*)ServiceName.c_str(), CoderServiceMain},
		{NULL, NULL}
	};

	//It should be invoked in main as soon as possible
	//Start failed, you can use GetLastError() to get the error code
	if (!StartServiceCtrlDispatcher(svcTableEntry))
		return -1;

	return 0;
}
