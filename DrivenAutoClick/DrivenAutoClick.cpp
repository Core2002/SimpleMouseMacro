#include <iostream>
#include <Windows.h>
#include <pthread.h>
#include <direct.h>
#include <string>


#pragma comment(lib, "kmclassdll.lib")
extern "C" __declspec(dllimport) BOOL __cdecl LoadNTDriver(char* lpszDriverName, char* lpszDriverPath);
extern "C" __declspec(dllimport) void __cdecl SetHandle();
extern "C" __declspec(dllimport) BOOL __cdecl UnloadNTDriver(char* szSvrName);
extern "C" __declspec(dllimport) void __cdecl MouseLeftButtonDown();
extern "C" __declspec(dllimport) void __cdecl MouseLeftButtonUp();


HHOOK _hook;

KBDLLHOOKSTRUCT kbdStruct;

int x1_Stats = 0;
int x2_Stats = 0;


LRESULT CALLBACK HookCallBack(int code, WPARAM wParam, LPARAM lParam) {
	kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
	if (wParam == WM_XBUTTONDOWN)
	{
		if (GET_XBUTTON_WPARAM(kbdStruct.flags) == 1)
		{
			x1_Stats = 1;
		}
		else if (GET_XBUTTON_WPARAM(kbdStruct.flags) == 2) {
			x2_Stats = 1;
		}
	}
	else if (wParam == WM_XBUTTONUP)
	{
		if (GET_XBUTTON_WPARAM(kbdStruct.flags) == 1)
		{
			x1_Stats = 0;
		}
		else if (GET_XBUTTON_WPARAM(kbdStruct.flags) == 2) {
			x2_Stats = 0;
		}
	}
	return CallNextHookEx(_hook, code, wParam, lParam);
}

void* daemonClick(void* args) {
	while (true)
	{
		if (x2_Stats == 1)
		{
			MouseLeftButtonDown();
			Sleep(5);
			MouseLeftButtonUp();
		}
		Sleep(15);
	}
	return 0;
}

int main()
{
	char lpszDriverName[] = "FiFuMouse";
	char* lpszDriverPath;
	//也可以将buffer作为输出参数
	if ((lpszDriverPath = _getcwd(NULL, 0)) == NULL)
	{
		perror("getcwd error");
		return 0;
	}
	else
	{
		std::string sysFile = "\\kmclass.sys";
		lpszDriverPath = (char*)sysFile.insert(0, lpszDriverPath).c_str();
		UnloadNTDriver(lpszDriverName);
		if (LoadNTDriver(lpszDriverName, lpszDriverPath))
		{
			std::cout << "成功加载驱动：" << lpszDriverPath << "\n";
		}
		else {
			return 0;
		}
		SetHandle();
	}

	SetConsoleTitle(L"CPS - 30 - HD");
	std::cout << "按住鼠标侧键不放自动连点" << std::endl;

	_hook = SetWindowsHookEx(WH_MOUSE_LL, HookCallBack, NULL, 0);
	MSG msg;

	pthread_t pthread;
	int ret = pthread_create(&pthread, NULL, daemonClick, NULL);
	if (ret != 0) std::cout << "pthread_create error: error_code=" << ret << std::endl;


	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
	}

	return 0;
}