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
BOOL directionX[THREAD_NUM] = { TRUE, };
BOOL directionY[THREAD_NUM] = { TRUE, };
int xPos[THREAD_NUM], yPos[THREAD_NUM];
static int r[THREAD_NUM], g[THREAD_NUM], b[THREAD_NUM];
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
		for (int i = 0; i < THREAD_NUM; i++)
		{
			xPos[i] = rand() % (rt.right - 200) + 100;
			yPos[i] = rand() % (rt.bottom - 200) + 100;
			r[i] = rand() % 256;
			g[i] = rand() % 256;
			b[i] = rand() % 256;
		}
		break;

	case WM_LBUTTONDOWN:
		for (int i = 0; i < THREAD_NUM; i++)
		{
			hThread[i] = (HANDLE)_beginthreadex(NULL, 0, (unsigned int(__stdcall *)(void *))ThreadProc, (void*)&i, 0, NULL);
			Sleep(50);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for (int i = 0; i < THREAD_NUM; i++)
		{
			//SelectObject(hdc, CreateSolidBrush(RGB(r[i], g[i], b[i])));
			Ellipse(hdc, xPos[i] - 20, yPos[i] - 20, xPos[i] + 20, yPos[i] + 20);
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
	while (1)
	{
		//WaitForSingleObject(hEvent, INFINITE);

		if (xPos[index] < 20 || xPos[index]>rt.right - 20)
			directionX[index] = directionX[index] ? FALSE : TRUE;
		if (yPos[index] < 20 || yPos[index]>rt.bottom - 20)
			directionY[index] = directionY[index] ? FALSE : TRUE;

		if (directionX[index])
			xPos[index] ++;
		else
			xPos[index] --;

		if (directionY[index])
			yPos[index] ++;
		else
			yPos[index] --;

		for (int i = 0; i < THREAD_NUM; i++)
		{
			if (sqrt(pow(xPos[index] - xPos[i], 2) + pow(yPos[index] - yPos[i], 2)) < 40 && i != index)	//충돌
			{
				xPos[index] = yPos[index] = -30;
				xPos[i] = yPos[i] = -30;
				InvalidateRgn(hwnd, NULL, TRUE);
				return;
			}
			//printf("%lf\n", sqrt(pow(xPos[index] - xPos[i], 2) + pow(yPos[index] - yPos[i], 2)));
		}
		Sleep(10);
		InvalidateRgn(hwnd, NULL, TRUE);
	}
	ReleaseDC(hwnd, hdc);
	return;
}