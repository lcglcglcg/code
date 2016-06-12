#include <ddk\ntddk.h>
typedef struct _DeviceExtension
{
    PDEVICE_OBJECT pThisDevice;
    UNICODE_STRING DeviceName;
    UNICODE_STRING SymboLinkName;
}DEVICE_EXTENSION,*PDEVICE_EXTENSION;
NTSTATUS STDCALL DriverEntry(IN PDRIVER_OBJECT,IN PUNICODE_STRING);
NTSTATUS STDCALL CreateDevice(IN PDRIVER_OBJECT);
void STDCALL DrvUnload(IN PDRIVER_OBJECT);
NTSTATUS STDCALL DrvDispatch(IN PDEVICE_OBJECT,IN PIRP);


//入口函数
NTSTATUS STDCALL DriverEntry(IN PDRIVER_OBJECT pDriverObject,IN PUNICODE_STRING pRegistryPath)
{
    DbgPrint(" Enter Driver\n");
    NTSTATUS Status;
    pDriverObject->MajorFunction[IRP_MJ_CREATE]=DrvDispatch;
    pDriverObject->MajorFunction[IRP_MJ_CLOSE]=DrvDispatch;
    pDriverObject->MajorFunction[IRP_MJ_READ]=DrvDispatch;
    pDriverObject->MajorFunction[IRP_MJ_WRITE]=DrvDispatch;
    pDriverObject->DriverUnload=DrvUnload;
    Status=CreateDevice(pDriverObject);
    return Status;
}

//insert
NTSTATUS STDCALL CreateDevice(IN PDRIVER_OBJECT pDriverObject)
{
    DbgPrint("CreateDevice\n");
    NTSTATUS Status;
    PDEVICE_OBJECT pDeviceObject;
    PDEVICE_EXTENSION pDeviceExtension;
    UNICODE_STRING DeviceName;
    RtlInitUnicodeString(&DeviceName,L"\\Device\\Device");
    Status=IoCreateDevice(pDriverObject,sizeof(DEVICE_EXTENSION),&(UNICODE_STRING)DeviceName,FILE_DEVICE_UNKNOWN,0,TRUE,&pDeviceObject);
    if(!NT_SUCCESS(Status))
    {
	DbgPrint("Create Device Error\n");
	return Status;
    }
    DbgPrint("CreateDevice Successfully\n");
    pDeviceObject->Flags|=DO_BUFFERED_IO;
    pDeviceExtension=(PDEVICE_EXTENSION)pDeviceObject->DeviceExtension;
    pDeviceExtension->pThisDevice=pDeviceObject;
    pDeviceExtension->DeviceName=DeviceName;

    //Create SymboLink
    UNICODE_STRING SymboLink;
    RtlInitUnicodeString(&SymboLink,L"\\??\\SymboLink");
    pDeviceExtension->SymboLinkName=SymboLink;
    Status=IoCreateSymbolicLink(&SymboLink,&DeviceName);
    if(!NT_SUCCESS(Status))
    {
	IoDeleteDevice(pDeviceObject);
	return Status;
    }
    return Status;
}

//unload
void STDCALL DrvUnload(IN PDRIVER_OBJECT pDriverObject)
{
    DbgPrint("Driver Unload\n");
    PDEVICE_OBJECT pNextObject;
    pNextObject=pDriverObject->DeviceObject;
    while(pNextObject!=NULL)
    {
	PDEVICE_EXTENSION pDeviceExtension=(PDEVICE_EXTENSION)pNextObject->DeviceExtension;
	UNICODE_STRING pLinkName=pDeviceExtension->SymboLinkName;
	IoDeleteSymbolicLink(&pLinkName);
	pNextObject=pNextObject->NextDevice;
	IoDeleteDevice(pDeviceExtension->pThisDevice);
    }
    return;
}

NTSTATUS STDCALL DrvDispatch(IN PDEVICE_OBJECT pDeviceObject,IN PIRP pIrp)
{
    DbgPrint("DrvDispatch\n");
    return STATUS_SUCCESS;
}


