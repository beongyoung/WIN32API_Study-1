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
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG9), hwnd, DlgFunc);
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
		MakeColumn(hDlg);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
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
	LVCOLUMN lvCol1 = { 0, }, lvCol2 = { 0, }, lvCol3 = { 0, };
	HWND hList;
	RECT rect;
	LPTSTR name[3] = { _T("이름"),_T("전화번호"),_T("주소") };

	hList = GetDlgItem(hDlg, IDC_LIST1);
	memset(&lvCol1, 0, sizeof(LVCOLUMN));
	memset(&lvCol2, 0, sizeof(LVCOLUMN));
	memset(&lvCol3, 0, sizeof(LVCOLUMN));
	GetClientRect(hList, &rect);
	lvCol1.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol2.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol3.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol1.fmt = LVCFMT_LEFT;
	lvCol2.fmt = LVCFMT_RIGHT;
	lvCol3.fmt = LVCFMT_CENTER;
	

	lvCol1.cx = rect.right*(1/5.0f);
	lvCol1.iSubItem = 0;
	lvCol1.pszText = name[0];
	SendMessage(hList, LVM_INSERTCOLUMN, (WPARAM)0, (LPARAM)&lvCol1);

	lvCol2.cx = rect.right*(3 / 10.0f);
	lvCol2.iSubItem = 1;
	lvCol2.pszText = name[1];
	SendMessage(hList, LVM_INSERTCOLUMN, (WPARAM)1, (LPARAM)&lvCol2);

	lvCol3.cx = rect.right*(5 / 10.0f);
	lvCol3.iSubItem = 2;
	lvCol3.pszText = name[2];
	SendMessage(hList, LVM_INSERTCOLUMN, (WPARAM)2, (LPARAM)&lvCol3);

}
