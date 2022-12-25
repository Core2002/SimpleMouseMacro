#include <iostream>
#include <Windows.h>

int main() {
	SetConsoleTitle(L"VK_XBUTTON1");
	std::cout << "一键瞬狙" << std::endl;
	int delay = 15;

	while (true)
	{
		if (GetAsyncKeyState(VK_XBUTTON1))
		{
			mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
			Sleep(5);
			mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
			Sleep(350);
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			Sleep(5);
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

			//Sleep(200);
			//// Q
			//keybd_event('Q', 0, 0, 0);
			//Sleep(5);
			//keybd_event('Q', 0, KEYEVENTF_KEYUP, 0);
			//Sleep(200);
			//// Q
			//keybd_event('Q', 0, 0, 0);
			//Sleep(5);
			//keybd_event('Q', 0, KEYEVENTF_KEYUP, 0);
		}
		Sleep(delay);
	}
}