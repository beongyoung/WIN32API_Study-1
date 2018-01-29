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
	hInst = hInstance;
	WNDCLASS WndClass;
	HACCEL hAccel;
	hAccel = LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_ACCELERATOR1));
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
		if (!TranslateAccelerator(hwnd, hAccel, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HDC hdc, memdc;
	PAINTSTRUCT ps;
	HPEN hPen, oldPen;
	static HBITMAP hBit, oldBit;
	static int pasteX,pasteY,startX, startY, endX, endY;
	static bool regionSelect = false;

	switch (iMsg)
	{
	case WM_CREATE:
		break;
	case WM_LBUTTONDOWN:
		if (!regionSelect)
		{
			startX = LOWORD(lParam);
			startY = HIWORD(lParam);
		}
		else
		{
			pasteX = LOWORD(lParam);
			pasteY = HIWORD(lParam);
		}
		break;
	case WM_LBUTTONUP:
		if (!regionSelect)
		{
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_ACCELERATOR40001://복사
			hdc = GetDC(hwnd);
			regionSelect = true;
			hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
			memdc = CreateCompatibleDC(hdc);
			oldBit = (HBITMAP)SelectObject(memdc, hBit);
			//MessageBox(hwnd, _T("복사키눌림"), _T("메시지박스"), MB_OK);
			break;
		case ID_ACCELERATOR40002://붙여넣기
			BitBlt(hdc, pasteX, pasteY, endX - startX, endY - startY, memdc, startX, startY, SRCCOPY);
			SelectObject(memdc, oldBit);
			ReleaseDC(hwnd, hdc);
			regionSelect = false;
			//MessageBox(hwnd, _T("붙여넣기키눌림"), _T("메시지박스"), MB_OK);
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		memdc = CreateCompatibleDC(hdc);
		oldBit = (HBITMAP)SelectObject(memdc, hBit);
		BitBlt(hdc, 0, 0, 1024, 768, memdc, 0, 0, SRCCOPY);
		SelectObject(memdc, oldBit);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}