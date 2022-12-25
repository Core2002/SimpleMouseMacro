#include <iostream>
#include <Windows.h>

int main() {
	SetConsoleTitle(L"CPS - 28");
	std::cout << "按住鼠标侧键不放自动连点" << std::endl;
	int delay = 15;

	while (true)
	{
		if (GetAsyncKeyState(VK_XBUTTON2))
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			Sleep(5);
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		}
		Sleep(delay);
	}
}