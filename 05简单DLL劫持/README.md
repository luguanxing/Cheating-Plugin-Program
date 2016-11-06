# 05简单DLL劫持

------

通过C语言编写一个游戏要加载的系统DLL文件(lpk.dll)，其中假DLL包含劫持功能和作弊功能且拥有相同的导出函数，将该假DLL放入游戏相同目录下，游戏打开时会自动加载该假DLL，使游戏直接包含作弊功能
> * 设计思路分析
> * 对象分析
> * 要用的函数介绍
> * 编写测试效果
> * 总体评价

------
##设计思路分析
1、构造一个与系统目录下LPK.DLL一样的导出表<br>
2、加载系统目录下的LPK.DLL<br>
3、将导出函数转发到系统目录下的LPK.DLL上<br>
4、在初始化函数中加入我们要执行的代码<br>
<br><br>

## 对象分析
![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/01%E4%B8%80%E4%B8%AA%E7%AE%80%E5%8D%95%E7%9A%84%E5%86%85%E5%AD%98%E5%A4%96%E6%8C%82/pictures/0.jpg?raw=true)<br>
<br>
本次游戏对象为Super Mario XP<br>
没有更新所以可用任意版本<br>
试玩发现人物血量最大为10，心最大为99，命最大为99<br><br><br><br>

## 要用的函数介绍
**__asm{ }**<br>
执行括号内的汇编代码
<br><br>

**JMP EAX**<br>
将EIP跳转到EAX执行，劫持DLL后要获取真正的原函数地址，使用时要进行跳转
<br><br>




## 编写测试效果

![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/05%E7%AE%80%E5%8D%95DLL%E5%8A%AB%E6%8C%81/pictures/1.jpg?raw=true)<br>
**将生成的mylpk和作弊模块放入游戏目录，并改mylpk名为lpk**<br><br><br><br><br><br>

![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/05%E7%AE%80%E5%8D%95DLL%E5%8A%AB%E6%8C%81/pictures/2.jpg?raw=true)<br>
![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/05%E7%AE%80%E5%8D%95DLL%E5%8A%AB%E6%8C%81/pictures/3.jpg?raw=true)<br>
**运行外挂**<br>
此时游戏已具备作弊效果(锁定血量)<br><br><br><br><br><br>


![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/05%E7%AE%80%E5%8D%95DLL%E5%8A%AB%E6%8C%81/pictures/4.jpg?raw=true)<br>
**工具查看加载模块**<br>
发现cheatDLL模块已加载入游戏，说明lpkj劫持成功<br><br><br><br><br><br>


