#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<math.h>
#include<stdio.h>
#include"resource.h"
//#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
HINSTANCE hInst;
HWND hwndChild[2];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASS WndClass;

	hInst = hInstance;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = FrameWndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU3);
	WndClass.lpszClassName = _T("Window Class Name");
	RegisterClass(&WndClass);

	WndClass.lpszMenuName = NULL;
	WndClass.lpfnWndProc = ChildWndProc;
	WndClass.lpszClassName = _T("Child Window Class Name");
	RegisterClass(&WndClass);

	hwnd = CreateWindow(
		_T("Window Class Name"),
		_T("¹ÚÁ¤È£"),
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

LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;

	switch (iMsg)
	{
	case WM_CREATE:
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_SPLIT:
			GetClientRect(hwnd, &rect);
			hwndChild[0] = CreateWindowEx(WS_EX_CLIENTEDGE, _T("Child Window Class Name"), NULL, WS_CHILD | WS_VISIBLE, 0, 0, rect.right, rect.bottom / 2 - 1, hwnd, NULL, hInst, NULL);
			hwndChild[1] = CreateWindowEx(WS_EX_CLIENTEDGE, _T("Child Window Class Name"), NULL, WS_CHILD | WS_VISIBLE, 0, rect.bottom / 2 + 1, rect.right, rect.bottom / 2, hwnd, NULL, hInst, NULL);

			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static RECT rect;
	static HDC hdc;
	PAINTSTRUCT ps;

	static bool dirX[2] = { true,true }, dirY[2] = { true,true };
	static int x[2], y[2];
	static int selection = -1;
	static bool flag[2] = { false,false };

	switch (iMsg)
	{
	case WM_CREATE:
		x[0] = y[0] = x[1] = y[1] = 50;
		GetClientRect(hwnd, &rect);
		break;
	case WM_LBUTTONDOWN:
		if (hwnd == hwndChild[0])
			selection = 0;
		else
			selection = 1;

		flag[selection] = flag[selection] ? false : true;

		if (flag[selection])
			SetTimer(hwndChild[selection], selection, 10, NULL);
		else if (!flag[selection])
			KillTimer(hwndChild[selection], selection);
		break;
	case WM_TIMER:
		if (x[wParam]<20 || x[wParam]>rect.right - 20) dirX[wParam] = dirX[wParam] ? false : true;
		if (y[wParam]<20 || y[wParam]>rect.bottom - 20) dirY[wParam] = dirY[wParam] ? false : true;

		if (dirX[wParam])
			x[wParam]++;
		else
			x[wParam]--;
		if (dirY[wParam])
			y[wParam]++;
		else
			y[wParam]--;
		InvalidateRgn(hwndChild[wParam], NULL, TRUE);

		break;
	case WM_PAINT:
		hdc = BeginPaint(hwndChild[0], &ps);
		Ellipse(hdc, x[0] - 20, y[0] - 20, x[0] + 20, y[0] + 20);
		EndPaint(hwndChild[0], &ps);

		hdc = BeginPaint(hwndChild[1], &ps);
		Ellipse(hdc, x[1] - 20, y[1] - 20, x[1] + 20, y[1] + 20);
		EndPaint(hwndChild[1], &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefMDIChildProc(hwnd, iMsg, wParam, lParam);
}