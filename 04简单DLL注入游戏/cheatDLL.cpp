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

void lockdata() {	//锁定人物数值功能
	while (true) {
		DWORD hp = 10;
		DWORD heart = 99;
		DWORD life = 99;

		DWORD addr = 0x00428282;
		DWORD addr2 = 0x00428292;
		DWORD addr3 = 0x004282a2;
  
		DWORD res = WriteProcessMemory(INVALID_HANDLE_VALUE, (LPVOID)addr, &hp, 4, 0);
		DWORD res2 = WriteProcessMemory(INVALID_HANDLE_VALUE, (LPVOID)addr2, &heart, 4, 0);
		DWORD res3 = WriteProcessMemory(INVALID_HANDLE_VALUE, (LPVOID)addr3, &life, 4, 0);

		Sleep(1000);
	}
}

DWORD WINAPI inject(LPVOID) {		//在新线中程执行作弊
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
