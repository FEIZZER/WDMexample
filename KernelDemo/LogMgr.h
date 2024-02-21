#pragma once


BOOLEAN AddLog(HANDLE processId);

BOOLEAN GetLog(PVOID pOutBuf, ULONG outLen);

BOOLEAN SetSyncEvent(HANDLE hEvent);