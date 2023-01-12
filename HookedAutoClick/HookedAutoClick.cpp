#include <iostream>
#include <Windows.h>
#include <pthread.h>

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
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			Sleep(5);
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		}
		Sleep(15);
	}
	return 0;
}

int main()
{
	SetConsoleTitle(L"CPS - 28 - H");
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