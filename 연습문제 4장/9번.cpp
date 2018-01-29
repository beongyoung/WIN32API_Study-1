#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<math.h>
#include"resource.h"
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;

	WNDCLASS WndClass;
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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);

	RegisterClass(&WndClass);

	hwnd = CreateWindow(
		_T("Window Class Name"),
		_T("홍길동의 첫 번째 윈도우"),
		WS_OVERLAPPEDWINDOW,
		100,
		100,
		1000,
		1000,
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
	HPEN hPen, orgPen;
	HBRUSH hBrush, orgBrush;
	CHOOSECOLOR COLOR;
	static RECT rect;
	static SIZE size;
	static COLORREF temp[16], PenColor = RGB(0, 0, 0), FaceColor = RGB(255, 255, 255);

	static bool isClick = false;
	static int mX1, mY1, mX2, mY2, count = 0, line = 0;
	static int Figure = 0;
	static TCHAR str[20][200];

	switch (iMsg)
	{
	case WM_CREATE:
		break;
	case WM_CHAR:
		hdc = GetDC(hwnd);
		GetTextExtentPoint(hdc, str[line], _tcslen(str[line]), &size);

		if (wParam == VK_RETURN)
		{
			//str[line][count] = '\n';
			line++;
			count = 0;
			rect.top += 20;
		}
		else str[line][count++] = wParam;
		if (size.cx >= rect.right - rect.left - 10)
		{
			//str[line][count] = '\n';
			line++;
			count = 0;
			rect.top += 20;
		}
		DrawText(hdc, str[line], _tcslen(str[line]), &rect, DT_LEFT | DT_TOP);

		//SetCaretPos(rect.left+size.cx, rect.top+line*20);
		ReleaseDC(hwnd, hdc);
		break;
	case  WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_PENCOLOR:
			for (int i = 0; i < 16; i++)
				temp[i] = RGB(rand() % 256, rand() % 256, rand() % 256);
			memset(&COLOR, 0, sizeof(CHOOSECOLOR));
			COLOR.lStructSize = sizeof(CHOOSECOLOR);
			COLOR.hwndOwner = hwnd;
			COLOR.lpCustColors = temp;
			COLOR.Flags = CC_FULLOPEN;
			if (ChooseColor(&COLOR) != 0)
			{
				PenColor = COLOR.rgbResult;
			}
			break;
		case ID_FACECOLOR:
			for (int i = 0; i < 16; i++)
				temp[i] = RGB(rand() % 256, rand() % 256, rand() % 256);
			memset(&COLOR, 0, sizeof(CHOOSECOLOR));
			COLOR.lStructSize = sizeof(CHOOSECOLOR);
			COLOR.hwndOwner = hwnd;
			COLOR.lpCustColors = temp;
			COLOR.Flags = CC_FULLOPEN;
			if (ChooseColor(&COLOR) != 0)
			{
				FaceColor = COLOR.rgbResult;
			}
			break;
		case ID_FONT:
			break;
		case ID_LINE:
			Figure = 1;
			break;
		case ID_ELLIPSE:
			Figure = 2;
			break;
		case ID_RECTANGLE:
			Figure = 3;
			break;
		case ID_WRITE:
			Figure = 4;
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		//hdc = GetDC(hwnd);
		isClick = true;
		mX1 = LOWORD(lParam);
		mY1 = HIWORD(lParam);
		//ReleaseDC(hwnd, hdc);
		break;
	case WM_LBUTTONUP:
		hdc = GetDC(hwnd);
		isClick = false;
		hPen = CreatePen(PS_SOLID, 1, PenColor);
		SelectObject(hdc, hPen);
		hBrush = CreateSolidBrush(FaceColor);
		SelectObject(hdc, hBrush);
		mX2 = LOWORD(lParam);
		mY2 = HIWORD(lParam);
		switch (Figure)
		{
		case 1:
			MoveToEx(hdc, mX1, mY1, NULL);
			LineTo(hdc, mX2, mY2);
			break;
		case 2:
			Ellipse(hdc, mX1, mY1, mX2, mY2);
			break;
		case 3:
			Rectangle(hdc, mX1, mY1, mX2, mY2);
			break;
		case 4:

			orgPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			SelectObject(hdc, orgPen);
			orgBrush = CreateSolidBrush(FaceColor);
			SelectObject(hdc, orgBrush);
			Rectangle(hdc, mX1, mY1, mX2, mY2);
			rect = RECT{ mX1, mY1, mX2, mY2 };
			for (int i = 0; i < 20; i++)
				for (int j = 0; j < 200; j++)
					str[i][j] = '\0';
			//CreateCaret(hwnd, NULL, 5, 15);
			//ShowCaret(hwnd);
			//SetCaretPos(mX1, mY1);
			line = 0;
			count = 0;
			break;
		}
		ReleaseDC(hwnd, hdc);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		HideCaret(hwnd);
		DestroyCaret();
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}