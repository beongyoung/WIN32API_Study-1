#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<math.h>
#include<Richedit.h>
#include"resource.h"
//#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
HINSTANCE hInst;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASS WndClass;
	HINSTANCE hRichedit;
	
	hInst = hInstance;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU3);
	WndClass.lpszClassName = _T("Window Class Name");

	RegisterClass(&WndClass);
	hRichedit = LoadLibrary(_T("RICHED20.DLL"));
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
	FreeLibrary(hRichedit);
	return (int)msg.wParam;
}
#define IDC_RICHEDIT 100
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static HWND hRichedit;
	RECT rect;
	CHOOSEFONT FONT;
	static LOGFONT logfont;
	CHARFORMAT cf;

	switch (iMsg)
	{
	case WM_CREATE:
		GetClientRect(hwnd, &rect);
		hRichedit = CreateWindow(_T("RichEdit20W"), _T("RichEdit Control Font"),
			WS_VISIBLE | WS_CHILD | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_BORDER,
			0, 0, rect.right, rect.bottom, hwnd, (HMENU)IDC_RICHEDIT, hInst, 0);
		break;
	case WM_SIZE:
		GetClientRect(hwnd, &rect);
		MoveWindow(hRichedit, 0, 0, rect.right, rect.bottom, TRUE);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FONT:
			memset(&FONT, 0, sizeof(CHOOSEFONT));
			FONT.lStructSize = sizeof(CHOOSEFONT);
			FONT.hwndOwner = hwnd;
			FONT.iPointSize = 100;
			FONT.Flags = CF_EFFECTS | CF_SCREENFONTS;
			FONT.lpLogFont = &logfont;
			if (ChooseFont(&FONT) != 0)
			{
				cf.cbSize = sizeof(CHARFORMAT);
				cf.dwMask = CFM_FACE | CFM_SIZE | CFM_COLOR;
				cf.yHeight = FONT.iPointSize * 2;
				cf.crTextColor = FONT.rgbColors;
				_tcscpy_s(cf.szFaceName, logfont.lfFaceName);
				SendMessage(hRichedit, EM_SETCHARFORMAT, (WPARAM)(UINT)SCF_SELECTION, (LPARAM)&cf);
			}
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}