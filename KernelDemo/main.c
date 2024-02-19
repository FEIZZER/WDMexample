#include <ntddk.h>

#include "ProcessNotify.h"
#include "../Common/CommunicateData.h"


#define DEVICE_NAME		L"\\DosDevices\\YOUGIN_DEVICE"

NTSTATUS IrpDeviceControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	NTSTATUS status;
	PIO_STACK_LOCATION irpSp = IoGetCurrentIrpStackLocation(Irp);
	ULONG IoControlCode = irpSp->Parameters.DeviceIoControl.IoControlCode;

	switch (IoControlCode)
	{
	case IOCTL_GetProcessNotify:
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