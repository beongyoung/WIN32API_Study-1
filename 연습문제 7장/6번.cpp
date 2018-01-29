#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<math.h>
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
#define IDC_EDIT1 100
#define IDC_EDIT2 101
#define IDC_EDIT3 102
#define IDC_BUTTON1 103
#define IDC_BUTTON2 104
#define IDC_BUTTON3 105
#define IDC_BUTTON4 106
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	static HWND hEdit1, hEdit2, hEdit3,hButton1, hButton2, hButton3, hButton4;
	double num1, num2, result;
	TCHAR tmp[20];
	switch (iMsg)
	{
	case WM_CREATE:
		hEdit1 = (HWND)CreateWindow(_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 80, 70, 80, 20, hwnd, (HMENU)IDC_EDIT1, hInst, NULL);
		hEdit2 = (HWND)CreateWindow(_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 200, 70, 80, 20, hwnd, (HMENU)IDC_EDIT2, hInst, NULL);
		hEdit3 = (HWND)CreateWindow(_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 100, 170, 150, 20, hwnd, (HMENU)IDC_EDIT3, hInst, NULL);
		hButton1 = (HWND)CreateWindow(_T("button"), _T("+"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 100, 100, 50, 20, hwnd, (HMENU)IDC_BUTTON1, hInst, NULL);
		hButton2 = (HWND)CreateWindow(_T("button"), _T("-"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 200, 100, 50, 20, hwnd, (HMENU)IDC_BUTTON2, hInst, NULL);
		hButton3 = (HWND)CreateWindow(_T("button"), _T("X"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 100, 130, 50, 20, hwnd, (HMENU)IDC_BUTTON3, hInst, NULL);
		hButton4 = (HWND)CreateWindow(_T("button"), _T("/"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 200, 130, 50, 20, hwnd, (HMENU)IDC_BUTTON4, hInst, NULL);

		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON1:
			num1=GetDlgItemInt(hwnd, IDC_EDIT1, NULL, TRUE);
			num2 = GetDlgItemInt(hwnd, IDC_EDIT2, NULL, TRUE);
			result = num1 + num2;
			SetDlgItemInt(hwnd, IDC_EDIT3, result, TRUE);
			break;
		case IDC_BUTTON2:
			num1 = GetDlgItemInt(hwnd, IDC_EDIT1, NULL, TRUE);
			num2 = GetDlgItemInt(hwnd, IDC_EDIT2, NULL, TRUE);
			result = num1 - num2;
			SetDlgItemInt(hwnd, IDC_EDIT3, result, TRUE);
			break;
		case IDC_BUTTON3:
			num1 = GetDlgItemInt(hwnd, IDC_EDIT1, NULL, TRUE);
			num2 = GetDlgItemInt(hwnd, IDC_EDIT2, NULL, TRUE);
			result = num1 * num2;
			SetDlgItemInt(hwnd, IDC_EDIT3, result, TRUE);
			break;
		case IDC_BUTTON4:
			num1 = GetDlgItemInt(hwnd, IDC_EDIT1, NULL, TRUE);
			num2 = GetDlgItemInt(hwnd, IDC_EDIT2, NULL, TRUE);
			result = num1 / num2;
			_stprintf_s(tmp, 20, _T("%lf"), result);
			SetDlgItemText(hwnd, IDC_EDIT3, tmp);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}