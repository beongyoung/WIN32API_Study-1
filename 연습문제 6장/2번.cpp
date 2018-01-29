#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<math.h>
#include"resource.h"
#include<stdio.h>
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
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hwnd, DlgFunc);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
BOOL CALLBACK DlgFunc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR str[200];
	static int check[3] = { 0, };
	static int radio[2] = { 1,0 };
	TCHAR hobby[3][20] = { _T("독서"),_T("음악감상"),_T("인터넷") };
	TCHAR sex[2][20] = { _T("여자"),_T("남자") };

	switch (iMsg)
	{
	case WM_INITDIALOG:
		CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_CHECK1:
			check[0] = 1- check[0];
			break;
		case IDC_CHECK2:
			check[1] = 1- check[1];
			break;
		case IDC_CHECK3:
			check[2] = 1- check[2];
			break;
		case IDC_RADIO1:
			radio[0] = 1;
			radio[1] = 0;
			break;
		case IDC_RADIO2:
			radio[0] = 0;
			radio[1] = 1;
			break;
		case IDC_BUTTON_PRINT:
			_stprintf_s(str, _T("취미는 %s%s%s%s%s입니다\r\n")_T("성별은 %s입니다"),
				check[0] ? hobby[0]: _T(""), check[0] &&check[1]|| check[0] && check[2] ? _T(",") : _T(""), check[1] ? hobby[1] : _T(""), check[1]&&check[2] ? _T(",") : _T(""), check[2] ? hobby[2] : _T(""),
				radio[0] ? sex[0] : sex[1]);
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDCLOSE:
			check[0] = 0;
			check[1] = 0;
			check[2] = 0;
			radio[0] = 1;
			radio[1] = 0;
			EndDialog(hDlg, 0);
			break;
		case IDCANCEL:
			check[0] = 0;
			check[1] = 0;
			check[2] = 0;
			radio[0] = 1;
			radio[1] = 0;
			EndDialog(hDlg, 0);
			break;
		}
		break;

	}
	return 0;
}