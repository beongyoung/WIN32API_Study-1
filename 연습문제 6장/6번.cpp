#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<math.h>
#include"resource.h"
//#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgFunc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMsg)
	{
	case WM_CREATE:
		break;

	case WM_LBUTTONDOWN:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG5), hwnd, DlgFunc);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

BOOL CALLBACK DlgFunc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int select = -1;
	static int centerX = 50, centerY = 50;
	static int mx, my;
	static bool line = false, ClickOK = false;
	switch (iMsg)
	{
	case WM_INITDIALOG:

		break;
	case WM_LBUTTONDOWN:
		if (ClickOK)
		{
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			line = true;
			InvalidateRgn(hDlg, NULL, TRUE);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_RADIO1:
			select = 0;
			break;
		case IDC_RADIO2:
			select = 1;
			break;
		case IDC_BUTTON_MOVE:
			SetTimer(hDlg, 1, 10, NULL);
			break;
		case IDOK:
			ClickOK = true;
			InvalidateRgn(hDlg, NULL, TRUE);
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
	case WM_TIMER:
		if (centerX < mx)
			centerX++;
		else
			centerX--;

		if (centerY < my)
			centerY++;
		else
			centerY--;
		InvalidateRgn(hDlg, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hDlg, &ps);
		if (select == 0)
		{
			Rectangle(hdc, centerX - 50, centerY - 50, centerX + 50, centerY + 50);
		}
		else if (select == 1)
		{
			Ellipse(hdc, centerX - 50, centerY - 50, centerX + 50, centerY + 50);
		}
		if (line)
		{
			MoveToEx(hdc, centerX, centerY, NULL);
			LineTo(hdc, mx, my);
		}
		if (centerX == mx&&centerY == my)
			KillTimer(hDlg, 1);
		EndPaint(hDlg, &ps);
		break;
	}

	return 0;
}