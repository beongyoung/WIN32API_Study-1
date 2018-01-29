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
void InsertData(HWND hDlg);
int SelectItem(HWND hDlg, LPARAM lParam);
void DeleteData(HWND hDlg, int selection);
void ModifyData(HWND hDlg, int selection);

static int SEX = 1;
static bool trigger = false;
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
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG8), hwnd, DlgFunc);
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
	TCHAR InsertString[100];
	static int selection = -1;
	LPNMHDR hdr;

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
		MakeColumn(hDlg);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON1://새회원
			SetDlgItemText(hDlg, IDC_EDIT1, _T(""));
			SetDlgItemText(hDlg, IDC_EDIT2, _T(""));
			SendMessage(hCombo, CB_SETCURSEL, 0, 0);
			break;
		case IDC_BUTTON2://가입
			InsertData(hDlg);
			break;
		case IDC_BUTTON3://탈퇴
			DeleteData(hDlg, selection);
			break;
		case IDC_BUTTON6://수정
			trigger = true;
			if (selection == -1)break;
			ModifyData(hDlg, selection);
			trigger = false;
			selection = -1;
			break;
		case IDC_RADIO1:
			SEX = 1;
			break;
		case IDC_RADIO2:
			SEX = 2;
			break;
		case IDC_COMBO1:
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	case WM_NOTIFY:
		hdr = (LPNMHDR)lParam;
		hList = GetDlgItem(hDlg, IDC_LIST2);
		if (hdr->hwndFrom == hList&&hdr->code == LVN_ITEMCHANGED&&trigger == false)
			selection = SelectItem(hDlg, lParam);
		break;
	}

	return 0;
}
void MakeColumn(HWND hDlg)
{
	LVCOLUMN lvCol = { 0, };
	HWND hList;
	RECT rect;
	LPTSTR name[4] = { _T("이름"),_T("전화번호"),_T("성별"),_T("출생연도") };

	hList = GetDlgItem(hDlg, IDC_LIST2);
	memset(&lvCol, 0, sizeof(LVCOLUMN));
	GetClientRect(hList, &rect);
	lvCol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt = LVCFMT_LEFT;

	for (int i = 0; i < 4; i++)
	{
		lvCol.cx = rect.right / 4;
		lvCol.iSubItem = i;
		lvCol.pszText = name[i];
		SendMessage(hList, LVM_INSERTCOLUMN, (WPARAM)i, (LPARAM)&lvCol);
	}
}
void InsertData(HWND hDlg)
{

	TCHAR name[20], phone[20], birth[10];
	HWND hCombo;
	LVITEM item;
	int count;
	HWND hList;
	hList = GetDlgItem(hDlg, IDC_LIST2);
	hCombo = GetDlgItem(hDlg, IDC_COMBO1);
	count = ListView_GetItemCount(hList);

	GetDlgItemText(hDlg, IDC_EDIT1, name, 20);
	if (_tcscmp(name, _T("")) == 0) return;
	SetDlgItemText(hDlg, IDC_EDIT1, _T(""));
	GetDlgItemText(hDlg, IDC_EDIT2, phone, 20);
	SetDlgItemText(hDlg, IDC_EDIT2, _T(""));
	GetDlgItemText(hDlg, IDC_COMBO1, birth, 10);
	SetDlgItemText(hDlg, IDC_COMBO1, _T(""));
	SendMessage(hCombo, CB_SETCURSEL, 0, 0);

	item.mask = LVIF_TEXT;
	item.iItem = count;
	item.iSubItem = 0;
	item.pszText = name;
	ListView_InsertItem(hList, &item);
	ListView_SetItemText(hList, count, 1, phone);
	ListView_SetItemText(hList, count, 2, SEX == 1 ? _T("여자") : _T("남자"));
	ListView_SetItemText(hList, count, 3, birth);
}
int SelectItem(HWND hDlg, LPARAM lParam)
{
	
	LPNMLISTVIEW nlv;
	HWND hList, hCombo;
	TCHAR name[20], phone[20], birth[20], sex[20];
	hCombo = GetDlgItem(hDlg, IDC_COMBO1);
	hList = GetDlgItem(hDlg, IDC_LIST2);
	int count;

	nlv = (LPNMLISTVIEW)lParam;
	ListView_GetItemText(hList, nlv->iItem, 0, name, 20);
	SetDlgItemText(hDlg, IDC_EDIT1, name);
	ListView_GetItemText(hList, nlv->iItem, 1, phone, 20);
	SetDlgItemText(hDlg, IDC_EDIT2, phone);
	ListView_GetItemText(hList, nlv->iItem, 2, sex, 20);
	if (_tcscmp(sex, _T("여자")) == 0)
	{
		CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
		SEX = 1;
	}
	
	else
	{
		CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
		SEX = 2;
	}
	ListView_GetItemText(hList, nlv->iItem, 3, birth, 20);
	count = SendMessage(hCombo, CB_FINDSTRING, 0, (LPARAM)birth);
	SendMessage(hCombo, CB_SETCURSEL, count, 0);

	return nlv->iItem;
}
void DeleteData(HWND hDlg, int selection)
{
	HWND hList, hCombo;

	hList = GetDlgItem(hDlg, IDC_LIST2);
	hCombo = GetDlgItem(hDlg, IDC_COMBO1);
	ListView_DeleteItem(hList, selection);
	SetDlgItemText(hDlg, IDC_EDIT1, _T(""));
	SetDlgItemText(hDlg, IDC_EDIT2, _T(""));
	SendMessage(hCombo, CB_SETCURSEL, 0, 0);
}
void ModifyData(HWND hDlg, int selection)
{
	TCHAR name[20], phone[20], birth[20], sex[20];
	HWND hList, hCombo;
	int count;
	hCombo = GetDlgItem(hDlg, IDC_COMBO1);
	hList = GetDlgItem(hDlg, IDC_LIST2);
	GetDlgItemText(hDlg, IDC_EDIT1, name, 20);
	if (_tcscmp(name, _T("")) == 0) return;
	GetDlgItemText(hDlg, IDC_EDIT2, phone, 20);
	GetDlgItemText(hDlg, IDC_COMBO1, birth, 10);
	ListView_SetItemText(hList, selection, 0, name);
	ListView_SetItemText(hList, selection, 1, phone);

	if (SEX == 1)
	{
		ListView_SetItemText(hList, selection, 2, _T("여자"), 20);
	}
	else if (SEX == 2)
	{
		ListView_SetItemText(hList, selection, 2, _T("남자"), 20);
	}

	//ListView_SetItemText(hList, selection, 2, SEX == 1 ? _T("여자") : _T("남자"));

	ListView_SetItemText(hList, selection, 3, birth);
	SetDlgItemText(hDlg, IDC_EDIT1, name);
	SetDlgItemText(hDlg, IDC_EDIT2, phone);
	count = SendMessage(hCombo, CB_FINDSTRING, 0, (LPARAM)birth);
	SendMessage(hCombo, CB_SETCURSEL, count, 0);
}