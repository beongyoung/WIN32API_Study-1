#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<winres.h>
#include<process.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
void ThreadProc();
HINSTANCE hInst;
HWND hwnd;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
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
#define MAX_THREAD_NUM 50
int XPOS, YPOS;
int r, g, b;
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static HANDLE hThread[MAX_THREAD_NUM];
	static int count = 0;
	static RECT rt;

	switch (iMsg)
	{
	case WM_CREATE:
		GetClientRect(hwnd, &rt);
		break;

	case WM_LBUTTONDOWN:
		XPOS = rand() % rt.right;
		YPOS = rand() % rt.bottom;
		r = rand() % 256;
		g = rand() % 256;
		b = rand() % 256;
		hThread[count] = (HANDLE)_beginthreadex(NULL, 0, (unsigned int(__stdcall *)(void *))ThreadProc, NULL, 0, NULL);
		count++;
		break;
	case WM_DESTROY:
		for (int i = 0; i < MAX_THREAD_NUM; i++)
			CloseHandle(hThread[i]);
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
void ThreadProc()
{
	HDC hdc;
	//srand(time(NULL));
	int xPos, yPos;

	hdc = GetDC(hwnd);
	xPos = XPOS;
	yPos = YPOS;
	SelectObject(hdc, CreateSolidBrush(RGB(r, g, b)));
	Ellipse(hdc, xPos - 10, yPos - 10, xPos + 10, yPos + 10);
	for (int i = 0; i < 10; i++)
	{
		Sleep(1000);
		Ellipse(hdc, xPos - 10, yPos - 10, xPos + 10, yPos + 10);
		SelectObject(hdc, CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256)));
		
		


	}
	ReleaseDC(hwnd, hdc);
	return;
}