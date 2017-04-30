#ifndef   CTL_CODE
	#pragma message("\n \n-----------in EXE Include winioctl.h ")
	#include<winioctl.h> //CTL_CODE ntddk.h wdm.h
#else 
	#pragma message("-\n \n---------in SYS NO Include winioctl.h ")
#endif
 
#define add_code	 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED,FILE_ANY_ACCESS)
#define sub_code     CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED,FILE_ANY_ACCESS)
#define hook_code    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED,FILE_ANY_ACCESS)
#define unhook_code  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED,FILE_ANY_ACCESS)
