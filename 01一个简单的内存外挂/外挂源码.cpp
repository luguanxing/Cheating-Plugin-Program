//
//  01一个简单的内存外挂.cpp
//  C/C++
//
//  Created by luguanxing.
//  Copyright @2016 LGX. All rights reserved.
//

#include <windows.h>
#include <stdio.h>

int main() {
	HWND h = ::FindWindow(NULL, "Super Mario XP");	//	寻找并打开进程
	DWORD processid;
	GetWindowThreadProcessId(h, &processid);
	HANDLE hprocess = 0;
	hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processid);

	if (hprocess == 0) {	//	对应处理
		printf("打开进程失败!\n");
		return 1;
	} else {
		printf("打开进程成功!\n");

		DWORD hp = 10;	//	要修改的游戏数据最大值	
		DWORD heart = 99;
		DWORD life = 99;

		DWORD addr = 0x00428282;	//	通过CE找到的游戏数据地址
		DWORD addr2 = 0x00428292;
		DWORD addr3 = 0x004282a2;
  
		DWORD res = WriteProcessMemory(hprocess, (LPVOID)addr, &hp, 4, 0);	//	写入内存修改游戏数据
		DWORD res2 = WriteProcessMemory(hprocess, (LPVOID)addr2, &heart, 4, 0);
		DWORD res3 = WriteProcessMemory(hprocess, (LPVOID)addr3, &life, 4, 0);
	
		return 0;
	}
}
