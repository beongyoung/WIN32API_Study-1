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
	HDC hdc, memdc;
	PAINTSTRUCT ps;
	HPEN hPen, oldPen;
	HBITMAP hBit,hObjectBit,hMaskBit, oldBit;
	static int mX, mY;
	switch (iMsg)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		
		ReleaseDC(hwnd, hdc);
		break;

	case WM_LBUTTONDOWN:
		hdc = GetDC(hwnd);
		mX = LOWORD(lParam);
		mY = HIWORD(lParam);
		hMaskBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));		//마스크
		hObjectBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));	//원본이미지
		memdc = CreateCompatibleDC(hdc);
		oldBit = (HBITMAP)SelectObject(memdc, hMaskBit);
		BitBlt(hdc, mX, mY, 55, 55, memdc, 0, 0, SRCAND);
		SelectObject(memdc, hObjectBit);
		BitBlt(hdc, mX, mY, 55, 55, memdc, 0, 0, SRCPAINT);
		SelectObject(memdc, oldBit);
		ReleaseDC(hwnd, hdc);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));//배경
		memdc = CreateCompatibleDC(hdc);
		oldBit = (HBITMAP)SelectObject(memdc, hBit);
		BitBlt(hdc, 0, 0, 1024, 768, memdc, 0, 0, SRCCOPY);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}