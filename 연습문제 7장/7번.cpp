#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<math.h>
//#include"resource.h"
//#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
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
#define IDC_COMBO1 100
#define IDC_COMBO2 101
#define IDC_COMBO3 102
#define IDC_EDIT1 103
#define IDC_EDIT2 104
#define IDC_BUTTON 105

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR str[20], editstr[20], tmp1[20], tmp2[20], tmp3[20];

	static HWND hCombo1, hCombo2, hCombo3, hEdit1, hEdit2,hEnrollButton;

	switch (iMsg)
	{
	case WM_CREATE:
		hCombo1 = (HWND)CreateWindow(_T("combobox"), NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | CBS_NOINTEGRALHEIGHT, 100, 100, 100, 400, hwnd, (HMENU)IDC_COMBO1, hInst, NULL);
		hCombo2 = (HWND)CreateWindow(_T("combobox"), NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | CBS_NOINTEGRALHEIGHT, 200, 100, 100, 300, hwnd, (HMENU)IDC_COMBO2, hInst, NULL);
		hCombo3 = (HWND)CreateWindow(_T("combobox"), NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | CBS_NOINTEGRALHEIGHT, 300, 100, 100, 600, hwnd, (HMENU)IDC_COMBO3, hInst, NULL);
		hEdit1 = (HWND)CreateWindow(_T("edit"), NULL, WS_CHILD | WS_VISIBLE | ES_READONLY | WS_BORDER, 130, 200, 100, 20, hwnd, (HMENU)IDC_EDIT1, hInst, NULL);
		hEdit2 = (HWND)CreateWindow(_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 270, 200, 100, 20, hwnd, (HMENU)IDC_EDIT2, hInst, NULL);
		hEnrollButton = (HWND)CreateWindow(_T("button"), _T("주민번호입력"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 400, 100, 120, 20, hwnd, (HMENU)IDC_BUTTON, hInst, NULL);

		for (int i = 0; i < 20; i++)
		{
			_stprintf_s(str, 20, _T("%d"), 1980 + i);
			SendMessage(hCombo1, CB_ADDSTRING, 0, (LPARAM)str);
		}
		for (int i = 1; i <= 12; i++)
		{
			_stprintf_s(str, 20, _T("%d월"), i);
			SendMessage(hCombo2, CB_ADDSTRING, 0, (LPARAM)str);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_COMBO2:
			switch (1 + SendMessage(hCombo2, CB_GETCURSEL, 0, 0))
			{
			case 1:
				SendMessage(hCombo3, CB_RESETCONTENT, 0, 0);
				for (int i = 1; i <= 31; i++)
				{
					_stprintf_s(str, 20, _T("%d"), i);
					SendMessage(hCombo3, CB_ADDSTRING, 0, (LPARAM)str);
				}
				break;
			case 2:
				SendMessage(hCombo3, CB_RESETCONTENT, 0, 0);
				for (int i = 1; i <= 28; i++)
				{
					_stprintf_s(str, 20, _T("%d"), i);
					SendMessage(hCombo3, CB_ADDSTRING, 0, (LPARAM)str);
				}
				break;
			case 3:
				SendMessage(hCombo3, CB_RESETCONTENT, 0, 0);
				for (int i = 1; i <= 31; i++)
				{
					_stprintf_s(str, 20, _T("%d"), i);
					SendMessage(hCombo3, CB_ADDSTRING, 0, (LPARAM)str);
				}
				break;
			case 4:
				SendMessage(hCombo3, CB_RESETCONTENT, 0, 0);
				for (int i = 1; i <= 30; i++)
				{
					_stprintf_s(str, 20, _T("%d"), i);
					SendMessage(hCombo3, CB_ADDSTRING, 0, (LPARAM)str);
				}
				break;
			case 5:
				SendMessage(hCombo3, CB_RESETCONTENT, 0, 0);
				for (int i = 1; i <= 31; i++)
				{
					_stprintf_s(str, 20, _T("%d"), i);
					SendMessage(hCombo3, CB_ADDSTRING, 0, (LPARAM)str);
				}
				break;
			case 6:
				SendMessage(hCombo3, CB_RESETCONTENT, 0, 0);
				for (int i = 1; i <= 30; i++)
				{
					_stprintf_s(str, 20, _T("%d"), i);
					SendMessage(hCombo3, CB_ADDSTRING, 0, (LPARAM)str);
				}
				break;
			case 7:
				SendMessage(hCombo3, CB_RESETCONTENT, 0, 0);
				for (int i = 1; i <= 31; i++)
				{
					_stprintf_s(str, 20, _T("%d"), i);
					SendMessage(hCombo3, CB_ADDSTRING, 0, (LPARAM)str);
				}
				break;
			case 8:
				SendMessage(hCombo3, CB_RESETCONTENT, 0, 0);
				for (int i = 1; i <= 31; i++)
				{
					_stprintf_s(str, 20, _T("%d"), i);
					SendMessage(hCombo3, CB_ADDSTRING, 0, (LPARAM)str);
				}
				break;
			case 9:
				SendMessage(hCombo3, CB_RESETCONTENT, 0, 0);
				for (int i = 1; i <= 30; i++)
				{
					_stprintf_s(str, 20, _T("%d"), i);
					SendMessage(hCombo3, CB_ADDSTRING, 0, (LPARAM)str);
				}
				break;
			case 10:
				SendMessage(hCombo3, CB_RESETCONTENT, 0, 0);
				for (int i = 1; i <= 31; i++)
				{
					_stprintf_s(str, 20, _T("%d"), i);
					SendMessage(hCombo3, CB_ADDSTRING, 0, (LPARAM)str);
				}
				break;
			case 11:
				SendMessage(hCombo3, CB_RESETCONTENT, 0, 0);
				for (int i = 1; i <= 30; i++)
				{
					_stprintf_s(str, 20, _T("%d"), i);
					SendMessage(hCombo3, CB_ADDSTRING, 0, (LPARAM)str);
				}
				break;
			case 12:
				SendMessage(hCombo3, CB_RESETCONTENT, 0, 0);
				for (int i = 1; i <= 31; i++)
				{
					_stprintf_s(str, 20, _T("%d"), i);
					SendMessage(hCombo3, CB_ADDSTRING, 0, (LPARAM)str);
				}
				break;
			}
			break;
		case IDC_BUTTON:
				SetDlgItemText(hwnd, IDC_EDIT1, _T(""));
				editstr[0] = '\0';
				GetDlgItemText(hwnd, IDC_COMBO1, tmp1, 20);
				GetDlgItemText(hwnd, IDC_COMBO2, tmp2, 20);
				GetDlgItemText(hwnd, IDC_COMBO3, tmp3, 20);
				editstr[0] = tmp1[2];
				editstr[1] = tmp1[3];
				if (_tcslen(tmp2) == 2)
				{
					editstr[2] = '0';
					editstr[3] = tmp2[0];
				}
				else
				{
					editstr[2] = tmp2[0];
					editstr[3] = tmp2[1];
				}
				if (_tcslen(tmp3) == 1)
				{
					editstr[4] = '0';
					editstr[5] = tmp3[0];
				}
				else
				{
					editstr[4] = tmp3[0];
					editstr[5] = tmp3[1];
				}
				editstr[6] = '\0';
				SetDlgItemText(hwnd, IDC_EDIT1, editstr);
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		TextOut(hdc, 120, 70, _T("연도선택"), _tcsclen(_T("연도선택")));
		TextOut(hdc, 220, 70, _T("월 선택"), _tcsclen(_T("월 선택")));
		TextOut(hdc, 320, 70, _T("일 선택"), _tcsclen(_T("일 선택")));
		TextOut(hdc, 130, 170, _T("주민번호 앞자리 - 주민번호 뒷자리"), _tcsclen(_T("주민번호 앞자리 - 주민번호 뒷자리")));
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}