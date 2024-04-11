#pragma once

#include "WorkThread.h"
#include "Poco/SingletonHolder.h"
class IpcMgr: public WorkThread
{
public:
	IpcMgr();
	virtual ~IpcMgr();

	static IpcMgr& GetIntance()
	{
		static Poco::SingletonHolder<IpcMgr> sh;
		return *sh.get();
	}

	bool Init();
	void UnInit();




private:

};