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
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, DlgFunc);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
BOOL CALLBACK DlgFunc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hButton;
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rect;
	static int X, Y;
	static BOOL dirX = true, dirY = true;

	switch (iMsg)
	{
	case WM_INITDIALOG:
		GetClientRect(hDlg, &rect);
		X = 10;
		Y = 50;
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_START:
			SetTimer(hDlg, 1, 10, NULL);
			hButton = GetDlgItem(hDlg, IDC_BUTTON_START);
			EnableWindow(hButton, FALSE);
			hButton = GetDlgItem(hDlg, IDC_BUTTON_STOP);
			EnableWindow(hButton, TRUE);
			break;
		case IDC_BUTTON_STOP:
			KillTimer(hDlg, 1);
			hButton = GetDlgItem(hDlg, IDC_BUTTON_STOP);
			EnableWindow(hButton, FALSE);
			hButton = GetDlgItem(hDlg, IDC_BUTTON_START);
			EnableWindow(hButton, TRUE);
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		case IDCLOSE:
			EndDialog(hDlg, 0);
			break;
		}
	case WM_TIMER:
		if (X < 0 || X > rect.right) dirX = dirX ? false : true;
		if (Y < 0 || Y > rect.bottom) dirY = dirY ? false : true;

		if (dirX) X++;
		else X--;
		if (dirY) Y--;
		else Y++;
		InvalidateRgn(hDlg, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hDlg, &ps);
		Ellipse(hdc, X - 20, Y - 20, X + 20, Y + 20);
		EndPaint(hDlg, &ps);
		break;
	}
	return 0;
}