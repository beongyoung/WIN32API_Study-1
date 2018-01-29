#include <Windows.h>
#include<tchar.h>
#include<string.h>
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

	RegisterClass(&WndClass); 

	hwnd = CreateWindow( 
		_T("Window Class Name"), 
		_T("홍길동의 첫 번째 윈도우"), 
		WS_OVERLAPPEDWINDOW,
		200, 
		300,
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
	RECT rect = { 0,0,1000,1000 };
	static TCHAR str[10][100];
	static int count,yPos,lengh[10];
	static SIZE size;


	switch (iMsg)
	{
	case WM_CREATE:
		count = 0;
		yPos = 0;
		CreateCaret(hwnd, NULL, 5, 15);	
		ShowCaret(hwnd);				
		break;
	case WM_CHAR:
		if (yPos != 10)
		{
			if (wParam == VK_RETURN&&count >= 0 && count <= 99)
			{
				str[yPos][count] = '\0';
				yPos++;
				count = 0;
			}
			else if (wParam == VK_BACK)
			{
				if (count > 0)
				{
					count--;
					str[yPos][count] = NULL;
				}
				else if (count == 0)
				{
					yPos--;
					count = _tcsclen(str[yPos]);
					count--;
					str[yPos][count] = NULL;
				}
			}
			else
			{
				str[yPos][count++] = wParam;
				//str[yPos][count] = '\0';
			}
			if (count == 100)
			{
				str[yPos][count] = '\0';
				yPos++;
				count = 0;
			}
		}
									 
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for(int i=0;i<=yPos;i++)
			TextOut(hdc, 0, i*20, str[i], _tcslen(str[i]));
		GetTextExtentPoint(hdc, str[yPos], _tcslen(str[yPos]), &size);
		SetCaretPos(size.cx, yPos * 20);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		DestroyCaret();
		HideCaret(hwnd);
		DestroyCaret();
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}