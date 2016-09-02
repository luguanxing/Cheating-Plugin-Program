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

    HWND h = ::FindWindow(NULL, "植物大战僵尸中文版");   //      寻找并打开进程
    DWORD processid;
    GetWindowThreadProcessId(h, &processid);
    HANDLE processh = 0;
    processh = OpenProcess(PROCESS_ALL_ACCESS,FALSE,processid);

    if (processh == 0) {    //    对应处理
        printf("打开进程失败!\n");
        return 1;
    } else {
        printf("打开进程成功!\n");

        int sun;    //    用于存放阳光数据
        LPCVOID mbase = (LPCVOID)0x006A9EC0;    //    [[[006A9EC0]+768]+5560] = sun
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
        DWORD maxsun = 99999;     //  写入内存修改游戏数据
        DWORD maxmoney = 99999;
        WriteProcessMemory(processh, (LPVOID)mbase, &maxsun, 4, 0);
        WriteProcessMemory(processh, (LPVOID)mbase2, &maxmoney, 4, 0);
        */
    }

    return 0;
}
