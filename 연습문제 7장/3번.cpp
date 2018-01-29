#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<math.h>
#include"resource.h"
//#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
HINSTANCE hInst;

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.lpszClassName = _T("Window Class Name");
	RegisterClass(&WndClass);

	WndClass.lpfnWndProc = ChildWndProc;
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = _T("Child Window Class Name");
	RegisterClass(&WndClass);

	hwnd = CreateWindow(
		_T("Window Class Name"),
		_T("Main Window Title"),
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

HWND ActivatedHwndChild;

int selection = -1;
int childcount = 0;
static struct CHILD
{
	HWND hwndChild;
	TCHAR str[10][100] = { '\0', };
	int count = 0, line = 0;
};
CHILD child[10];
HWND hwndChild[10] = { 0, };
TCHAR str[10][100] = { '\0', };
int count = 0, line =0;
LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndClient;
	

	CLIENTCREATESTRUCT clientcreate;
	MDICREATESTRUCT mdicreate;

	switch (iMsg)
	{
	case WM_CREATE:
		clientcreate.hWindowMenu = GetSubMenu(GetMenu(hwnd), 0);
		clientcreate.idFirstChild = 100;
		hwndClient = CreateWindow(_T("MDICLIENT"), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
			0, 0, 0, 0, hwnd, NULL, hInst, (LPSTR)&clientcreate);
		ShowWindow(hwndClient, SW_SHOW);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILENEW:
			mdicreate.szClass = _T("Child Window Class Name");
			mdicreate.szTitle = _T("NoName");
			mdicreate.hOwner = hInst;
			mdicreate.style = 0;
			mdicreate.lParam = 0;
			mdicreate.x = CW_USEDEFAULT;
			mdicreate.y = CW_USEDEFAULT;
			mdicreate.cx = CW_USEDEFAULT;
			mdicreate.cy = CW_USEDEFAULT;
			child[childcount++].hwndChild = (HWND)SendMessage(hwndClient, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT)&mdicreate);

			break;
		
		}
	case WM_MOUSEACTIVATE:
		ActivatedHwndChild = (HWND)SendMessage(hwndClient, WM_MDIGETACTIVE, 0, 0);
		for (int i = 0; i < childcount; i++)
		{
			if (ActivatedHwndChild == child[i].hwndChild)
			{
				selection = i;
				break;
			}
		}
		break;
		
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefFrameProc(hwnd, hwndClient, iMsg, wParam, lParam);
}

LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR tmp[20];
	switch (iMsg)
	{
	case WM_CREATE:
		_stprintf_s(tmp, 20, _T("%d"),wParam);
		MessageBox(hwnd, tmp, _T(""), MB_OK);
		break;
	case WM_CHAR:
		if (wParam == VK_RETURN)
		{
			child[selection].line++;
			child[selection].count = 0;
		}
		else if (wParam == VK_BACK)
		{

		}
		else
		{
			child[selection].str[child[selection].line][child[selection].count++] = wParam;

		}
		InvalidateRgn(ActivatedHwndChild, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(ActivatedHwndChild, &ps);
		for (int i = 0; i <= child[selection].line; i++)
			TextOut(hdc, 0, i * 20, child[selection].str[i], _tcslen(child[selection].str[i]));
		EndPaint(ActivatedHwndChild, &ps);
		break;
	case WM_DESTROY:
		break;
	}
	return DefMDIChildProc(hwnd, iMsg, wParam, lParam);
}