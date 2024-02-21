#include <ntddk.h>
#include <ntdef.h>

#include "../Common/CommunicateData.h"
#include "../Common/KBobMap.h"
#include "LogMgr.h"

#include "ProcessNotify.h"

#define SECONDS(n64)            (((LONGLONG)n64) * 10000000L)
#define PROCESS_QUERY_INFORMATION          (0x0400) 
#define PoolTag								'FEP1'


typedef struct _ProcessInfo
{
	ULONG ProcessId;
	USHORT Status;


}ProcessInfo, *PProcessInfo;
BobMap gProcessIdMap;
KEVENT* Process_Low_Event = NULL;

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

BOOLEAN IsEqualProcessInfo(IN PVOID pNodeInMap, IN PVOID pForeignKey)
{
	if (NULL == pNodeInMap || NULL == pForeignKey)
	{
		return FALSE;
	}

	return (((PProcessInfo)pNodeInMap)->ProcessId == *(PULONG)pForeignKey);
}

VOID Process_NotifyProcessEx(PEPROCESS Process, HANDLE ProcessId, PPS_CREATE_NOTIFY_INFO CreateInfo)
{
	if (!ProcessId || !CreateInfo) return;
	DbgPrint("feizzer: %wZ, Process Created", CreateInfo->ImageFileName);

	if (16 == RtlCompareMemory(L"Demo.exe", CreateInfo->ImageFileName->Buffer + CreateInfo->ImageFileName->Length/2 - 8, 16))
	{
		DbgPrint("feizzer processId:%d, imagePath:%wZ", HandleToULong(ProcessId), CreateInfo->ImageFileName);

		INT retry = 0;
		LARGE_INTEGER time;
		ProcessInfo ProInfo;
		ProInfo.ProcessId = HandleToULong(ProcessId);
		AddNodeToBobMap(&gProcessIdMap, &ProInfo.ProcessId, &ProInfo);
		AddLog(ProcessId);

		while (retry <= 10)
		{
			FindNodeInBobMap(&gProcessIdMap, &ProInfo.ProcessId, &ProInfo);

			if (ProInfo.Status == 1)
			{
				break;
			}


			time.QuadPart = -(SECONDS(1) / 4); // 250ms*40 = 10s
			KeWaitForSingleObject(Process_Low_Event,
				Executive, KernelMode, FALSE, &time);
			++retry;
		}

		DeleteNodeFromBobMap(&gProcessIdMap, &ProcessId, NULL);
	}
}

VOID UpdateProcessInfo(HANDLE ProcessId)
{
	ULONG ulong_pid = HandleToULong(ProcessId);
	PProcessInfo pProInfo = (PProcessInfo)GetAddrOfNode(&gProcessIdMap, &ulong_pid);
	pProInfo->Status = 1;
}

BOOLEAN InitProcessFrame() 
{
	Process_Low_Event = ExAllocatePool(NonPagedPool, sizeof(KEVENT));
	if (!Process_Low_Event) {
		return FALSE;
	}
	KeInitializeEvent(Process_Low_Event, SynchronizationEvent, FALSE);
	if (!InitBobMap(&gProcessIdMap, 5, sizeof(ProcessInfo), NonPagedPool, PoolTag, TRUE, FALSE, IsEqualProcessInfo, NULL))
	{
		DbgPrint("feizzer InitBobMap failed");
		return FALSE;
	}

	NTSTATUS status = PsSetCreateProcessNotifyRoutineEx(Process_NotifyProcessEx, FALSE);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("feizzer PsSetCreateProcessNotifyRoutineEx failed:%x", status);
		return FALSE;
	}
	return TRUE;
}

BOOLEAN UnInitProcessFrame()
{
	PsSetCreateProcessNotifyRoutineEx(Process_NotifyProcessEx, TRUE);

	return TRUE;
}


