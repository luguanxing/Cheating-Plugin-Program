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