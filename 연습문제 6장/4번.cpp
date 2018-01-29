#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<math.h>
#include"resource.h"
#include<stdio.h>
#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
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
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG4), hwnd, DlgFunc);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

BOOL CALLBACK DlgFunc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static TCHAR text[30];
	static int count = 0, count2 = 1;
	static double num1, num2;
	static double result;
	static bool ClickPoint = false;
	static bool compute = false;
	TCHAR temp[5];
	switch (iMsg)
	{
	case WM_INITDIALOG:
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON1:
			text[count++] += '1';
			SetDlgItemText(hDlg, IDC_EDIT1, text);
			if (ClickPoint)
			{
				if (compute)
					num2 += 1 / (float)pow(10, count2++);
				else
					num1 += 1 / (float)pow(10, count2++);
			}
			else
			{
				if (compute)
					num2 = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
				else
					num1 = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
			}
			break;
		case IDC_BUTTON2:
			text[count++] += '2';
			SetDlgItemText(hDlg, IDC_EDIT1, text);
			if (ClickPoint)
			{
				if (compute)
					num2 += 2 / (float)pow(10, count2++);
				else
					num1 += 2 / (float)pow(10, count2++);
			}
			else
			{
				if (compute)
					num2 = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
				else
					num1 = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
			}
			break;
		case IDC_BUTTON3:
			text[count++] += '3';
			SetDlgItemText(hDlg, IDC_EDIT1, text);
			if (ClickPoint)
			{
				if (compute)
					num2 += 3 / (float)pow(10, count2++);
				else
					num1 += 3 / (float)pow(10, count2++);
			}
			else
			{
				if (compute)
					num2 = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
				else
					num1 = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
			}
			break;
		case IDC_BUTTON4:
			text[count++] += '4';
			SetDlgItemText(hDlg, IDC_EDIT1, text);
			if (ClickPoint)
			{
				if (compute)
					num2 += 4 / (float)pow(10, count2++);
				else
					num1 += 4 / (float)pow(10, count2++);
			}
			else
			{
				if (compute)
					num2 = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
				else
					num1 = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
			}
			break;
		case IDC_BUTTON5:
			text[count++] += '5';
			SetDlgItemText(hDlg, IDC_EDIT1, text);
			if (ClickPoint)
			{
				if (compute)
					num2 += 5 / (float)pow(10, count2++);
				else
					num1 += 5 / (float)pow(10, count2++);
			}
			else
			{
				if (compute)
					num2 = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
				else
					num1 = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
			}
			break;
		case IDC_BUTTON6:
			text[count++] += '6';
			SetDlgItemText(hDlg, IDC_EDIT1, text);
			if (ClickPoint)
			{
				if (compute)
					num2 += 6 / (float)pow(10, count2++);
				else
					num1 += 6 / (float)pow(10, count2++);
			}
			else
			{
				if (compute)
					num2 = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
				else
					num1 = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
			}
			break;
		case IDC_BUTTON7:
			text[count++] += '7';
			SetDlgItemText(hDlg, IDC_EDIT1, text);
			if (ClickPoint)
			{
				if (compute)
					num2 += 7 / (float)pow(10, count2++);
				else
					num1 += 7 / (float)pow(10, count2++);
			}
			else
			{
				if (compute)
					num2 = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
				else
					num1 = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
			}
			break;
		case IDC_BUTTON8:
			text[count++] += '8';
			SetDlgItemText(hDlg, IDC_EDIT1, text);
			if (ClickPoint)
			{
				if (compute)
					num2 += 8 / (float)pow(10, count2++);
				else
					num1 += 8 / (float)pow(10, count2++);
			}
			else
			{
				if (compute)
					num2 = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
				else
					num1 = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
			}
			break;
		case IDC_BUTTON9:
			text[count++] += '9';
			SetDlgItemText(hDlg, IDC_EDIT1, text);
			if (ClickPoint)
			{
				if (compute)
					num2 += 9 / (float)pow(10, count2++);
				else
					num1 += 9 / (float)pow(10, count2++);
			}
			else
			{
				if (compute)
					num2 = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
				else
					num1 = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
			}
			break;
		case IDC_BUTTON0:
			text[count++] += '0';
			SetDlgItemText(hDlg, IDC_EDIT1, text);
			if (ClickPoint)
			{
				if (compute)
					num2 += 0 / (float)pow(10, count2++);
				else
					num1 += 0 / (float)pow(10, count2++);
			}
			else
			{
				if (compute)
					num2 = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
				else
					num1 = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
			}
			break;
		case IDC_BUTTON_PLU:
			compute = true;
			ClickPoint = false;
			SetDlgItemText(hDlg, IDC_EDIT1, _T(""));
			SetDlgItemText(hDlg, IDC_EDIT2, _T("+"));
			for (int i = 0; i < 30; i++)
				text[i] = NULL;
			count = 0;
			count2 = 1;
			break;
		case IDC_BUTTON_MIN:
			compute = true;
			ClickPoint = false;
			SetDlgItemText(hDlg, IDC_EDIT1, _T(""));
			SetDlgItemText(hDlg, IDC_EDIT2, _T("-"));
			for (int i = 0; i < 30; i++)
				text[i] = NULL;
			count = 0;
			count2 = 1;
			break;
		case IDC_BUTTON_DIV:
			compute = true;
			ClickPoint = false;
			SetDlgItemText(hDlg, IDC_EDIT1, _T(""));
			SetDlgItemText(hDlg, IDC_EDIT2, _T("/"));
			for (int i = 0; i < 30; i++)
				text[i] = NULL;
			count = 0;
			count2 = 1;
			break;
		case IDC_BUTTON_MULT:
			compute = true;
			ClickPoint = false;
			SetDlgItemText(hDlg, IDC_EDIT1, _T(""));
			SetDlgItemText(hDlg, IDC_EDIT2, _T("*"));
			for (int i = 0; i < 30; i++)
				text[i] = NULL;
			count = 0;
			count2 = 1;
			break;
		case IDC_BUTTON_EQUAL:
			for (int i = 0; i < 30; i++)
				text[i] = NULL;
			GetDlgItemText(hDlg, IDC_EDIT2, temp, 5);
			if (_tcscmp(temp, _T("+")) == 0)
			{
				result = num1 + num2;
			}
			else if (_tcscmp(temp, _T("-")) == 0)
			{
				result = num1 - num2;
			}
			else if (_tcscmp(temp, _T("/")) == 0)
			{
				result = num1 / num2;
			}
			else if (_tcscmp(temp, _T("*")) == 0)
			{
				result = num1 * num2;
			}
			_stprintf_s(text, _T("%lf"), result);
			SetDlgItemText(hDlg, IDC_EDIT1, text);
			break;
		case IDC_BUTTON_POINT:
			if (!ClickPoint)
			{
				ClickPoint = true;
				text[count++] += '.';
				SetDlgItemText(hDlg, IDC_EDIT1, text);
			}
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