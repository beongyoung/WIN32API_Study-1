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
	HACCEL hAcc;
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
		1000,
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
void Animation(int xPos, int yPos, HDC hdc, HWND hwnd)
{
	HDC mem1dc, mem2dc;
	HBITMAP RunBit[10], Mask[10], hBit, old1Bit, old2Bit;
	static int count;
	int i;

	count++;
	count = count % 10;
	hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP5));				//배경로드
	for (i = 0; i < 10; i++)
		RunBit[i] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP8 + i));	//달리는이미지로드
	for (i = 0; i < 10; i++)
		Mask[i] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP18 + i));		//달리는마스크로드
	mem1dc = CreateCompatibleDC(hdc);
	mem2dc = CreateCompatibleDC(hdc);

	old2Bit = (HBITMAP)SelectObject(mem2dc, hBit);				//배경
	BitBlt(hdc, 0, 0, 819, 614, mem2dc, 0, 0, SRCCOPY);

	old1Bit = (HBITMAP)SelectObject(mem1dc, Mask[count]);		//마스크
	BitBlt(mem1dc, 0, 0, 180, 240, mem1dc, 0, 0, SRCAND);

	SelectObject(mem2dc, RunBit[count]);						//원본
	BitBlt(mem1dc, 0, 0, 180, 240, mem2dc, 0, 0, SRCPAINT);

	BitBlt(hdc, xPos, 400, 180, 240, mem1dc, 0, 0, SRCAND);
	BitBlt(hdc, xPos, 400, 180, 240, mem2dc, 0, 0, SRCPAINT);

	SelectObject(mem1dc, old1Bit);
	SelectObject(mem2dc, old2Bit);
	for (i = 0; i < 10; i++)
		DeleteObject(Mask[i]);
	for (i = 0; i < 10; i++)
		DeleteObject(RunBit[i]);
	DeleteDC(mem1dc);
	DeleteDC(mem2dc);
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int xPos;
	switch (iMsg)
	{
	case WM_CREATE:
		xPos = -10;
		SetTimer(hwnd, 1, 100, NULL);
		break;
	case WM_TIMER:
		xPos += 5;
		if (xPos > 819) xPos = -10;
		InvalidateRgn(hwnd, NULL, true);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		Animation(xPos, 300, hdc, hwnd);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}