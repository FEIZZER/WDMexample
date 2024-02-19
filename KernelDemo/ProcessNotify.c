#include "ProcessNotify.h"

#include <ntddk.h>
#include <ntdef.h>

#include "../Common/CommunicateData.h"
#define PROCESS_QUERY_INFORMATION          (0x0400) 


BOOLEAN GetProcessName(HANDLE ProcessId)
{
	BOOLEAN bRet = FALSE;
	NTSTATUS status;
	OBJECT_ATTRIBUTES objattrs;
	CLIENT_ID cid;
	HANDLE hProcess = NULL;

	if (!ProcessId) return FALSE;
	
	do
	{
		InitializeObjectAttributes(&objattrs, NULL, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
		cid.UniqueProcess = ProcessId;
		cid.UniqueThread = 0;
		status = ZwOpenProcess(&hProcess, PROCESS_QUERY_INFORMATION, &objattrs, &cid);
		if (!NT_SUCCESS(status))
		{
			break;
		}

		bRet = TRUE;
	} while (0);
	
	
	
	return bRet;
}



VOID Process_NotifyProcessEx(PEPROCESS Process, HANDLE ProcessId, PPS_CREATE_NOTIFY_INFO CreateInfo)
{
	if (!ProcessId || !CreateInfo) return;
	DbgPrint("yougin Process Created");
	DbgPrint("yougin: %wZ, Process Created", CreateInfo->ImageFileName);
}

BOOLEAN InitProcessFrame() 
{

	NTSTATUS status = PsSetCreateProcessNotifyRoutineEx(Process_NotifyProcessEx, FALSE);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("yougin PsSetCreateProcessNotifyRoutineEx failed:%x", status);
		return FALSE;
	}
	return TRUE;
}

BOOLEAN UnInitProcessFrame()
{
	PsSetCreateProcessNotifyRoutineEx(Process_NotifyProcessEx, TRUE);

	return TRUE;
}
