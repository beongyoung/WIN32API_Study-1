#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<math.h>
#include<stdio.h>
#include"resource.h"
#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
HINSTANCE hInst;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	hInst = hInstance;
	WNDCLASS WndClass;
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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	RegisterClass(&WndClass);

	hwnd = CreateWindow(
		_T("Window Class Name"),
		_T("홍길동의 첫 번째 윈도우"),
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, mem1dc;
	PAINTSTRUCT ps;
	HPEN hPen, oldPen;
	static HBITMAP hBit, oldBit;
	static int BY = 1;
	static bool DUP = false;
	static int posX[5], posY[5];
	switch (iMsg)
	{
	case WM_CREATE:
		hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP5));
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_3BY3:
			BY = 3;
			break;
		case ID_4BY4:
			BY = 4;
			break;
		case ID_5BY5:
			BY = 5;
			break;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		mem1dc = CreateCompatibleDC(hdc);
		oldBit = (HBITMAP)SelectObject(mem1dc, hBit);
		switch (BY)
		{
		case 3:
			posX[0] = 1; posX[1] = 0; posX[2] = 2;
			posY[0] = 0; posY[1] = 2; posY[2] = 1;
			break;
		case 4:
			posX[0] = 1; posX[1] = 0; posX[2] = 3; posX[3] = 2;
			posY[0] = 3; posY[1] = 2; posY[2] = 1; posY[3] = 0;
			break;
		case 5:
			posX[0] = 1; posX[1] = 0; posX[2] = 3; posX[3] = 2; posX[4] = 4;
			posY[0] = 4; posY[1] = 2; posY[2] = 1; posY[3] = 3; posY[4] = 0;
			break;
		}

		for (int i = 0; i < BY; i++)
		{
			for (int j = 0; j < BY; j++)
			{
				BitBlt(hdc, i * (819.0f / BY), j * (614.0f / BY), 819.0f / BY, 614.0f / BY, mem1dc, (819.0f / BY)*posX[i], (614.0f / BY)*posY[j], SRCCOPY);
			}
		}
		SelectObject(mem1dc, hBit);
		DeleteDC(mem1dc);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		DeleteObject(hBit);
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}