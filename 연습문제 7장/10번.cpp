#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<math.h>
#include"resource.h"
#include<stdio.h>
//#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgFunc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
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
		_T("박정호"),
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
#define IDC_BUTTON 100
HDC hdc;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	static HWND hButton;
	hdc = GetDC(hwnd);
	switch (iMsg)
	{
	case WM_CREATE:
		hButton = (HWND)CreateWindow(_T("button"), _T("회원 입력"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 100, 20, hwnd, (HMENU)IDC_BUTTON, hInst, NULL);
		break;
	case WM_COMMAND:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, DlgFunc);
		
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		ReleaseDC(hwnd, hdc);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
BOOL CALLBACK DlgFunc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hCombo;
	TCHAR tmp1[20], tmp2[20], tmp3[20];
	TCHAR str[100];
	bool sex = -1;
	static int line = 20;
	switch (iMsg)
	{
	case WM_INITDIALOG:
		hCombo = GetDlgItem(hDlg, IDC_COMBO1);
		for (int i = 0; i < 20; i++)
		{
			_stprintf_s(tmp1, 20, _T("%d"), 1980 + i);
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)tmp1);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			GetDlgItemText(hDlg, IDC_EDIT1, tmp1, 20);
			GetDlgItemText(hDlg, IDC_EDIT2, tmp2, 20);
			GetDlgItemText(hDlg, IDC_COMBO1, tmp3, 20);
			_stprintf_s(str, 100, _T("이름:%s, 전화번호: %s, 성별: %s, 출생연도:%s"), tmp1, tmp2, sex ? _T("여자") : _T("남자"), tmp3);
			TextOut(hdc, 0, line, str,_tcslen(str));
			line += 20;
			break;
		case IDC_RADIO1:
			sex = true;
			break;
		case IDC_RADIO2:
			sex = false;
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	case WM_DESTROY:
		EndDialog(hDlg, 0);
		break;
	}

	return 0;
}