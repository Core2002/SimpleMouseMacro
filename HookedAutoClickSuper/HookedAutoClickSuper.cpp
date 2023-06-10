#include <iostream>
#include <Windows.h>
#include <winuser.h>
#include <pthread.h>
#include "NTDriverLoader.hpp"
#include <shlwapi.h>
#include <comdef.h>
#define var auto

HANDLE drvhandle;

bool SetHandle()
{
	drvhandle = CreateFileA("\\\\.\\kmclass", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, 0);
	return ((char*)drvhandle + 1) != 0;
}

int LeftDown[] = { 0, 1, 0, 0, 0, 0 };
int LeftUp[] = { 0, 2, 0, 0, 0, 0 };

DWORD Call(int* p)
{
	DWORD lpBytesReturned = 0;
	DeviceIoControl(drvhandle, 0x8000201C, p, 24, NULL, 0, &lpBytesReturned, NULL);
	return lpBytesReturned;
}

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
			Call(LeftDown);
			Sleep(5);
			Call(LeftUp);
		}
		Sleep(15);
	}
	return 0;
}



int main()
{
	SetConsoleTitle(L"CPS - 31 - H - S");
	std::cout << "按住鼠标侧键不放自动连点" << std::endl;
	WCHAR Filename[114514];
	GetModuleFileNameW(0, Filename, 0x400u);
	PathRemoveFileSpecW(Filename);
	PathAppendW(Filename, L"SuperMouse64.sys");
	if (!PathFileExistsW(Filename))
	{
		MessageBoxW(0, L"驱动SuperMouse64.sys加载失败，请重新下载并解压软件", L"错误", 0);
		return 0;
	}
	_bstr_t fileName(Filename);
	MyNTDriverLoader::LoadNTDriver("SuperMouseService", fileName);
	var ok = SetHandle();
	std::cout << ok << " Start\n";

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