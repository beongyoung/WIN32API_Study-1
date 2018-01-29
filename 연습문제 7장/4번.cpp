#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<math.h>
//#include"resource.h"
//#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
HINSTANCE hInst;
HWND hwndChild[4];

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
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = _T("Window Class Name");
	RegisterClass(&WndClass);

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
	RECT rect;
	switch (iMsg)
	{
	case WM_CREATE:
		break;

	case WM_LBUTTONDOWN:
		GetClientRect(hwnd, &rect);
		hwndChild[0] = CreateWindowEx(WS_EX_CLIENTEDGE, _T("Child Window Class Name"), NULL, WS_CHILD | WS_VISIBLE, 0, 0, rect.right / 2 - 1, rect.bottom / 2 - 1, hwnd, NULL, hInst, NULL);
		hwndChild[1] = CreateWindowEx(WS_EX_CLIENTEDGE, _T("Child Window Class Name"), NULL, WS_CHILD | WS_VISIBLE, rect.right/2+1, 0, rect.right / 2 - 1, rect.bottom / 2 - 1, hwnd, NULL, hInst, NULL);
		hwndChild[2] = CreateWindowEx(WS_EX_CLIENTEDGE, _T("Child Window Class Name"), NULL, WS_CHILD | WS_VISIBLE, 0, rect.bottom/2+1, rect.right / 2 - 1, rect.bottom / 2 - 1, hwnd, NULL, hInst, NULL);
		hwndChild[3] = CreateWindowEx(WS_EX_CLIENTEDGE, _T("Child Window Class Name"), NULL, WS_CHILD | WS_VISIBLE, rect.right/2+1, rect.bottom / 2 + 1, rect.right / 2 - 1, rect.bottom / 2 - 1, hwnd, NULL, hInst, NULL);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{


	return DefMDIChildProc(hwnd, iMsg, wParam, lParam);
}