#pragma once
#ifdef __cplusplus
extern "C"
	{
#endif
#include <NTDDK.h> //这里包含需要用C方式编译的头文件
#ifdef __cplusplus
	}
#endif

BOOL ssdthook_flag = FALSE; //当前是否进行了hook的标志
ULONG  RealNtOpenAddress; 	//hook前的NtOpenAddress地址
HANDLE hookPID; 			//要hook的进程id

typedef struct _ServiceDescriptorTable {
	PVOID ServiceTableBase;					//System Service Dispatch Table 的基地址  
	PVOID ServiceCounterTable;				//包含着 SSDT 中每个服务被调用次数的计数器。这个计数器一般由sysenter 更新。 
	unsigned int NumberOfServices;			//由 ServiceTableBase 描述的服务的数目。  
	PVOID ParamTableBase;					//包含每个系统服务参数字节数表的基地址-系统服务参数表 
} *PServiceDescriptorTable;  
extern "C" extern PServiceDescriptorTable KeServiceDescriptorTable;

// 构建自定义NtOpenProcess函数指针
extern "C" typedef NTSTATUS __stdcall NTOPENPROCESS ( OUT PHANDLE ProcessHandle, 
													IN ACCESS_MASK AccessMask, 
													IN POBJECT_ATTRIBUTES ObjectAttributes, 
													IN PCLIENT_ID ClientId 
													);
NTOPENPROCESS *RealNtOpenProcess; 
PEPROCESS EP;

// 自定义的NtOpenProcess函数MyNtOpenProcess
extern "C" NTSTATUS __stdcall MyNtOpenProcess(OUT PHANDLE ProcessHandle, IN  ACCESS_MASK DesiredAccess, IN  POBJECT_ATTRIBUTES ObjectAttributes, IN PCLIENT_ID ClientId ) { 
	NTSTATUS rc; 
	HANDLE PID; 
	KdPrint(("进入了MyNtOpenProcess"));  	
	if(ClientId != NULL) { 
		PID = ClientId->UniqueProcess; 	 
		KdPrint(( "进程PID=%d\n",(int*)PID ));
		if(PID == hookPID) {			//判断是否为需要保护的进程pid
			KdPrint(("%d是被保护进程 hookPID=%d \n",(int)PID, (int)hookPID));
			KdPrint(("进程正在保护,NtOpenProcess失败!!!!!\n"));
			ProcessHandle = NULL;
			rc = STATUS_ACCESS_DENIED;	//拒绝访问
			EP = PsGetCurrentProcess();	//也可使用PsLookupProcessByProcessId((ULONG)PID,&EP);		 
			KdPrint(("进程名:%s\n",(PTSTR)((ULONG)EP+0x174)));
		} else {
			KdPrint(("%d不是被保护进程 hookPID=%d \n",(int)PID, (int)hookPID));
			rc = (NTSTATUS)RealNtOpenProcess(ProcessHandle, DesiredAccess, ObjectAttributes, ClientId); 
		}
	} 
	return rc; 
} 

// HOOK 函数构建
VOID HookNtOpenProcess() { 
	ssdthook_flag=true;
 	LONG PSSDT_Addr, *PNtOpenProcessr_Addr, NtOpenProcessr_Addr; 

	PSSDT_Addr=(LONG)KeServiceDescriptorTable->ServiceTableBase;
	PNtOpenProcessr_Addr=(PLONG)(PSSDT_Addr+0x7A*4);
	NtOpenProcessr_Addr=*PNtOpenProcessr_Addr;	 
	RealNtOpenAddress = *PNtOpenProcessr_Addr; 
	RealNtOpenProcess = (NTOPENPROCESS *)RealNtOpenAddress; 

	KdPrint(("真实的NtOpenProcess地址: %x\n",(int)RealNtOpenAddress)); 
	KdPrint(("伪造NTOpenProcess地址: %x\n", (int)MyNtOpenProcess));  
	__asm
	{
		 cli
			mov eax,cr0
			and eax,not 10000h
			mov cr0,eax
	}

	// 修改SSDT 
	*PNtOpenProcessr_Addr= (ULONG)MyNtOpenProcess; 
	KdPrint(("UnHook修改NTOpenProcess OK \n")); 
		
	__asm 
	{ 
		  mov  eax, cr0 
		  or  eax, 10000h 
		  mov  cr0, eax 
		  sti 
	}   
	return;
} 

//UnHook函数构建
VOID UnHookNtOpenProcess() { 
	ULONG PNtOpenProcessr_Addr; 
	PNtOpenProcessr_Addr = (ULONG)KeServiceDescriptorTable->ServiceTableBase + 0x7A * 4; 
	if (ssdthook_flag) {
		ssdthook_flag=false;
		__asm 
		{ 
			cli 
			mov  eax, cr0 
			and  eax, not 10000h 
			mov  cr0, eax 
		} 

		// 还原SSDT 
		*((ULONG*)PNtOpenProcessr_Addr) = (ULONG)RealNtOpenAddress; 
		KdPrint(("UnHook还原NTOpenProcess OK \n")); 
		
		__asm 
		{ 
			mov  eax, cr0 
			or  eax, 10000h 
			mov  cr0, eax 
			sti 
		} 
	}
	return;
}
