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
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, DlgFunc);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

BOOL CALLBACK DlgFunc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR text[30];
	int num1, num2;
	double result;
	switch (iMsg)
	{
	case WM_INITDIALOG:
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_PLUS:
			num1 = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
			num2 = GetDlgItemInt(hDlg, IDC_EDIT2, NULL, TRUE);
			result = num1 + num2;
			SetDlgItemInt(hDlg, IDC_EDIT3, result, 1);
			break;
		case IDC_BUTTON_MINUS:
			num1 = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
			num2 = GetDlgItemInt(hDlg, IDC_EDIT2, NULL, TRUE);
			result = num1 - num2;
			SetDlgItemInt(hDlg, IDC_EDIT3, result, 1);
			break;
		case IDC_BUTTON_MULTI:
			num1 = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
			num2 = GetDlgItemInt(hDlg, IDC_EDIT2, NULL, TRUE);
			result = num1 * num2;
			SetDlgItemInt(hDlg, IDC_EDIT3, result, 1);
			break;
		case IDC_BUTTON_DIVISE:
			num1 = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
			num2 = GetDlgItemInt(hDlg, IDC_EDIT2, NULL, TRUE);
			result = num1 / (float)num2;
			_stprintf_s(text, _T("%lf"), result);
			SetDlgItemText(hDlg, IDC_EDIT3, text);
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		case IDCLOSE:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return 0;
}