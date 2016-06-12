#include <ddk/ntddk.h>
#include <ddk/ntifs.h>
#include <ddk/ntapi.h>

extern VOID STDCALL DriverUnload(IN PDRIVER_OBJECT DriverObject);
extern NTSTATUS STDCALL DriverCreate(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
extern NTSTATUS STDCALL DriverClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
extern NTSTATUS STDCALL DriverControl(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);

extern PVOID GetSystemCallAddress(wchar_t *str);
extern PVOID GetKillProcessAddress(void);
extern VOID PspTerminateProcess(int pid);

NTSTATUS STDCALL DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{

    DbgPrint("[DriverEntry]");
    DriverObject->DriverUnload = DriverUnload;
    DriverObject->MajorFunction[IRP_MJ_CREATE] = DriverCreate;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = DriverClose;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DriverControl;

    PDEVICE_OBJECT pDevice;
    UNICODE_STRING devNameUnicd;
    UNICODE_STRING devLinkUnicd;
    RtlInitUnicodeString(&devNameUnicd,L"\\Device\\MyEvent");
    RtlInitUnicodeString(&devLinkUnicd,L"\\??\\MyEvent");

    NTSTATUS Status = IoCreateDevice(DriverObject,
	    0,
	    &devNameUnicd,FILE_DEVICE_UNKNOWN,
	    0,
	    FALSE,
	    &pDevice);

    if(!NT_SUCCESS(Status)) {

	DbgPrint("[DriverEntry][IoCreateDevice failed]");
	return Status;
    }

    Status = IoCreateSymbolicLink(&devLinkUnicd,&devNameUnicd);
    if(!NT_SUCCESS(Status)) {

	DbgPrint("[DriverEntry][IoCreateSymbolicLink failed]");
	return Status;
    }

    return STATUS_SUCCESS;
}

VOID STDCALL DriverUnload(IN PDRIVER_OBJECT DriverObject)
{

    DbgPrint("[DriverUnload]");

    UNICODE_STRING devLinkUnicd;
    RtlInitUnicodeString(&devLinkUnicd,L"\\??\\MyEvent");

    NTSTATUS Status = IoDeleteSymbolicLink(&devLinkUnicd);
    if(!NT_SUCCESS(Status)) {

	DbgPrint("[DriverUnload][IoDeleteSymbolicLink failed]");
	return;
    }

    IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS STDCALL DriverCreate(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{

    DbgPrint("[DriverCreate]");

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest (Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

NTSTATUS STDCALL DriverClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{

    DbgPrint("[DriverClose]");

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest (Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

NTSTATUS STDCALL DriverControl(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{

    DbgPrint("[DriverControl]");

    PIO_STACK_LOCATION irpStack = {0};
    irpStack = IoGetCurrentIrpStackLocation(Irp);
    int pid = irpStack->Parameters.DeviceIoControl.IoControlCode;
    PspTerminateProcess(pid);
    DbgPrint("[DriverControl][pid=%d]", pid);

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest (Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}


//获取(系统调用)函数地址
PVOID GetSystemCallAddress(wchar_t *str)
{

    PVOID addr;
    UNICODE_STRING pslookup;

    RtlInitUnicodeString (&pslookup, str);
    addr = (PUCHAR) MmGetSystemRoutineAddress(&pslookup);
    DbgPrint("[my_drv][GetSystemCallAddress][%S=%p]", str, addr);
    return addr;
}

//KillProcess地址
PVOID GetKillProcessAddress(void)
{

    ULONG size;
    PULONG buffer;
    ZwQuerySystemInformation(SystemModuleInformation, &size, 0, &size);
    if((buffer = (PULONG)ExAllocatePool(PagedPool, size)) == NULL) {

	DbgPrint("[my_drv][ExAllocatePool failed!]");
	return NULL;
    }

    NTSTATUS status = ZwQuerySystemInformation(SystemModuleInformation,buffer, size , 0);
    if(!NT_SUCCESS(status)) {

	DbgPrint("[my_drv][ZwQuerySystemInformation failed!]");
	return NULL;
    }

    PSYSTEM_MODULE_INFORMATION_ENTRY module = {0};
    module = (PSYSTEM_MODULE_INFORMATION_ENTRY)((PULONG)buffer + 1);
    ULONG AddrCur = (ULONG)module->Base;
    ULONG AddrMax = (ULONG)module->Base + (ULONG)module->Size;
    ExFreePool(buffer);

    ULONG code1 = 0x8b55ff8b;
    ULONG code2 = 0xa16456ec;
    ULONG code3 = 0x00000124;
    ULONG code4 = 0x3b08758b;

    for (; AddrCur < AddrMax; AddrCur++) {

	if ((*((ULONG *)AddrCur)) == code1) {

	    if ((*((ULONG *)(AddrCur + 4))) == code2) {

		if ((*((ULONG *)(AddrCur + 8))) == code3) {

		    if ((*((ULONG *)(AddrCur + 12))) == code4) {

			DbgPrint("[my_drv][AddrCur=%x]", AddrCur);
			return (PVOID)AddrCur;
		    }
		}
	    }
	}
    }

    return NULL;
}

//PspTerminateProcess函数实现
VOID PspTerminateProcess(int pid)
{

    //PsLookupProcessByProcessId(pid, &Process);
    typedef NTKERNELAPI
	NTSTATUS
	NTAPI
	(PSLOOKUPPROCESSBYPROCESSID)
	(PVOID ProcessId,
	 PEPROCESS *Process);

    PEPROCESS Process = NULL;
    PSLOOKUPPROCESSBYPROCESSID *PsLookupProcessByProcessIdCall;
    PVOID addr = GetSystemCallAddress(L"PsLookupProcessByProcessId");
    if (!addr)
	return;

    PsLookupProcessByProcessIdCall = (PSLOOKUPPROCESSBYPROCESSID *)addr;
    PsLookupProcessByProcessIdCall((PVOID)pid, &Process);
    DbgPrint("[my_drv][PsLookupProcessByProcessId=%p]", Process);

    //KillProcess(PEPROCESS, exitstatus);
    typedef NTSTATUS 
	NTKERNELAPI 
	(KILLPROCESS)
	(PEPROCESS Process,
	 NTSTATUS ExitStatus);

    KILLPROCESS *killprocess = NULL;
    killprocess = (KILLPROCESS *)GetKillProcessAddress();
    if (killprocess) {

	killprocess(Process, 0);
    }
}


