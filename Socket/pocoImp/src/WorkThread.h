#pragma once

#include "Poco/Runnable.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"

class WorkThread: public Poco::Runnable
{
public:

	WorkThread();
	~WorkThread();

	virtual void run();

	bool InitThread();
	bool UnInitThread();
	
	void SetExit();
	bool CheckExit(unsigned long milliseconds);
	
	bool IsRunning();

private:

	Poco::Thread m_workThread;
	Poco::Event  m_exitEvent;
	Poco::AtomicCounter m_exitAtom;
};

