#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<math.h>
#include<iostream>
#include"resource.h"
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
HINSTANCE hInst;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	hInst = hInstance;
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

	RegisterClass(&WndClass);

	hwnd = CreateWindow(
		_T("Window Class Name"),
		_T("홍길동의 첫 번째 윈도우"),
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
void TextPrint(HDC hdc, int x, int y, TCHAR word[])
{
	SetTextColor(hdc, RGB(255, 255, 255));
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			TextOut(hdc, x + i, y + j, word, _tcslen(word));
	SetTextColor(hdc, RGB(0, 0, 0));
	TextOut(hdc, x, y, word, _tcslen(word));
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, mem1dc, mem2dc;
	PAINTSTRUCT ps;
	HPEN hPen, oldPen;
	static HBITMAP hBit1, hBit2, oldBit1, oldBit2;
	static SIZE size;
	static TCHAR str[10]=_T(""),text[5][10];
	static int count = 0;
	static int x[5], y[5];
	static char temp[5];
	switch (iMsg)
	{
	case WM_CREATE:
		hBit2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		_tcscpy_s(text[0], _countof(text[0]), _T("조랑말"));
		_tcscpy_s(text[1], _countof(text[1]), _T("얼룩말"));
		_tcscpy_s(text[2], _countof(text[2]), _T("돼지"));
		_tcscpy_s(text[3], _countof(text[3]), _T("오리"));
		_tcscpy_s(text[4], _countof(text[4]), _T("닭"));
		for (int i = 0; i < 5; i++)
		{
			x[i] = rand() % 800 + 100;
			y[i] = rand() % 200;
		}
		SetTimer(hwnd, 1, 1000, NULL);

		CreateCaret(hwnd, NULL, 5, 15);	//캐럿생성
		ShowCaret(hwnd);				//캐럿보이기
		break;

	case WM_KEYDOWN:
		break;
	case WM_CHAR:
		if (wParam == VK_RETURN)
		{
			for (int i = 0; i < 5; i++)
			{
				if (_tcscmp(str,text[i])==0)
				{
					y[i] = x[i] = 2000;
					count = 0;
					_tcscpy_s(str, _countof(str), _T(""));
					MessageBox(hwnd, _T("번째 텍스트일치"), _T("dd"), MB_OK);
					break;
				}
			}
		}
		else if (wParam == VK_BACK&&count>0)
		{
			count--;
		}
		else if(wParam!=VK_BACK&&wParam!=VK_RETURN)
		{
			str[count++] = wParam;
			//str[count] = NULL;
		}
		str[count] = NULL;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_TIMER:
		for (int i = 0; i < 5; i++)
		{
			y[i] += 10;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_PAINT:
	
		hdc = BeginPaint(hwnd, &ps);
		mem1dc = CreateCompatibleDC(hdc);
		mem2dc = CreateCompatibleDC(hdc);
		if (hBit1 == NULL)
			hBit1 = CreateCompatibleBitmap(hdc, 1024, 768);
		oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);
		oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit2);
		BitBlt(mem1dc, 0, 0, 1024, 768, mem2dc, 0, 0, SRCCOPY);		//배경적재
		SetBkMode(mem1dc, TRANSPARENT);
		for (int i = 0; i < 5; i++)
			TextPrint(mem1dc, x[i], y[i], text[i]);					//텍스트적재
		BitBlt(hdc, 0, 0, 1024, 768, mem1dc, 0, 0, SRCCOPY);

		Rectangle(hdc, 400, 550, 600, 570);
		SetBkMode(hdc, TRANSPARENT);
		GetTextExtentPoint(hdc, str, _tcslen(str), &size);			 //문자열이 차지하는 크기 측정하기
		SetCaretPos(400+size.cx, 550);								 //캐럿위치설정
		TextOut(hdc, 400, 550, str, _tcslen(str));
		SelectObject(mem1dc, oldBit1);
		SelectObject(mem2dc, oldBit2);
		DeleteDC(mem1dc);
		DeleteDC(mem2dc);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		DeleteObject(hBit1);
		DeleteObject(hBit2);
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}