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

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMsg)
	{
	case WM_CREATE:
		break;

	case WM_LBUTTONDOWN:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG7), hwnd, DlgFunc);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

BOOL CALLBACK DlgFunc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hCombo, hList;
	static TCHAR InitBirth[20][10];
	static int sex = 1;
	TCHAR name[20], phone[20], birth[10];
	TCHAR InsertString[100];
	static int selection = -1;

	switch (iMsg)
	{
	case WM_INITDIALOG:
		hCombo = GetDlgItem(hDlg, IDC_COMBO1);
		hList = GetDlgItem(hDlg, IDC_LIST2);

		for (int i = 0; i < 20; i++)
		{
			_stprintf_s(InitBirth[i], 10, _T("%d"), 1990 + i);
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)InitBirth[i]);
		}
		CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_LIST2:
			if (HIWORD(wParam) == LBN_SELCHANGE)
				selection = SendMessage(hList, LB_GETCURSEL, 0, 0);
				break;
		case IDC_BUTTON1://새회원
			SetDlgItemText(hDlg, IDC_EDIT1, _T(""));
			SetDlgItemText(hDlg, IDC_EDIT2, _T(""));
			SendMessage(hCombo, CB_SETCURSEL, 0, 0);
			break;
		case IDC_BUTTON2://가입w2
			GetDlgItemText(hDlg, IDC_EDIT1, name, 20);
			GetDlgItemText(hDlg, IDC_EDIT2, phone, 20);
			GetDlgItemText(hDlg, IDC_COMBO1, birth, 10);
			_stprintf_s(InsertString, 100, _T("이름:%s, 전화번호:%s, 성별:%s, 출생연도:%s"), name, phone, sex == 1 ? _T("여자") : _T("남자"), birth);
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)InsertString);
			break;
		case IDC_BUTTON3://탈퇴
			SendMessage(hList, LB_DELETESTRING, selection, 0);
			break;
		case IDC_RADIO1:
			sex = 1;
			break;
		case IDC_RADIO2:
			sex = 2;
			break;
		case IDC_COMBO1:
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}

	}

	return 0;
}