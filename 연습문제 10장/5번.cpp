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
void ThreadProc(void *);
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

#define THREAD_NUM 10
static RECT rt;
HANDLE hEvent;
int r[THREAD_NUM], g[THREAD_NUM], b[THREAD_NUM];
int xPos[THREAD_NUM], yPos[THREAD_NUM];
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static HANDLE hThread[THREAD_NUM];
	static int count = 0;
	static BOOL GO = FALSE;
	static BOOL init = TRUE;
	int thread_num = 0;
	switch (iMsg)
	{
	case WM_CREATE:
		GetClientRect(hwnd, &rt);
		hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
		break;

	case WM_LBUTTONDOWN:
		for (int i = 0; i < THREAD_NUM; i++)
		{
			xPos[i] = rand() % 41 + 5;	//5~45
			yPos[i] = rand() % 26 + 5;	//5~30
			xPos[i] = xPos[i] * 20;
			yPos[i] = yPos[i] * 20;
			r[i] = rand() % 256;
			g[i] = rand() % 256;
			b[i] = rand() % 256;
			hThread[i] = (HANDLE)_beginthreadex(NULL, 0, (unsigned int(__stdcall *)(void *))ThreadProc, (void*)&i, 0, NULL);
			Sleep(500);
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (int i = 0; i < THREAD_NUM; i++)
		{
			SelectObject(hdc, CreateSolidBrush(RGB(r[i], g[i], b[i])));
			Ellipse(hdc, xPos[i] - 10, yPos[i] - 10, xPos[i] + 10, yPos[i] + 10);
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		CloseHandle(hThread);
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
void ThreadProc(void *arg)
{
	HDC hdc;
	int index = *((int *)arg);
	hdc = GetDC(hwnd);

	srand(time(NULL));
	while (1)
	{
		int num = 1000 * (rand() % 6 + 4);
		Sleep(num);			//4~9초
		//printf("sleep:%d\n", num);
		xPos[index] = yPos[index] = -30;
		InvalidateRgn(hwnd, NULL, TRUE);

		int num2 = 1000 * (rand() % 2 + 1);
		Sleep(num);			//1~2초
		//printf("sleep:%d\n", num2);

		srand(time(NULL));
		xPos[index] = rand() % 41 + 5;	//5~45
		yPos[index] = rand() % 26 + 5;	//5~30

		xPos[index] = xPos[index] * 20;
		yPos[index] = yPos[index] * 20;
		printf("(%d,%d)\n", xPos[index],yPos[index]);
		InvalidateRgn(hwnd, NULL, TRUE);
		
	}
	ReleaseDC(hwnd, hdc);
	return;
}