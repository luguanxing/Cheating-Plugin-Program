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
			::LoadLibrary("cheatDLL");	//加载作弊模块cheatDLL.dll
        } break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
        default:;
    }
    return true;
}
