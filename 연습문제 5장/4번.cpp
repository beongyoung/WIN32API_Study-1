#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<math.h>
#include"resource.h"
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

HINSTANCE hInst;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;
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
		_T("홍길동의 첫 번째 윈도우"),
		WS_OVERLAPPEDWINDOW,
		100,
		100,
		1000,
		600,
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HDC hdc, memdc;
	PAINTSTRUCT ps;
	HPEN hPen, oldPen;
	static HBITMAP hBit, oldBit;
	static int xPos, yPos;
	static RECT rect;
	static bool DirX = true, DirY = false;

	switch (iMsg)
	{
	case WM_CREATE:
		xPos = yPos = 100;
		GetClientRect(hwnd, &rect);
		break;
	case WM_KEYDOWN:
		SetTimer(hwnd, 1, 100, NULL);
		break;
	case WM_TIMER:
		hdc = GetDC(hwnd);
		if (DirX == true && DirY == false)
		{
			xPos += 20;
			yPos -= 20;
		}
		else if (DirX == true && DirY == true)
		{
			xPos += 20;
			yPos += 20;
		}
		else if (DirX == false && DirY == true)
		{
			xPos -= 20;
			yPos += 20;
		}
		else if (DirX == false && DirY == false)
		{
			xPos -= 20;
			yPos -= 20;
		}
		BitBlt(hdc, xPos, yPos, 200, 200, memdc, 100, 150, SRCCOPY);
		if (xPos + 200 >= rect.right || xPos <= rect.left)
			DirX = DirX ? false : true;
		if (yPos + 200 >= rect.bottom || yPos <= rect.top)
			DirY = DirY ? false : true;
		ReleaseDC(hwnd, hdc);
		break;
	case WM_PAINT:
		hdc = GetDC(hwnd);
		hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		memdc = CreateCompatibleDC(hdc);
		oldBit = (HBITMAP)SelectObject(memdc, hBit);
		BitBlt(hdc, xPos, yPos, 200, 200, memdc, 100, 150, SRCCOPY);
		ReleaseDC(hwnd, hdc);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}