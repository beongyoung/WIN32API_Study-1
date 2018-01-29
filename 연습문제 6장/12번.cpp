#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<math.h>
#include"resource.h"
#include<CommCtrl.h>
//#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgFunc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
HINSTANCE hInst;
void MakeColumn(HWND hDlg);
static int count = 0;

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
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG10), hwnd, DlgFunc);
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
	LPNMHDR hdr;
	switch (iMsg)
	{
	case WM_INITDIALOG:
		hCombo = GetDlgItem(hDlg, IDC_COMBO1);
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)_T("왼쪽"));
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)_T("오른쪽"));
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)_T("가운데"));
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDMAKE:
			MakeColumn(hDlg);
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return 0;
}
void MakeColumn(HWND hDlg)
{
	LVCOLUMN lvCol1 = { 0, };
	HWND hList, hCombo;
	RECT rect;
	//LPTSTR name[3] = { _T("이름"),_T("전화번호"),_T("주소") };
	TCHAR name[20];
	TCHAR sort[10];
	int size;

	hList = GetDlgItem(hDlg, IDC_LIST2);
	hCombo = GetDlgItem(hDlg, IDC_COMBO1);
	memset(&lvCol1, 0, sizeof(LVCOLUMN));
	GetClientRect(hList, &rect);
	lvCol1.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	GetDlgItemText(hDlg, IDC_EDIT1, name, 20);
	size = GetDlgItemInt(hDlg, IDC_EDIT2, NULL, TRUE);
	GetDlgItemText(hDlg, IDC_COMBO1, sort, 10);

	if (_tcscmp(sort, _T("왼쪽")) == 0)
		lvCol1.fmt = LVCFMT_LEFT;
	else if (_tcscmp(sort, _T("오른쪽")) == 0)
		lvCol1.fmt = LVCFMT_RIGHT;
	else if (_tcscmp(sort, _T("가운데")) == 0)
		lvCol1.fmt = LVCFMT_CENTER;

	lvCol1.cx = rect.right*(size / 10.0f);
	lvCol1.iSubItem = count;
	lvCol1.pszText = name;
	SendMessage(hList, LVM_INSERTCOLUMN, (WPARAM)0, (LPARAM)&lvCol1);


	SetDlgItemText(hDlg, IDC_EDIT1, _T(""));
	SetDlgItemText(hDlg, IDC_EDIT2, _T(""));
	SendMessage(hCombo, CB_SETCURSEL, 0, (LPARAM)0);
}
