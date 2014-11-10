///
/// @file	shdpci_filter.c
/// @author	yee
/// @date	2009-11-30
///

#include <ntddk.h>
#include <stdio.h>
#define MAX_SHDPCI 64
#define PLX_DRIVER_NAME_UNICODE L"Pci9050"
static PDRIVER_OBJECT filter_driverob[MAX_SHDPCI];
static PDRIVER_OBJECT native_driverob[MAX_SHDPCI];

//
//	ͨ���������ӻ�ȡ�豸����
//
PDRIVER_OBJECT FilterOpenAudioBoard(ULONG num, NTSTATUS *status)	
{
	static WCHAR devicename[32];
	UNICODE_STRING uni_devicename;
	PFILE_OBJECT fileobj = NULL;
	PDEVICE_OBJECT devobj = NULL;

	swprintf(devicename,	
		L"\\Device\\" PLX_DRIVER_NAME_UNICODE L"-%d",
        	num);
	RtlInitUnicodeString(&uni_devicename, devicename);
	
	*status = IoGetDeviceObjectPointer(&uni_devicename, FILE_ALL_ACCESS, &fileobj, &devobj);
	if (*status == STATUS_SUCCESS)
		ObDereferenceObject(fileobj);

	return devobj;
}


//
//	������Ҫattach�豸�Ķ�дģʽ
//	���ݱ������豸�����²����������ڶ�̬ɾ����������ʱ����
//
NTSTATUS FilterAttachOneAudioBoard(PDRIVER_OBJECT driver, PDEVICE_OBJECT oldobj, PDEVICE_OBJECT *fltobj, PDEVICE_OBJECT *next)
{
	NTSTATUS status;
	PDEVICE_OBJECT topdev = NULL;

	status = IoCreateDevice(driver,	0, NULL, oldobj->DeviceType, 0, FALSE, fltobj);

	if (status != STATUS_SUCCESS)
		return status;

	if(oldobj->Flags & DO_BUFFERED_IO)
		(*fltobj)->Flags |= DO_BUFFERED_IO;
	if(oldobj->Flags & DO_DIRECT_IO)
		(*fltobj)->Flags |= DO_DIRECT_IO;
	if(oldobj->Flags & DO_BUFFERED_IO)
		(*fltobj)->Flags |= DO_BUFFERED_IO;
	if(oldobj->Characteristics & FILE_DEVICE_SECURE_OPEN)
		(*fltobj)->Characteristics |= FILE_DEVICE_SECURE_OPEN;
	(*fltobj)->Flags |=  DO_POWER_PAGABLE;

	topdev = IoAttachDeviceToDeviceStack(*fltobj,oldobj);
	if (topdev == NULL)
	{
		IoDeleteDevice(*fltobj);
		*fltobj = NULL;
		status = STATUS_UNSUCCESSFUL;
		return status;
	}
	*next = topdev;

	(*fltobj)->Flags = (*fltobj)->Flags & ~DO_DEVICE_INITIALIZING;
	return STATUS_SUCCESS;
}


//
// attach����Ŀ���豸
// FilterOpenAudioBoard��������ȡĿ���豸ָ��
// FilterAttachOneAudioBoard������attachĿ���豸
//
NTSTATUS FilterAttachAllAudioBoard(PDRIVER_OBJECT driver)
{
	ULONG i;
        PDEVICE_OBJECT audioborad_ob;
	NTSTATUS status;

	for(i=0;i<MAX_SHDPCI;i++)
	{
		audioborad_ob = FilterOpenAudioBoard(i, &status);
		if(audioborad_ob == NULL)
			continue;

		FilterAttachOneAudioBoard(driver, audioborad_ob, &filter_driverob[i], &native_driverob[i]);
	}	
}


//
// ����IRP 
//
NTSTATUS FilterDispatch(PDRIVER_OBJECT pdriverob, PIRP pirp)
{
	PIO_STACK_LOCATION irpsp = IoGetCurrentIrpStackLocation(pirp);
	NTSTATUS status;
	ULONG i;

	for(i=0;i<MAX_SHDPCI;i++)
	{
		if(pdriverob == filter_driverob[i])
		{
 			if(irpsp->MajorFunction == IRP_MJ_POWER)
            		{
                                PoStartNextPowerIrp(pirp);
                		IoSkipCurrentIrpStackLocation(pirp);
                		return PoCallDriver(native_driverob[i],pirp);
            		}

			if(irpsp->MajorFunction == IRP_MJ_CREATE)
			{
				DbgPrint("FilterDispatch:IRP_MJ_DEVICE_CONTROL");
			}
				
			IoSkipCurrentIrpStackLocation(pirp);
			return IoCallDriver(native_driverob[i], pirp);
		}
	}

	pirp->IoStatus.Information = 0;
    	pirp->IoStatus.Status = STATUS_INVALID_PARAMETER;
    	IoCompleteRequest(pirp,IO_NO_INCREMENT);
    	return STATUS_SUCCESS;	

}


//
// ֹͣ�豸��ע��ѭ���ӳ�
//
void FilterUnload(PDRIVER_OBJECT pdriverob)
{
	ULONG i;
	LARGE_INTEGER interval;

	for(i=0;i<MAX_SHDPCI;i++)
	{
		if(native_driverob[i] != NULL)
			IoDetachDevice(native_driverob[i]);
	}

	interval.QuadPart = (5*1000 * 1000);		
	KeDelayExecutionThread(KernelMode,FALSE,&interval);

	for(i=0;i<MAX_SHDPCI;i++)
	{
		if(filter_driverob[i] != NULL)
			IoDeleteDevice(filter_driverob[i]);
	}
}


NTSTATUS DriverEntry(PDRIVER_OBJECT pdriverob, PUNICODE_STRING pregpath)
{
	ULONG i;
	for(i=0;i<IRP_MJ_MAXIMUM_FUNCTION;i++)
		pdriverob->MajorFunction[i] = FilterDispatch;
	
	pdriverob->DriverUnload = FilterUnload;

	FilterAttachAllAudioBoard(pdriverob);

	return STATUS_SUCCESS;
}
