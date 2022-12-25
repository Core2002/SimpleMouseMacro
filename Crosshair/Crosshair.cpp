#include <Windows.h>
#include <gl/glut.h>

int main()
{
	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = 40;
	pfd.nVersion = 1;
	//支持绘制到窗口、支持OPENGL、支持GDI
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 0;
	pfd.cRedBits = 0;
	pfd.cRedShift = 0;
	pfd.cGreenBits = 0;
	pfd.cGreenShift = 0;
	pfd.cBlueBits = 0;
	pfd.cBlueShift = 0;
	pfd.cAlphaBits = 0;
	pfd.cAlphaShift = 0;
	pfd.cAccumBits = 0;
	pfd.cAccumRedBits = 0;
	pfd.cAccumGreenBits = 0;
	pfd.cAccumBlueBits = 0;
	pfd.cAccumAlphaBits = 0;
	pfd.cDepthBits = 0;
	pfd.cStencilBits = 0;
	pfd.cAuxBuffers = 0;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.bReserved = 0;
	pfd.dwLayerMask = 0;
	pfd.dwVisibleMask = 0;
	pfd.dwDamageMask = 0;

	//获取屏幕的设备环境
	HDC hdc = GetDC(NULL);
	//选择像素格式
	int pixelFormat = ChoosePixelFormat(hdc, &pfd);
	//设置像素格式
	SetPixelFormat(hdc, pixelFormat, &pfd);
	//创建OpenGL渲染环境
	HGLRC hglrc = wglCreateContext(hdc);
	//为当前线程指定设备环境和渲染环境
	wglMakeCurrent(hdc, hglrc);

	//char ch;
	int x = 1271, y = 429;
	while (true)
	{
		/*ch = _getch();
		printf("x %d  y %d\n", x, y);
		if (ch == 119)
		{
			y++;
		}
		else if (ch == 115) {
			y--;
		}
		else if (ch == 97) {
			x--;
		}
		else if (ch == 100) {
			x++;
		}*/

		glViewport(x, y, 16, 16);
		//glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.0f, 0.5f);
		glVertex2f(0.5f, -0.5f);
		glEnd();
		glFlush();

	}
	return 0;
}