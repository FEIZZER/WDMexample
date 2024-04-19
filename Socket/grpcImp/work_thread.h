#pragma once
#ifndef WORKTHREAD
#define WORKTHREAD

#include <Poco/Thread.h>


class WorkThread : public Poco::Runnable
{
public:
	WorkThread() {};
	virtual ~WorkThread() {};

	virtual void run() {};


	bool InitThread()
	{
		thread_.start(*this);
		return thread_.isRunning();
	}


	bool UnInitThread() {};

private:

	Poco::Thread thread_;

};

#endif // !WORKTHREAD

