#include <Windows.h>
#include<tchar.h>
#include<string.h>
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;

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

	RegisterClass(&WndClass);

	hwnd = CreateWindow(
		_T("Window Class Name"),
		_T("홍길동의 첫 번째 윈도우"),
		WS_OVERLAPPEDWINDOW,
		200,
		300,
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static TCHAR str[10][100];
	static int count, yPos, lengh[10];
	static SIZE size;
	RECT rect1 = { 100,180,150,230 };
	HBRUSH hBrush, oldBrush;
	RECT rect = { 300,300,400,400 };
	switch (iMsg)
	{
	case WM_CREATE:
		break;
	case WM_KEYDOWN:
		hdc = GetDC(hwnd);
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

		switch (wParam)
		{
		case (VK_RIGHT):
			Rectangle(hdc, 200, 150, 250, 230); //오
			break;
		case VK_UP:
			Rectangle(hdc, 150, 70, 200, 150); //위
			break;
		case VK_LEFT:
			Rectangle(hdc, 100, 150, 150, 230); //왼
			break;
		case VK_DOWN:
			Rectangle(hdc, 150, 230, 200, 310); //아래
			break;
		}
		SelectObject(hdc, oldBrush);
		ReleaseDC(hwnd, hdc);
		break;
	case WM_KEYUP:
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

		Rectangle(hdc, 100, 150, 150, 230); //왼
		TextOut(hdc, 110, 180, _T("왼쪽"), _tcslen(_T("왼쪽")));
		Rectangle(hdc, 200, 150, 250, 230); //오
		TextOut(hdc, 210, 180, _T("오른"), _tcslen(_T("오른")));
		Rectangle(hdc, 150, 70, 200, 150); //위
		TextOut(hdc, 160, 100, _T("위쪽"), _tcslen(_T("위쪽")));
		Rectangle(hdc, 150, 230, 200, 310); //아래
		TextOut(hdc, 160, 260, _T("아래"), _tcslen(_T("아래")));

		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);								//브러쉬 삭제 (메모리낭비방지)
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		DestroyCaret();
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}