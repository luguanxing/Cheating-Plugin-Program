# 04简单DLL注入游戏

------

通过C语言编写一个DLL文件和一个EXE文件，其中DLL包含作弊功能，运行EXE后将DLL注入游戏，使得游戏仅靠自身进程便可以实现作弊功能

> * 对象分析
> * 要用的API函数简单介绍
> * 编写测试效果
> * 总体评价


------

## 对象分析
![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/01%E4%B8%80%E4%B8%AA%E7%AE%80%E5%8D%95%E7%9A%84%E5%86%85%E5%AD%98%E5%A4%96%E6%8C%82/pictures/0.jpg?raw=true)<br>
<br>
本次游戏对象为Super Mario XP<br>
没有更新所以可用任意版本<br>
试玩发现人物血量最大为10，心最大为99，命最大为99<br><br><br><br>


## 要用的API函数简单介绍
HANDLE **CreateThread**(LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE, LPVOID, DWORD ,LPDWORD)<br>
CreateThread将在主线程的基础上创建一个新线程
<br><br>

LPVOID **VirtualAllocEx**(HANDLE, LPVOID, SIZE_T, DWORD, DWORD);<br>
向指定进程申请内存，其中flAllocationType取值MEM_COMMIT表示写入物理存储而非磁盘交换内存
<br><br>

FARPROC **GetProcAddress**(HMODULE hModule, LPCSTR);<br>
检索指定的动态链接库(DLL)中的输出库函数地址
<br><br>

HANDLE **CreateRemoteThread**(HANDLE, LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE, LPVOID, DWORD, LPDWORD);<br>
创建一个在其它进程地址空间中运行的线程
<br><br><br><br>




## 编写测试效果

![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/04%E7%AE%80%E5%8D%95DLL%E6%B3%A8%E5%85%A5%E6%B8%B8%E6%88%8F/pictures/01.jpg?raw=true)<br>
**打开游戏**<br><br><br><br>

![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/04%E7%AE%80%E5%8D%95DLL%E6%B3%A8%E5%85%A5%E6%B8%B8%E6%88%8F/pictures/02.jpg?raw=true)<br>
**运行外挂**<br>
打开注入器Injecter，注入器注入DLL后自动退出，仅剩下游戏，此时游戏已具备作弊效果(锁定血量)<br><br><br><br>



![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/04%E7%AE%80%E5%8D%95DLL%E6%B3%A8%E5%85%A5%E6%B8%B8%E6%88%8F/pictures/03.jpg?raw=true)<br>
**检测有效**<br><br><br><br>
```
//
//  04简单DLL注入游戏(作弊模块DLL部分)
//  C/C++
//
//  Created by luguanxing.
//  Copyright @2016 LGX. All rights reserved.
//
#include <windows.h>
#define Dllfunciton extern "C" __declspec(dllexport)	//以C方式导出

Dllfunciton void lockdata();
Dllfunciton DWORD WINAPI inject(LPVOID);

void lockdata() {
	while (true) {
		DWORD hp = 10;
		DWORD heart = 99;
		DWORD life = 99;

		DWORD addr = 0x00428282;
		DWORD addr2 = 0x00428292;
		DWORD addr3 = 0x004282a2;
  
		DWORD res = WriteProcessMemory(INVALID_HANDLE_VALUE, (LPVOID)addr, &hp, 4, 0);	//写入自身修改游戏数据
		DWORD res2 = WriteProcessMemory(INVALID_HANDLE_VALUE, (LPVOID)addr2, &heart, 4, 0);
		DWORD res3 = WriteProcessMemory(INVALID_HANDLE_VALUE, (LPVOID)addr3, &life, 4, 0);

		Sleep(1000);
	}
}

DWORD WINAPI inject(LPVOID) {
	lockdata();
	return true;
}

BOOL APIENTRY DllMain(HMODULE hModule,  DWORD  ul_reason_for_call, LPVOID lpReserved) {
	switch(ul_reason_for_call) {
		case DLL_PROCESS_ATTACH: {
			::DisableThreadLibraryCalls(hModule);	//创建线程包含死循环，为防卡死必须设置
			CreateThread(NULL, 0, inject, NULL, 0, NULL);
		}
		break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
		default:;
	}
    return true;
}
```
<br><br><br><br>
```
//
//  04简单DLL注入游戏(注入器EXE部分)
//  C/C++
//
//  Created by luguanxing.
//  Copyright @2016 LGX. All rights reserved.
//
#include <windows.h>
#include <string.h>
#include <string>
#include <iostream>
using namespace std;

HWND hwnd = NULL;
DWORD processid = NULL;
HANDLE hprocess = NULL;
PVOID procdlladdr = NULL;

char dllname[25] = "cheatDLL";
char loadfunc[25] = "LoadLibraryA";
FARPROC loadfuncaddr = NULL;
HANDLE hfile;

void getwindow() {
    hwnd = ::FindWindow(NULL, "Super Mario XP");
	if (hwnd == NULL)
		MessageBox(NULL, "找不到游戏", "错误",  MB_OK);
    GetWindowThreadProcessId(hwnd, &processid);
    hprocess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,processid);
	if (hprocess == NULL)
		MessageBox(NULL, "打开游戏失败", "错误", MB_OK);
}


void inject() {
	int size = strlen(dllname)+5;
	procdlladdr = ::VirtualAllocEx(hprocess, NULL, size, MEM_COMMIT, PAGE_READWRITE);	//向目标申请空间，得到新空间地址
	if (procdlladdr == NULL)
		MessageBox(NULL, "申请空间失败", "错误", MB_OK);
	DWORD writenum;
	::WriteProcessMemory(hprocess, procdlladdr, dllname, size, &writenum);	//向新空间写入要注入的DLL名称
	loadfuncaddr = ::GetProcAddress(::GetModuleHandle("kernel32.dll"), loadfunc);	//获得LoadLibraryA的地址,在任何进程空间都一样
	HANDLE hthread = ::CreateRemoteThread(hprocess, NULL, 0, (LPTHREAD_START_ROUTINE)loadfuncaddr, (LPVOID)procdlladdr, 0, NULL);
	//新建线程执行LoadLibrary参数是已在目标进程新空间写入的DLL名称,注意这个函数在64位下无法成功
	::WaitForSingleObject(hthread, INFINITE); 
	::CloseHandle(hthread);
	::CloseHandle(hprocess);
}

int main() {
	getwindow();
	inject();
	return 0;
}
```
<br><br><br>



## 总体评价

DLL注入可使作弊模块在程序自身“名义”下进行作弊，提高作弊成功率，然而载入的DLL容易被DLL检测发现
