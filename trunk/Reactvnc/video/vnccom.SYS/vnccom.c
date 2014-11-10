#include "wdm.h"
//#pragma  comment(lib,"vncdrv.lib") 
typedef struct _MEMSTRUCT{
    ULONG dword_10A04;
    ULONG dword_10A08;
    ULONG dword_10A0C;
    ULONG dword_10A10;
    ULONG dword_10A14;
    ULONG dword_10A18;
    ULONG dword_10A1C;
    ULONG dword_10A20;
    USHORT  byte_10A24;
}MEMSTRUCT, *PMEMSTRUCT;
typedef struct _MEMTRUCTARRAY{
    ULONG dword_10A44;
    ULONG dword_10A48;
    ULONG dword_10A4C;
    ULONG dword_10A50;
    ULONG dword_10A54;
    ULONG dword_10A58;
    char byte_10A5C[125];
}MEMTRUCTARRAY, PMEMTRUCTARRAY;
extern MEMSTRUCT _declspec(dllimport) memstruct;
extern MEMTRUCTARRAY _declspec(dllimport) memtructarray;

//
//sub_1097A
//
NTSTATUS
Dispatch_Create(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    )
{
  //*(_DWORD *)((PIRP)pIrp + 24) = 0;
  //*(_DWORD *)((PIRP)pIrp + 28) = 0;
  pIrp->IoStatus.Status = 0;
  pIrp->IoStatus.Information = 0;
  IofCompleteRequest(pIrp, IO_NO_INCREMENT);
  //DbgPrint("IOCTL.SYS:");
  //DbgPrint((CHAR *)&dword_1095C[3] + 2);
  return STATUS_SUCCESS;	
}

//
//sub_1091E
//
NTSTATUS
Dispatch_Close(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    )
{
  pIrp->IoStatus.Status = 0;
  pIrp->IoStatus.Information = 0;
  IofCompleteRequest(pIrp, IO_NO_INCREMENT);
  /*
  if ( *((_BYTE *)memstruct + 36) == 1 )
  {
    DbgPrint(dword_10900);
    DbgPrint((CHAR *)dword_10910);
  }
  */

  return STATUS_SUCCESS;
}

//
//sub_10CF8
//
NTSTATUS
Dispatch_IoControl(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    )
{
  return STATUS_SUCCESS;
}

//
//sub_105E6
//
NTSTATUS
Dispatch_Cleanup(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    )
{
  pIrp->IoStatus.Status = 0;
  pIrp->IoStatus.Information = 0;
  //DbgPrint(word_105C6);
  //DbgPrint((CHAR *)&dword_105C8[3] + 2);
  //*((_BYTE *)memstruct + 36) = 1;
  memstruct.dword_10A14 = 1;
  //sub_104E8(99);
  //*((_BYTE *)memstruct + 36) = 0;
  IofCompleteRequest(pIrp, IO_NO_INCREMENT);
  
  return STATUS_SUCCESS;
}

//
//sub_109F6
//
VOID
DriverUnload(
    PDRIVER_OBJECT pDriverObject
    )
{
   DEVICE_OBJECT *fdo;
  UNICODE_STRING SymbolicLinkName; // [sp+8h] [bp-8h]@1

  fdo = pDriverObject->DeviceObject;
  RtlInitUnicodeString(&SymbolicLinkName, L"DosDevices\\VncIoctl");
  IoDeleteSymbolicLink(&SymbolicLinkName);
  if ( fdo )
    IoDeleteDevice(fdo);
  DbgPrint("SIOCTL.SYS: ");
  //DbgPrint((CHAR *)&word_109E6)
  return;
}

UCHAR byte_10884;
NTSTATUS
DriverEntry(
    PDRIVER_OBJECT  pDriverObject,
    PUNICODE_STRING pRegistryPath
    )
{
  NTSTATUS result; // eax@1
  NTSTATUS v4; // esi@2
  NTSTATUS v5; // eax@2
  PDEVICE_OBJECT DeviceObject; // [sp+20h] [bp-4h]@1
  UNICODE_STRING DestinationString; // [sp+18h] [bp-Ch]@1
  UNICODE_STRING SymbolicLinkName; // [sp+10h] [bp-14h]@2

  DeviceObject = 0;
  RtlInitUnicodeString(&DestinationString, L"\\Device\\VNCIOCTL");
  result = IoCreateDevice(pDriverObject, 0, &DestinationString, 0x22u, 0, 0, &DeviceObject);
  if ( result >= 0 )
  {
    DbgPrint((CHAR *)"SIOCTL.SYS: ");
    DbgPrint("create the device object\n");
    pDriverObject->MajorFunction[IRP_MJ_CREATE] = Dispatch_Create;
    pDriverObject->MajorFunction[IRP_MJ_CLOSE]  = Dispatch_Close;
    pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = Dispatch_IoControl;
    pDriverObject->MajorFunction[IRP_MJ_CLEANUP] = Dispatch_Cleanup;
    pDriverObject->DriverUnload = DriverUnload;
    RtlInitUnicodeString(&SymbolicLinkName, L"\\DosDevices\\VncIoctl");
    v5 = IoCreateSymbolicLink(&SymbolicLinkName, &DestinationString);
    v4 = v5;
    if ( v5 < 0 )
      IoDeleteDevice(DeviceObject);
    byte_10884 = 0;
    result = v4;
  }
  return result;
}
