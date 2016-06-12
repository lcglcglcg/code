#include <ddk/ntddk.h>
#include <ddk/ntifs.h>
#include <ddk/ntapi.h>

#define	code1(addr) ((*((ULONG *)addr)) == 0x8b55ff8b)
#define	code2(addr) ((*((ULONG *)(addr + 4))) == 0xa16456ec)
#define	code3(addr) ((*((ULONG *)(addr + 8))) == 0x00000124)
#define	code4(addr) ((*((ULONG *)(addr + 12))) == 0x3b08758b)

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
    RtlInitUnicodeString(&devNameUnicd,L"\\Device\\mydev");
    RtlInitUnicodeString(&devLinkUnicd,L"\\??\\mydev");

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
    RtlInitUnicodeString(&devLinkUnicd,L"\\??\\mydev");

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
    DbgPrint("[DriverControl][pid=%d]", pid);
    PspTerminateProcess(pid);

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
    DbgPrint("[GetSystemCallAddress][%S=%p]", str, addr);
    return addr;
}

//KillProcess地址
PVOID GetKillProcessAddress(void)
{

    ULONG size;
    PULONG buffer;
    ZwQuerySystemInformation(SystemModuleInformation, &size, 0, &size);
    if((buffer = (PULONG)ExAllocatePool(PagedPool, size)) == NULL) {

	DbgPrint("[ExAllocatePool failed!]");
	return NULL;
    }

    NTSTATUS status = ZwQuerySystemInformation(SystemModuleInformation,buffer, size , 0);
    if(!NT_SUCCESS(status)) {

	DbgPrint("[ZwQuerySystemInformation failed!]");
	return NULL;
    }

    PSYSTEM_MODULE_INFORMATION_ENTRY module = {0};
    module = (PSYSTEM_MODULE_INFORMATION_ENTRY)((PULONG)buffer + 1);
    ULONG AddrCur = (ULONG)module->Base;
    ULONG AddrMax = (ULONG)module->Base + (ULONG)module->Size;
    ExFreePool(buffer);

    for (; AddrCur < AddrMax; AddrCur++) {


	if (code1(AddrCur) && code2(AddrCur) && code3(AddrCur) && code4(AddrCur)) {

	    DbgPrint("[GetKillProcessAddress][addr=%x]", AddrCur);
	    return (PVOID)AddrCur;
	}
    }

    DbgPrint("[GetKillProcessAddress][addr=NULL]");
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
    DbgPrint("[PsLookupProcessByProcessId=%p]", Process);

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


