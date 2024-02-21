
#include <ntddk.h>
#include <ntdef.h>


#include "LogMgr.h"

HANDLE ProcessId2Inject;
PVOID pEvent = NULL;

BOOLEAN AddLog(HANDLE processId)
{
	ProcessId2Inject = processId;
	return TRUE;
}


BOOLEAN GetLog(PVOID pOutBuf, ULONG outLen)
{
	RtlCopyMemory(pOutBuf, ProcessId2Inject, outLen);
	if (pEvent == NULL)
	{
		return FALSE;
	}
	KeSetEvent((PRKEVENT)pEvent,
		IO_NO_INCREMENT,
		FALSE);

	return TRUE;
}


BOOLEAN SetSyncEvent(HANDLE hEvent)
{
	NTSTATUS status = ObReferenceObjectByHandle( // run at PASSIVE_LEVEL
		hEvent,
		EVENT_MODIFY_STATE,
		*ExEventObjectType,
		UserMode,
		&pEvent,
		NULL);

	if (!NT_SUCCESS(status))
	{
		DbgPrint("feizzer ObReferenceObjectByHandle failed:%x", status);
		return FALSE;
	}

	return TRUE;
}


