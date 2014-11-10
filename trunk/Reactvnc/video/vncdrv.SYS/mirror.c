//#include "ntddk.h"
#include "dderror.h"
#include "devioctl.h"
#include "miniport.h"
#include "ntddvdeo.h"
#include "video.h"

#include "mirror.h"


VOID DbgBreakPoint() {};

//
//导出的全局变量
//
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
MEMSTRUCT _declspec(dllexport) memstruct;

typedef struct _MEMTRUCTARRAY{
    ULONG dword_10A44;
    ULONG dword_10A48;
    ULONG dword_10A4C;
    ULONG dword_10A50;
    ULONG dword_10A54;
    ULONG dword_10A58;
    char byte_10A5C[125];
}MEMTRUCTARRAY, PMEMTRUCTARRAY;
MEMTRUCTARRAY _declspec(dllexport) memtructarray;


VOID MirrorNotImplemented(char *s)
{
   VideoDebugPrint((0, "Mirror Sample: Not used '%s'.\n", s));
}

BOOLEAN
MirrorResetHW(
    PVOID HwDeviceExtension,
    ULONG Columns,
    ULONG Rows
    )
{
   MirrorNotImplemented("MirrorResetHW");
   
   return TRUE;
}

BOOLEAN
MirrorVidInterrupt(
    PVOID HwDeviceExtension
    )
{
   MirrorNotImplemented("MirrorVidInterrupt");

   return TRUE;
}

VP_STATUS
MirrorGetPowerState(
    PVOID                   HwDeviceExtension,
    ULONG                   HwId,
    PVIDEO_POWER_MANAGEMENT VideoPowerControl
    )
{
   MirrorNotImplemented("MirrorGetPowerState");

   return NO_ERROR;
}

VP_STATUS
MirrorSetPowerState(
    PVOID                   HwDeviceExtension,
    ULONG                   HwId,
    PVIDEO_POWER_MANAGEMENT VideoPowerControl
    )
{
   MirrorNotImplemented("MirrorSetPowerState");

   return NO_ERROR;
}

ULONG
MirrorGetChildDescriptor (
    IN  PVOID                  HwDeviceExtension,
    IN  PVIDEO_CHILD_ENUM_INFO ChildEnumInfo,
    OUT PVIDEO_CHILD_TYPE      pChildType,
    OUT PVOID                  pChildDescriptor,
    OUT PULONG                 pUId,
    OUT PULONG                 pUnused
    )
{
   MirrorNotImplemented("MirrorGetChildDescriptor");

   return ERROR_NO_MORE_DEVICES;
}

VP_STATUS MirrorFindAdapter(
   IN PVOID HwDeviceExtension,
   IN PVOID HwContext,
   IN PWSTR ArgumentString,
   IN OUT PVIDEO_PORT_CONFIG_INFO ConfigInfo,
   OUT PUCHAR Again
   )
{
   VideoDebugPrint((0,"FindAdapter Called.\n"));
 
   return NO_ERROR;
}

BOOLEAN MirrorInitialize(
   PVOID HwDeviceExtension
   )
{
   VideoDebugPrint((0,"Initialize Called.\n"));

   return TRUE;
}


ULONG sub_107E6(PMEMSTRUCT pmemstruct)
{
   //ExAllocatePoolWithTag(0, 3, 'DDK');
   return 1;
}


LONG sub_10480()
{
    LONG result = 0;
    return result;
}

LONG sub_104B8()
{
	return 1;
}

BOOLEAN MirrorStartIO(
   PVOID HwDeviceExtension,
   PVIDEO_REQUEST_PACKET RequestPacket
   )
{
   PVIDEO_REQUEST_PACKET pRequestPacket; 
   PVOID pInputBuffer;
   ULONG v9;
   LONG v6;	

   VideoDebugPrint((0,"StartIO Called.\n"));

   pRequestPacket = RequestPacket;
   memstruct.byte_10A24 = 1;
   if(pRequestPacket->IoControlCode == IOCTL_VIDEO_MAP_VIDEO_MEMORY/*2294872*/)
   {
	//100011 00 0 00100010110 00     2294872
	//#define CTL_CODE( DeviceType, Function, Method, Access ) (             \
    	//((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
	//#define IOCTL_VIDEO_MAP_VIDEO_MEMORY \
        //CTL_CODE(FILE_DEVICE_VIDEO, 0x116, METHOD_BUFFERED, FILE_ANY_ACCESS)
       	pInputBuffer = pRequestPacket->InputBuffer;
	memstruct.dword_10A10 = *((ULONG *)pInputBuffer+16); 
	memstruct.dword_10A14 = *((ULONG *)pInputBuffer+20); 
	memstruct.dword_10A18 = *((ULONG *)pInputBuffer+24);
	v9 =  *((ULONG *)pInputBuffer+4);
	memstruct.dword_10A04 = *((ULONG *)pInputBuffer+4); 
	memstruct.dword_10A0C = *((ULONG *)pInputBuffer+12); 
	memstruct.dword_10A1C = *((ULONG *)pInputBuffer+28); 
	memstruct.dword_10A20 = *((ULONG *)pInputBuffer+32); 
	if(!v9)
	{
	    sub_107E6(&memstruct);	
	    v6 = sub_10480();
	    if(v6 == 99)
	    {
                    memstruct.byte_10A24 = 0;
		    return TRUE;
	    }
	}	
   }
   if(pRequestPacket->IoControlCode == IOCTL_VIDEO_UNMAP_VIDEO_MEMORY/*2294876*/)
   {

   }

   return TRUE;
}



//
//导出的函数
//
ULONG
DriverEntry (
    PVOID Context1,
    PVOID Context2
    )
{
    VIDEO_HW_INITIALIZATION_DATA hwInitData;
    ULONG initializationStatus;

    VideoDebugPrint((0, "Mirrored Driver VideoPort [Driver Entry]\n"));

    // Zero out structure.

    VideoPortZeroMemory(&hwInitData, sizeof(VIDEO_HW_INITIALIZATION_DATA));

    // Specify sizes of structure and extension.

    hwInitData.HwInitDataSize = sizeof(VIDEO_HW_INITIALIZATION_DATA);

    // Set entry points.

    //sub_10554
    hwInitData.HwFindAdapter             = &MirrorFindAdapter;
    
    //sub_1055A
    hwInitData.HwInitialize              = &MirrorInitialize;

    //sub_10560
    hwInitData.HwStartIO                 = &MirrorStartIO;

    //sub_10540
    hwInitData.HwResetHw                 = &MirrorResetHW;

    //sub_1055A
    hwInitData.HwInterrupt               = &MirrorInitialize; //&MirrorVidInterrupt;

    //sub_10546
    hwInitData.HwGetPowerState           = &MirrorGetPowerState;
    hwInitData.HwSetPowerState           = &MirrorSetPowerState;

    //sub_1054C
    hwInitData.HwGetVideoChildDescriptor = &MirrorGetChildDescriptor;

    hwInitData.HwLegacyResourceList      = NULL; 
    hwInitData.HwLegacyResourceCount     = 0; 

    // no device extension necessary
    hwInitData.HwDeviceExtensionSize = 0;
    hwInitData.AdapterInterfaceType = 0;

    initializationStatus = VideoPortInitialize(Context1,
                                               Context2,
                                               &hwInitData,
                                               NULL);

    return initializationStatus;

} // end DriverEntry()

