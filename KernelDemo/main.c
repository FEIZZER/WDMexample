#include <ntddk.h>

#include "ProcessNotify.h"
#include "../Common/CommunicateData.h"
#include "LogMgr.h"

#define DEVICE_NAME		L"\\DosDevices\\YOUGIN_DEVICE"

NTSTATUS IrpDeviceControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	NTSTATUS status = STATUS_SUCCESS;
	PIO_STACK_LOCATION irpSp = IoGetCurrentIrpStackLocation(Irp);
	ULONG IoControlCode = irpSp->Parameters.DeviceIoControl.IoControlCode;
	PVOID pBuf = Irp->AssociatedIrp.SystemBuffer;
	ULONG ulInputLen = irpSp->Parameters.DeviceIoControl.InputBufferLength;
	ULONG ulOutputLen = irpSp->Parameters.DeviceIoControl.OutputBufferLength;

	switch (IoControlCode)
	{
	case IOCTL_GetProcessNotify:
		DbgPrint("feizzer IOCTL_GetProcessNotify %d", ulOutputLen);
		GetLog(pBuf, ulOutputLen);
		break;

	case IOCTL_SetSyncEvent:
		DbgPrint("feizzer IOCTL_SetSyncEvent");
		SetSyncEvent(*(HANDLE*)pBuf);
		break;
	case IOCTL_SetProcessStatus:
		DbgPrint("feizzer IOCTL_SetProcessStatus");
		UpdateProcessInfo(*(HANDLE*)pBuf);
		break;
	default:
		break;
	}
	return status;
}


void DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	DbgPrint("yougin, In DriverUnload");
	UnInitProcessFrame();
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING prRegPath)
{
	DbgPrint("yougin, In DriverEntry");
	pDriverObject->DriverUnload = DriverUnload;

	PDEVICE_OBJECT pDeviceObject = NULL;
	UNICODE_STRING ntUnicodeString;
	RtlInitUnicodeString(&ntUnicodeString, DEVICE_NAME);
	NTSTATUS ntstatus = IoCreateDevice(
		pDriverObject,                   // Our Driver Object
		0,                              // We don't use a device extension
		&ntUnicodeString,               // Device name "\Device\SIOCTL"
		FILE_DEVICE_UNKNOWN,            // Device type
		FILE_DEVICE_SECURE_OPEN,		// Device characteristics
		FALSE,                          // Not an exclusive device
		&pDeviceObject);                // Returned ptr to Device Object

	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IrpDeviceControl;


	InitProcessFrame();
	return ntstatus;
}