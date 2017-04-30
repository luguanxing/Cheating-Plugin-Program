# 07简单进程保护

------
通过编写简单的进程hook保护驱动并运行，分析如何解除驱动保护

> * 设计思路分析
> * 对象分析
> * 要用的函数介绍
> * 编写测试效果
> * 总体评价

------

<br><br>
## 设计思路分析
1、通过编写简单的进程hook保护驱动并运行，用以保护记事本进程<br>
2、分析如何过保护<br>
<br><br><br>

## 对象分析
![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/07%E7%AE%80%E5%8D%95%E8%BF%9B%E7%A8%8B%E4%BF%9D%E6%8A%A4/pictures/demo.gif?raw=true)<br>
运行自写程序HookProcess.exe选择要保护的进程(记事本)，自动加载同目录下的驱动hook.sys并进行驱动保护<br>
<br><br><br>

## 主要函数介绍
**NtOpenProcess**<br>
驱动层函数，用于打开进程，原型参考如下<br>

NTSTATUS ZwOpenProcess(<br>
    _Out_    PHANDLE            ProcessHandle,    //句柄指针<br>
    _In_     ACCESS_MASK        DesiredAccess,    //渴望得到的访问权限（标志）<br>
    _In_     POBJECT_ATTRIBUTES ObjectAttributes, //属性结构指针<br>
    _In_opt_ PCLIENT_ID         ClientId          //进程标示符结构指针<br>
);<br>
<br><br><br>




## 编写测试效果

![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/07%E7%AE%80%E5%8D%95%E8%BF%9B%E7%A8%8B%E4%BF%9D%E6%8A%A4/pictures/1.jpg?raw=true)<br>
![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/07%E7%AE%80%E5%8D%95%E8%BF%9B%E7%A8%8B%E4%BF%9D%E6%8A%A4/pictures/2.jpg?raw=true)<br>
**运行保护程序保护记事本，使用CE(非内核模式)无法打开记事本，OD(无插件)也检测不到记事本进程**<br><br><br><br><br><br>

![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/07%E7%AE%80%E5%8D%95%E8%BF%9B%E7%A8%8B%E4%BF%9D%E6%8A%A4/pictures/3.jpg?raw=true)<br>
**用工具检测SSDT发现NtOpenProcess确实被HOOK了**<br><br><br><br><br><br>

![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/07%E7%AE%80%E5%8D%95%E8%BF%9B%E7%A8%8B%E4%BF%9D%E6%8A%A4/pictures/4.jpg?raw=true)<br>
**这时选择被HOOK内核函数NtOpenProcess把它恢复就可以了**<br><br><br><br><br><br>

![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/07%E7%AE%80%E5%8D%95%E8%BF%9B%E7%A8%8B%E4%BF%9D%E6%8A%A4/pictures/6.jpg?raw=true)<br>
**也可以反汇编当前被HOOK地址，让他跳回正常内核函数NtOpenProcess原地址**<br><br><br><br><br><br>

![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/07%E7%AE%80%E5%8D%95%E8%BF%9B%E7%A8%8B%E4%BF%9D%E6%8A%A4/pictures/7.jpg?raw=true)<br>
![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/07%E7%AE%80%E5%8D%95%E8%BF%9B%E7%A8%8B%E4%BF%9D%E6%8A%A4/pictures/5.jpg?raw=true)<br>
**恢复后CE能扫描，且OD也能检测到进程了**<br><br><br><br><br><br>


```
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
```
<br><br><br>

```
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
```
<br><br><br>



## 总体评价

本驱动保护比较简单，很容易就会被OD插件解除，仅用于学习原理，在windowsXP下测试成功，EXE由MFC编写可能部分有不完美之处，控制台可参考<br>
https://github.com/luguanxing/Kernel-Driver/tree/master/10-%E7%AE%80%E5%8D%95%E8%BF%9B%E7%A8%8B%E4%BF%9D%E6%8A%A4
