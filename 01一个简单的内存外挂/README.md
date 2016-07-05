# 01一个简单的内存外挂

------

通过C语言编写一个简单的外挂，通过API函数修改游戏数据，从而实现作弊功能 

> * 对象分析
> * 要用的API函数简单介绍
> * 编写测试效果
> * 总体评价


------

## 对象分析

![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/01%E4%B8%80%E4%B8%AA%E7%AE%80%E5%8D%95%E7%9A%84%E5%86%85%E5%AD%98%E5%A4%96%E6%8C%82/pictures/0.jpg)
<br><br>
本次游戏对象为Super Mario XP<br>
没有更新所以可用任意版本<br>
试玩发现人物血量最大为10，心最大为99，命最大为99<br><br><br><br>


## 要用的API函数简单介绍
HWND **FindWindow**（LPCTSTR IpClassName，LPCTSTR IpWindowName）;
通过类名或窗口名查找，返回窗口句柄

DWORD **GetWindowThreadProcessId**(HWND hWnd,LPDWORD lpdwProcessId);
得到窗口句柄后通过GetWindowThreadProcessId这个函数来获得窗口所属进程ID和线程ID

HANDLE **OpenProcess**(DWORD dwDesiredAccess,BOOL bInheritHandle,DWORD dwProcessId)
打开一个已存在的进程对象，并返回进程的句柄

bool **WriteProcessMemory**(HANDLE hProcess,LPVOID lpBaseAddress,LPVOID lpBuffer,DWORD nSize,LPDWORD lpNumberOfBytesWritten
);
能写入某一进程的内存区域。入口区必须可以访问，否则操作将失败<br><br><br>

## 编写测试效果

![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/01%E4%B8%80%E4%B8%AA%E7%AE%80%E5%8D%95%E7%9A%84%E5%86%85%E5%AD%98%E5%A4%96%E6%8C%82/pictures/0.jpg)<br>
**打开游戏**<br><br>


![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/01%E4%B8%80%E4%B8%AA%E7%AE%80%E5%8D%95%E7%9A%84%E5%86%85%E5%AD%98%E5%A4%96%E6%8C%82/pictures/01.jpg)<br>
**运行外挂**<br><br>


![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/01%E4%B8%80%E4%B8%AA%E7%AE%80%E5%8D%95%E7%9A%84%E5%86%85%E5%AD%98%E5%A4%96%E6%8C%82/pictures/02.jpg)<br>
![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/01%E4%B8%80%E4%B8%AA%E7%AE%80%E5%8D%95%E7%9A%84%E5%86%85%E5%AD%98%E5%A4%96%E6%8C%82/pictures/03.jpg)<br>
**检测有效**<br><br>
```
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
```
<br><br><br>



## 总体评价

由于是简单外挂，没有GDI界面和失败对应处理，仅供入门学习交流



