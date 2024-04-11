#include "WorkThread.h"


WorkThread::WorkThread()
{
}

WorkThread::~WorkThread()
{
	UnInitThread();
}

bool WorkThread::InitThread()
{
	try
	{
		m_exitAtom = 0;
		m_exitEvent.reset();
		m_workThread.start(*this);
	}
	catch (...)
	{
	}
	return IsRunning();
}
bool WorkThread::UnInitThread()
{
	bool bRet = false;
	SetExit();

	try
	{
		bRet = m_workThread.tryJoin(2000);
	}
	catch (...)
	{
	}
	
	if (!bRet) // todo: Ç¿É±½ø³Ì
	{
	}

	return bRet;
}

bool WorkThread::IsRunning()
{
	return m_workThread.isRunning();
}


void WorkThread::SetExit()
{
	try
	{
		m_exitAtom = 1;
		m_exitEvent.set();
	}
	catch (...)
	{
	}
}
bool WorkThread::CheckExit(unsigned long milliseconds)
{
	try
	{
		if (m_exitAtom.value())
		{
			return true;
		}
		if (milliseconds == 0)
		{
			return false;
		}

		if (m_exitEvent.tryWait(milliseconds))
		{
			return true;
		}
		if (m_exitAtom.value())
		{
			return true;
		}
	}
	catch (...)
	{
	}
	return false;
}

// empty virtual
void WorkThread::run()
{
}