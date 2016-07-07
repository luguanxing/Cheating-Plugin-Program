#include <stdio.h>
#include <windows.h>
int main() {
	
	HWND h = ::FindWindow(NULL, "植物大战僵尸中文版");  //  寻找并打开进程
	DWORD processid;
	GetWindowThreadProcessId(h, &processid);
	HANDLE processh = 0;
	processh = OpenProcess(PROCESS_ALL_ACCESS,FALSE,processid);
	
	if (processh == 0) {    //  对应处理
        printf("打开进程失败!\n");
        return 1;
	} else {
		printf("打开进程成功!\n");
		
		int sun;	// [[[006A9EC0]+768]+5560] = sun
		LPCVOID mbase = (LPCVOID)0x006A9EC0;
		LPVOID mbuffer = (LPVOID)&sun;
		::ReadProcessMemory(processh, mbase, mbuffer, 4,  0);
		mbase = (LPCVOID)(sun + 0x768);
		::ReadProcessMemory(processh, mbase, mbuffer, 4,  0);
		mbase = (LPCVOID)(sun + 0x5560);
		::ReadProcessMemory(processh, mbase, mbuffer, 4,  0);
		printf("您有阳光:%d\n", sun);

		int money;	//	[[[006A9F38]+82C]+28] = money
		LPCVOID mbase2 = (LPCVOID)0x006A9F38;
		LPVOID mbuffer2 = (LPVOID)&money;
		::ReadProcessMemory(processh, mbase2, mbuffer2, 4,  0);
		mbase2 = (LPCVOID)(money + 0x82C);
		::ReadProcessMemory(processh, mbase2, mbuffer2, 4,  0);
		mbase2 = (LPCVOID)(money + 0x28);
		::ReadProcessMemory(processh, mbase2, mbuffer2, 4,  0);
		printf("您有钱:%d0\n", sun);
	
	}
	
	return 0;
}


