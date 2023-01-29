#include <iostream>
#include <Windows.h>
#include <winuser.h>
#include <pthread.h>

HHOOK _hook;

KBDLLHOOKSTRUCT kbdStruct;

int x1_Stats = 0;
int x2_Stats = 0;

bool show = false;
bool last = false;

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
	if (!x1_Stats && last)
	{
		show = !show;
		show? Beep(1080, 150): Beep(500, 150);
	}
	last = x1_Stats;
	return CallNextHookEx(_hook, code, wParam, lParam);
}

VOID SimulateKeystroke()
{
	INPUT ip{};
	ip.type = INPUT_KEYBOARD;
	ip.ki.time = 0;
	ip.ki.wVk = 0;
	ip.ki.dwExtraInfo = 0;
	ip.ki.dwFlags = KEYEVENTF_SCANCODE;
	ip.ki.wScan = 0x21; // F

	SendInput(1, &ip, sizeof(INPUT));

	ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));
}

void* autoFr(void* args) {
	while (true)
	{
		if (!x1_Stats && show)
		{
			SimulateKeystroke();
		}
		Sleep(500);
	}
	return 0;
}

int main()
{
	SetConsoleTitle(L"FR - H");
	std::cout << "按下侧键开启/关闭自动检视" << std::endl;

	_hook = SetWindowsHookEx(WH_MOUSE_LL, HookCallBack, NULL, 0);
	MSG msg;

	pthread_t pthread;
	int ret = pthread_create(&pthread, NULL, autoFr, NULL);
	if (ret != 0) std::cout << "pthread_create error: error_code=" << ret << std::endl;


	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
	}

	return 0;
}