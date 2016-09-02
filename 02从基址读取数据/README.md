# 02从基址读取数据

------

通过C语言编写一个外挂，通过API函数获取游戏数据，从而实现作弊功能 

> * 对象分析
> * 要用的API函数简单介绍
> * 编写测试效果
> * 总体评价


------

## 对象分析
![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/02%E4%BB%8E%E5%9F%BA%E5%9D%80%E8%AF%BB%E5%8F%96%E6%95%B0%E6%8D%AE/pictures/0.jpg?raw=true)<br>
<br>
本次游戏对象为Plants vs. Zombies1.0.0.1051汉化第一版<br><br>
试玩发现主要数据有阳光、金钱，其中金钱最小以10为单位，因此数据应以“显示钱数/10”保存<br><br>
通过CE确定阳光和金钱的基址计算公式<br>
[[[006A9EC0]+768]+5560] = sun<br>
[[[006A9F38]+82C]+28] = money<br>
故所求基址为006A9EC0和006A9F38，利用偏移即可得出数据
<br><br><br><br><br>


## 要用的API函数简单介绍
HWND **ReadProcessMemory**(HANDLE hProcess, LPCVOID lpBaseAddress, LPVOID lpBuffer, DWORD nSize, LPDWORD lpNumberOfBytesRead);
根据进程句柄读入该进程的某个内存空间lpBaseAddress的nSize字节，并写入缓冲区lpBuffer，多次计算基址和偏移即可
<br><br><br><br><br>


## 编写测试效果

![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/02%E4%BB%8E%E5%9F%BA%E5%9D%80%E8%AF%BB%E5%8F%96%E6%95%B0%E6%8D%AE/pictures/01.jpg?raw=true)<br>
**打开游戏**<br><br>


![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/02%E4%BB%8E%E5%9F%BA%E5%9D%80%E8%AF%BB%E5%8F%96%E6%95%B0%E6%8D%AE/pictures/02.jpg?raw=true)<br>
**运行外挂(截图前修改过钱数值)**<br>
成功读取出阳光和金钱的数据，如再使用写入内存将所想要的值写入即达到修改功能
<br><br>


![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/02%E4%BB%8E%E5%9F%BA%E5%9D%80%E8%AF%BB%E5%8F%96%E6%95%B0%E6%8D%AE/pictures/03.jpg?raw=true)<br>
**修改成功**<br><br>
```
//
//  02从基址读取数据.cpp
//  C/C++
//
//  Created by luguanxing.
//  Copyright @2016 LGX. All rights reserved.
//

#include <stdio.h>
#include <windows.h>
int main() {
	
	HWND h = ::FindWindow(NULL, "植物大战僵尸中文版");   //	  寻找并打开进程
	DWORD processid;
	GetWindowThreadProcessId(h, &processid);
	HANDLE processh = 0;
	processh = OpenProcess(PROCESS_ALL_ACCESS,FALSE,processid);
	
	if (processh == 0) { 	//    对应处理
        printf("打开进程失败!\n");
        return 1;
	} else {
		printf("打开进程成功!\n");
		
		int sun;    //	  用于存放阳光数据
		LPCVOID mbase = (LPCVOID)0x006A9EC0;	//    [[[006A9EC0]+768]+5560] = sun
		LPVOID mbuffer = (LPVOID)&sun;
		::ReadProcessMemory(processh, mbase, mbuffer, 4,  0);
		mbase = (LPCVOID)(sun + 0x768);
		::ReadProcessMemory(processh, mbase, mbuffer, 4,  0);
		mbase = (LPCVOID)(sun + 0x5560);
		::ReadProcessMemory(processh, mbase, mbuffer, 4,  0);
		printf("您有阳光:%d\n", sun);

		int money;     //    用于存放钱数据
		LPCVOID mbase2 = (LPCVOID)0x006A9F38;      //    [[[006A9F38]+82C]+28] = money
		LPVOID mbuffer2 = (LPVOID)&money;
		::ReadProcessMemory(processh, mbase2, mbuffer2, 4,  0);
		mbase2 = (LPCVOID)(money + 0x82C);
		::ReadProcessMemory(processh, mbase2, mbuffer2, 4,  0);
		mbase2 = (LPCVOID)(money + 0x28);
		::ReadProcessMemory(processh, mbase2, mbuffer2, 4,  0);
		printf("您有钱:%d0\n", sun);
	
		/*
		DWORD maxsun = 99999;	  //  写入内存修改游戏数据
        DWORD maxmoney = 99999;
		WriteProcessMemory(processh, (LPVOID)mbase, &maxsun, 4, 0);
        WriteProcessMemory(processh, (LPVOID)mbase2, &maxmoney, 4, 0);
		*/
	}
	
	return 0;
}

```
<br><br><br>



## 总体评价

简单读取游戏中的内存数据并显示



