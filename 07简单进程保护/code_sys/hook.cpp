#pragma once
#ifdef __cplusplus
extern "C"
	{
#endif
#include <NTDDK.h> //这里包含需要用C方式编译的头文件
#ifdef __cplusplus
	}
#endif
#include <windef.h>
#include "hook.h"
#include "ctl_code.h"
#define INITCODE code_seg("INIT")	//指的代码运行后 就从内存释放掉
#define PAGECODE code_seg("PAGE")	//表示内存不足时，可以被置换到硬盘							

NTSTATUS CreateMyDevice(IN PDRIVER_OBJECT pDriverObject) {
	NTSTATUS status;
	UNICODE_STRING devName;				//设备名称
	UNICODE_STRING sysLinkName;			//系统符号链接名
	PDEVICE_OBJECT pDevObject;				//用于返回创建设备

	RtlInitUnicodeString(&devName, L"\\Device\\MyDevObj");
	status = IoCreateDevice(pDriverObject, 0, &devName, FILE_DEVICE_UNKNOWN, 0, TRUE, &pDevObject);
	if (!NT_SUCCESS(status)) {						//判断创建设备是否成功
		if (status == STATUS_INSUFFICIENT_RESOURCES)
			KdPrint(("资源不足\n"));
		if (status == STATUS_OBJECT_NAME_EXISTS)
			KdPrint(("指定对象名存在\n"));
		if (status == STATUS_OBJECT_NAME_COLLISION)
			KdPrint(("对象名有冲突\n"));
		return status;
	}
	KdPrint(("设备创建成功\n"));

	pDevObject->Flags |= DO_BUFFERED_IO;	//缓冲区方式读写
	RtlInitUnicodeString(&sysLinkName, L"\\??\\MySysLinkName_20170411");
	IoDeleteSymbolicLink(&sysLinkName);		//防止已有相同符号链接重复
	status = IoCreateSymbolicLink(&sysLinkName, &devName);		//判断生成符号链接是否成功

	if (!NT_SUCCESS(status)) {
		KdPrint(("生成符号链接失败\n"));
		IoDeleteDevice(pDevObject);
		return status;
	}
	KdPrint(("生成符号链接成功\n"));
	return STATUS_SUCCESS;
}



VOID DriverUnload(PDRIVER_OBJECT pDriverObject) {
	PDEVICE_OBJECT pDevObject;
	UNICODE_STRING sysLinkName;

	pDevObject = pDriverObject->DeviceObject;
	IoDeleteDevice(pDevObject);	//取得设备并删除
	KdPrint(("成功删除设备\n"));

	RtlInitUnicodeString(&sysLinkName, L"\\??\\MySysLinkName_20170411");
	IoDeleteSymbolicLink(&sysLinkName);	//取得符号链接并删除
	KdPrint(("成功删除符号链接\n"));

	UnHookNtOpenProcess();
	KdPrint(("驱动成功卸载\n"));
}



NTSTATUS MyDispatchRoutine(IN PDEVICE_OBJECT pDevobj,IN PIRP pIrp) {		//也可分开用不同函数处理例程
	ULONG info;
	PIO_STACK_LOCATION psl=IoGetCurrentIrpStackLocation(pIrp);
	switch (psl->MajorFunction) {
	   case IRP_MJ_CREATE:
		   break;
	   case IRP_MJ_CLOSE:
		   break;
	   case IRP_MJ_READ:
		   break;
	   case IRP_MJ_WRITE:
		   break;
	   case  IRP_MJ_DEVICE_CONTROL: {
				NTSTATUS status = STATUS_SUCCESS;
				ULONG cbin = psl->Parameters.DeviceIoControl.InputBufferLength;		//获取输入缓冲区大小
				ULONG cbout = psl->Parameters.DeviceIoControl.OutputBufferLength;	//获取输出缓冲区大小
				ULONG code = psl->Parameters.DeviceIoControl.IoControlCode;			//得到IOCTL码
				KdPrint(("Enter IRP_MJ_DEVICE_CONTROL\n"));
				switch(code) {
				   case add_code: {
						int a, b, r;
						int *inputBuffer = (int*)pIrp->AssociatedIrp.SystemBuffer;	//获取输入缓冲区数据
						int *outputBuffer = (int*)pIrp->AssociatedIrp.SystemBuffer;	//获取输出缓冲区数据
						KdPrint(("Enter add_code\n"));
						_asm {							//汇编方式取出a,b值,等价于a = *inputBuffer; b = *(inputBuffer+1);
							mov eax, inputBuffer
							mov ebx, [eax]
							mov a, ebx
							mov ebx, [eax+4]
							mov b, ebx
						}
						KdPrint(("a=%d, b=%d\n",a,b));
						r = a + b;
						_asm {							//汇编方式保存r值,等价于*inputBuffer = r;
							mov eax, r
							mov ebx, outputBuffer
							mov [ebx], eax
						}
						KdPrint(("a+b=%d",r));
						info = 4;
					} break;
				   case sub_code: {
						int a, b, r;
						int *inputBuffer = (int*)pIrp->AssociatedIrp.SystemBuffer;	//获取输入缓冲区数据
						int *outputBuffer = (int*)pIrp->AssociatedIrp.SystemBuffer;	//获取输出缓冲区数据
						KdPrint(("Enter sub_code\n"));
						_asm {							//汇编方式取出a,b值,等价于a = *inputBuffer; b = *(inputBuffer+1);
							mov eax, inputBuffer
							mov ebx, [eax]
							mov a, ebx
							mov ebx, [eax+4]
							mov b, ebx
						}
						KdPrint(("a=%d, b=%d\n",a,b));
						r = a - b;
						_asm {							//汇编方式保存r值,等价于*inputBuffer = r;
							mov eax, r
							mov ebx, outputBuffer
							mov [ebx], eax
						}
						KdPrint(("a-b=%d",r));
						info = 4;
					} break;
				   case hook_code: {
					   //从缓冲区获取hookPID
						int *inputBuffer = (int*)pIrp->AssociatedIrp.SystemBuffer;	//获取输入缓冲区数据
						int *outputBuffer = (int*)pIrp->AssociatedIrp.SystemBuffer;	//获取输出缓冲区数据
						_asm {						
							mov eax, inputBuffer
							mov ebx, [eax]
							mov hookPID, ebx
						}
						KdPrint(("传入参数hookPID=%d",hookPID));
						_asm {							
							mov eax, 1
							mov ebx, outputBuffer
							mov [ebx], eax
						}
						HookNtOpenProcess();
						info = 4;
					} break;
				   case unhook_code: {
					   UnHookNtOpenProcess();
					} break;
				}
			}
		   break;
	   default:
	 KdPrint(("其它处理")); 
	}

	pIrp->IoStatus.Information=info;			//设置操作的字节数为0，这里无实际意义
	pIrp->IoStatus.Status=STATUS_SUCCESS;		//返回成功
	IoCompleteRequest(pIrp,IO_NO_INCREMENT);	//指示完成此IRP
	KdPrint(("离开派遣函数\n"));				//调试信息
	return STATUS_SUCCESS;
}





#pragma INITCODE
extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING reg_path) {
	KdPrint(("驱动成功加载\n"));
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = MyDispatchRoutine;	//创建派遣例程，也可分开用不同函数
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = MyDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_READ] = MyDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_WRITE] = MyDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = MyDispatchRoutine;
	CreateMyDevice(pDriverObject);
	pDriverObject->DriverUnload=DriverUnload;
	return STATUS_SUCCESS;
}