![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/05%E7%AE%80%E5%8D%95DLL%E5%8A%AB%E6%8C%81/pictures/5.jpg?raw=true)<br>
![image](https://github.com/luguanxing/Cheating-Plugin-Program/blob/master/05%E7%AE%80%E5%8D%95DLL%E5%8A%AB%E6%8C%81/pictures/6.jpg?raw=true)<br>
**WIN7下z劫持**<br>
在WIN7下要劫持只需稍修改注册表即可<br><br><br><br><br><br>


```
//
//  05简单DLL劫持(作弊模块DLL部分)
//  C/C++
//
//  Created by luguanxing.
//  Copyright @2016 LGX. All rights reserved.
//
#include <windows.h>
#define Dllfunciton extern "C" __declspec(dllexport)    //以C方式导出

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

        DWORD res = WriteProcessMemory(INVALID_HANDLE_VALUE, (LPVOID)addr, &hp, 4, 0);  //写入自身修改游戏数据
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
            ::DisableThreadLibraryCalls(hModule);   //创建线程包含死循环，为防卡死必须设置
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
//  05简单DLL劫持(自写LPK.DLL模块部分)
//  C/C++
//
//  Created by luguanxing.
//  Copyright @2016 LGX. All rights reserved.
//

#include <windows.h>
#include <stdlib.h>
#define Dllfunciton extern "C" __declspec(dllexport)    //以C方式导出

#pragma comment(linker, "/EXPORT:LpkInitialize=_gamehacker_LpkInitialize,@1")	//设置导出表
#pragma comment(linker, "/EXPORT:LpkTabbedTextOut=_gamehacker_LpkTabbedTextOut,@2")
#pragma comment(linker, "/EXPORT:LpkDllInitialize=_gamehacker_LpkDllInitialize,@3")
#pragma comment(linker, "/EXPORT:LpkDrawTextEx=_gamehacker_LpkDrawTextEx,@4")
#pragma comment(linker, "/EXPORT:LpkExtTextOut=_gamehacker_LpkExtTextOut,@6")
#pragma comment(linker, "/EXPORT:LpkGetCharacterPlacement=_gamehacker_LpkGetCharacterPlacement,@7")
#pragma comment(linker, "/EXPORT:LpkGetTextExtentExPoint=_gamehacker_LpkGetTextExtentExPoint,@8")
#pragma comment(linker, "/EXPORT:LpkPSMTextOut=_gamehacker_LpkPSMTextOut,@9")
#pragma comment(linker, "/EXPORT:LpkUseGDIWidthCache=_gamehacker_LpkUseGDIWidthCache,@10")
#pragma comment(linker, "/EXPORT:ftsWordBreak=_gamehacker_ftsWordBreak,@11")

char syslpk[250] = {0};
HMODULE hmodule;
FARPROC funcaddr = NULL;

FARPROC WINAPI GetAddress(PCSTR pszProcName) {	//从真正lpk.dll中找需要调用的真正函数地址
	funcaddr = GetProcAddress(hmodule, pszProcName);  
	return funcaddr;
}

Dllfunciton gamehacker_LpkInitialize() {	//找真正函数地址后跳转
	GetAddress("LpkInitialize");
	__asm JMP EAX;
}

Dllfunciton gamehacker_LpkTabbedTextOut() {
	GetAddress("LpkTabbedTextOut");
	__asm JMP EAX;
}

Dllfunciton gamehacker_LpkDllInitialize() {
	GetAddress("LpkDllInitialize");
	__asm JMP EAX;
}

Dllfunciton gamehacker_LpkDrawTextEx() {
	GetAddress("LpkDrawTextEx");
	__asm JMP EAX;
}

Dllfunciton gamehacker_LpkEditControl() {
	GetAddress("LpkEditControl");
	__asm jmp DWORD ptr [EAX];
}

Dllfunciton gamehacker_LpkExtTextOut() {
	GetAddress("LpkExtTextOut");
	__asm JMP EAX;
}

Dllfunciton gamehacker_LpkGetCharacterPlacement() {
	GetAddress("LpkGetCharacterPlacement");
	__asm JMP EAX;
}

Dllfunciton gamehacker_LpkGetTextExtentExPoint() {
	GetAddress("LpkGetTextExtentExPoint");
	__asm JMP EAX;
}

Dllfunciton gamehacker_LpkPSMTextOut() {
	GetAddress("LpkPSMTextOut");
	__asm JMP EAX;
}

Dllfunciton gamehacker_LpkUseGDIWidthCache() {
	GetAddress("LpkUseGDIWidthCache");
	__asm JMP EAX;
}

Dllfunciton gamehacker_ftsWordBreak() {
	GetAddress("ftsWordBreak");
	__asm JMP EAX;
}


BOOL APIENTRY DllMain(HMODULE hModule,  DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch(ul_reason_for_call) {
        case DLL_PROCESS_ATTACH: {
			GetSystemDirectory(syslpk, 250);
			strcat(syslpk, "\\lpk");
			hmodule = LoadLibrary(syslpk);	//加载真正系统lpk.dll
			::LoadLibrary("cheatDLL");	//加载作弊模块lpk.dll
        } break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
        default:;
    }
    return true;
}
```
<br><br><br>



## 总体评价

DLL劫持可以在不用手动加载外挂,在运行游戏时自动加载作弊功能
