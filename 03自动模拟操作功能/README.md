# 03自动模拟操作功能

------

通过C语言编写一个外挂，通过API函数模拟进行连连看操作，实现自动进行游戏

> * 对象分析
> * 要用的API函数简单介绍
> * 编写测试效果
> * 总体评价


------

## 对象分析
![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/03%E8%87%AA%E5%8A%A8%E6%A8%A1%E6%8B%9F%E6%93%8D%E4%BD%9C%E5%8A%9F%E8%83%BD/pictures/0.jpg)<br><br>
本次游戏对象为某连连看游戏<br><br>
试玩发现需要进行棋子确定和模拟鼠标操作<br><br>
通过CE确定棋盘基址获得游戏数据，再利用算法确定目标，执行鼠标操作<br><br><br><br>


## 要用的API函数简单介绍
LRESULT **SendMessage**（HWND hWnd，UINT Msg，WPARAM wParam，LPARAM IParam）;<br><br>
其中Msg为WM_LBUTTONDOWN，WM_LBUTTONUP，表示对鼠标的软件模拟操作(虽然兼容性和安全性不如硬件模拟的mouse_event，但不改变移动鼠标指针)<br><br>
故一般写作SendMessage(hwnd, WM_LBUTTONUP/WM_LBUTTONUP, 0, Y坐标<<16+X);<br><br><br><br>




## 编写测试效果

![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/03%E8%87%AA%E5%8A%A8%E6%A8%A1%E6%8B%9F%E6%93%8D%E4%BD%9C%E5%8A%9F%E8%83%BD/pictures/0.jpg)<br>
**打开游戏**<br><br><br><br>


![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/03%E8%87%AA%E5%8A%A8%E6%A8%A1%E6%8B%9F%E6%93%8D%E4%BD%9C%E5%8A%9F%E8%83%BD/pictures/1.jpg)<br>
**运行外挂**<br>
我用MFC将几个函数功能封装为按钮<br>
1.单消：读取数据并消一对<br>
2.自动：恢复/暂停一个线程，这个线程有消一对和延时的循环功能<br>
3.秒杀：不延时，直接消999对<br><br><br><br>



![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/03%E8%87%AA%E5%8A%A8%E6%A8%A1%E6%8B%9F%E6%93%8D%E4%BD%9C%E5%8A%9F%E8%83%BD/pictures/2.jpg)<br>
![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/03%E8%87%AA%E5%8A%A8%E6%A8%A1%E6%8B%9F%E6%93%8D%E4%BD%9C%E5%8A%9F%E8%83%BD/pictures/3.jpg)<br>
**检测有效**<br><br><br><br>
```
//
//  03自动模拟操作功能（部分）.cpp
//  C/C++
//
//  Created by luguanxing.
//  Copyright @2016 LGX. All rights reserved.
//

#include<windows.h>
#include<iostream>
using namespace std;

HWND gameh;
DWORD processid = 0;
HANDLE processh = 0;
byte chessdata[11][19];		//数据保存缓冲区,假设棋盘为11*19
const char *gamename = "连连看";	//某连连看游戏名称，可通过SPY++查找

struct point {		//具体使用和算法见"连连看BFS算法"
	int x, y, direct;
	int step, hp;
};

void readchess() {	//读数据并更新
	gameh = ::FindWindow(NULL, gamename);
	::GetWindowThreadProcessId(gameh, &processid);
	processh = ::OpenProcess(PROCESS_ALL_ACCESS, false, processid);
	LPCVOID pbase = (LPCVOID)棋盘基址;
	LPVOID  nbuffer = (LPVOID)&chessdata;
	::ReadProcessMemory(processh, pbase, nbuffer, 11*19, &byread);
}

void click2p(POINT p1, POINT p2) {	//点击两个点的操作功能
	HWND hwnd=FindWindow(NULL, gamename);
	int lparam;
	lparam=((p1.y*35+192)<<16)+(p1.x*31+21);
	SendMessage(hwnd, WM_LBUTTONDOWN, 0, lparam);
	SendMessage(hwnd, WM_LBUTTONUP, 0, lparam);
	lparam=((p2.y*35+192)<<16)+(p2.x*31+21);
	SendMessage(hwnd, WM_LBUTTONDOWN, 0, lparam);
	SendMessage(hwnd, WM_LBUTTONUP, 0, lparam);
}

void clearapair() {	//找到可消去的两个点并点击
	 POINT p1, p2;
	 int x1, y1, x2, y2;
	  for (y1=0; y1<11; y1++)
		  for (x1=0; x1<19; x1++) {
		  	 if (!chessdata[y1][x1])
		 	  	continue;
			  for (y2=0; y2<11; y2++)
				for (x2=0; x2<19; x2++)
				  if (chessdata[y2][x2] && (chessdata[y1][x1]==chessdata[y2][x2]) && (x1!=x2 || y1!=y2) )  {  
					  p1.x=x1; p1.y=y1;
					  p2.x=x2; p2.y=y2;
					  readchess();
					  if (llk_bfs(y1, x1, y2, x2) != -1) {
					  	click2p(p1, p2);
					  	return;	
					  }
				  }
		  }

}

```
<br><br><br>



## 总体评价

简单自动模拟操作进行游戏



