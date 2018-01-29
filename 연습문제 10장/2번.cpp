#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<winres.h>
#include<process.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
//#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
void ThreadProc();
HINSTANCE hInst;
HWND hwnd;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS WndClass;

	hInst = hInstance;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = _T("Window Class Name");

	RegisterClass(&WndClass);

	hwnd = CreateWindow(
		_T("Window Class Name"),
		_T("π⁄¡§»£"),
		WS_OVERLAPPEDWINDOW,
		50,
		50,
		1000,
		700,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
int xPos = 30, yPos = 30;
static RECT rt;
HANDLE hEvent;
BOOL directionX = TRUE;
BOOL directionY = TRUE;
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static HANDLE hThread;
	static int count = 0;
	static BOOL GO = FALSE;
	static BOOL init = TRUE;
	switch (iMsg)
	{
	case WM_CREATE:
		GetClientRect(hwnd, &rt);
		hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
		break;

	case WM_LBUTTONDOWN:
		if (init)
		{
			hThread = (HANDLE)_beginthreadex(NULL, 0, (unsigned int(__stdcall *)(void *))ThreadProc, NULL, 0, NULL);
			init = FALSE;
		}
		if (GO)				//∏ÿ√„
		{
			ResetEvent(hEvent);
			GO = FALSE;
		}
		else				//Ω√¿€
		{
			SetEvent(hEvent);
			GO = TRUE;
		}

		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		Ellipse(hdc, xPos - 20, yPos - 20, xPos + 20, yPos + 20);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		CloseHandle(hThread);
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
void ThreadProc()
{
	HDC hdc;

	hdc = GetDC(hwnd);
	while (1)
	{
		WaitForSingleObject(hEvent, INFINITE);
		if (xPos < 20 || xPos>rt.right - 20)
			directionX = directionX ? FALSE : TRUE;
		if (yPos < 20 || yPos>rt.bottom - 20)
			directionY = directionY ? FALSE : TRUE;

		if (directionX)
			xPos++;
		else
			xPos--;

		if (directionY)
			yPos++;
		else
			yPos--;

		InvalidateRgn(hwnd, NULL, TRUE);
		Sleep(5);
	}
	ReleaseDC(hwnd, hdc);
	return;
}