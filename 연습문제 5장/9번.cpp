#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<math.h>
#include<stdio.h>
#include"resource.h"
//#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, mem1dc;
	PAINTSTRUCT ps;
	HPEN hPen, oldPen;
	static HBITMAP hBit, oldBit;
	static int BY = 1;
	static int stdX = 0, stdY = 0;
	static int Image[5][5];
	static int tempX, tempY;
	static bool DUP = false, GAMESTART = false, DEVIDE = false;
	struct POS
	{
		int ImageX, ImageY;
	};
	static POS pos[5][5];
	int temp;
	switch (iMsg)
	{
	case WM_CREATE:
		hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP5));
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_RIGHT:
			if (stdX < BY - 1)
			{
				temp = pos[stdX][stdY].ImageX;
				pos[stdX][stdY].ImageX = pos[stdX + 1][stdY].ImageX;
				pos[stdX + 1][stdY].ImageX = temp;

				temp = pos[stdX][stdY].ImageY;
				pos[stdX][stdY].ImageY = pos[stdX + 1][stdY].ImageY;
				pos[stdX + 1][stdY].ImageY = temp;
				stdX++;
			}
			break;
		case VK_LEFT:
			if (stdX > 0)
			{
				temp = pos[stdX][stdY].ImageX;
				pos[stdX][stdY].ImageX = pos[stdX - 1][stdY].ImageX;
				pos[stdX - 1][stdY].ImageX = temp;

				temp = pos[stdX][stdY].ImageY;
				pos[stdX][stdY].ImageY = pos[stdX - 1][stdY].ImageY;
				pos[stdX - 1][stdY].ImageY = temp;
				stdX--;
			}
			break;
		case VK_UP:
			if (stdY > 0)
			{
				temp = pos[stdX][stdY].ImageX;
				pos[stdX][stdY].ImageX = pos[stdX][stdY - 1].ImageX;
				pos[stdX][stdY - 1].ImageX = temp;

				temp = pos[stdX][stdY].ImageY;
				pos[stdX][stdY].ImageY = pos[stdX][stdY - 1].ImageY;
				pos[stdX][stdY - 1].ImageY = temp;
				stdY--;
			}
			break;
		case VK_DOWN:
			if (stdY < BY - 1)
			{
				temp = pos[stdX][stdY].ImageX;
				pos[stdX][stdY].ImageX = pos[stdX][stdY + 1].ImageX;
				pos[stdX][stdY + 1].ImageX = temp;

				temp = pos[stdX][stdY].ImageY;
				pos[stdX][stdY].ImageY = pos[stdX][stdY + 1].ImageY;
				pos[stdX][stdY + 1].ImageY = temp;
				stdY++;
			}
			break;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_3BY3:
			BY = 3;
			break;
		case ID_4BY4:
			BY = 4;
			break;
		case ID_5BY5:
			BY = 5;
			break;
		case ID_GAMESTART:
			GAMESTART = true;
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		case ID_OVERVIEW:
			BY = 1;
			GAMESTART = false;
			DEVIDE = false;
			break;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		mem1dc = CreateCompatibleDC(hdc);
		oldBit = (HBITMAP)SelectObject(mem1dc, hBit);
		if (!GAMESTART)
		{
			switch (BY)
			{
			case 3:
				pos[0][0].ImageX = 0; pos[0][1].ImageX = 2; pos[0][2].ImageX = 1;
				pos[0][0].ImageY = 1; pos[0][1].ImageY = 0; pos[0][2].ImageY = 1;

				pos[1][0].ImageX = 1; pos[1][1].ImageX = 0; pos[1][2].ImageX = 2;
				pos[1][0].ImageY = 0; pos[1][1].ImageY = 2; pos[1][2].ImageY = 1;

				pos[2][0].ImageX = 2; pos[2][1].ImageX = 1; pos[2][2].ImageX = 0;
				pos[2][0].ImageY = 2; pos[2][1].ImageY = 2; pos[2][2].ImageY = 0;
				break;
			case 4:
				pos[0][0].ImageX = 2; pos[0][1].ImageX = 0; pos[0][2].ImageX = 2; pos[0][3].ImageX = 1;
				pos[0][0].ImageY = 1; pos[0][1].ImageY = 0; pos[0][2].ImageY = 3; pos[0][3].ImageY = 0;

				pos[1][0].ImageX = 3; pos[1][1].ImageX = 1; pos[1][2].ImageX = 3; pos[1][3].ImageX = 2;
				pos[1][0].ImageY = 0; pos[1][1].ImageY = 1; pos[1][2].ImageY = 2; pos[1][3].ImageY = 2;

				pos[2][0].ImageX = 0; pos[2][1].ImageX = 2; pos[2][2].ImageX = 1; pos[2][3].ImageX = 0;
				pos[2][0].ImageY = 1; pos[2][1].ImageY = 0; pos[2][2].ImageY = 2; pos[2][3].ImageY = 3;

				pos[3][0].ImageX = 3; pos[3][1].ImageX = 3; pos[3][2].ImageX = 0; pos[3][3].ImageX = 1;
				pos[3][0].ImageY = 3; pos[3][1].ImageY = 1; pos[3][2].ImageY = 2; pos[3][3].ImageY = 3;
				break;
			case 5:
				pos[0][0].ImageX = 3; pos[0][1].ImageX = 4; pos[0][2].ImageX = 3; pos[0][3].ImageX = 1; pos[0][4].ImageX = 2;
				pos[0][0].ImageY = 0; pos[0][1].ImageY = 4; pos[0][2].ImageY = 2; pos[0][3].ImageY = 1; pos[0][4].ImageY = 4;

				pos[1][0].ImageX = 1; pos[1][1].ImageX = 0; pos[1][2].ImageX = 2; pos[1][3].ImageX = 4; pos[1][4].ImageX = 0;
				pos[1][0].ImageY = 2; pos[1][1].ImageY = 1; pos[1][2].ImageY = 1; pos[1][3].ImageY = 0; pos[1][4].ImageY = 2;

				pos[2][0].ImageX = 2; pos[2][1].ImageX = 1; pos[2][2].ImageX = 3; pos[2][3].ImageX = 4; pos[2][4].ImageX = 3;
				pos[2][0].ImageY = 0; pos[2][1].ImageY = 0; pos[2][2].ImageY = 4; pos[2][3].ImageY = 3; pos[2][4].ImageY = 3;

				pos[3][0].ImageX = 3; pos[3][1].ImageX = 1; pos[3][2].ImageX = 0; pos[3][3].ImageX = 1; pos[3][4].ImageX = 0;
				pos[3][0].ImageY = 1; pos[3][1].ImageY = 4; pos[3][2].ImageY = 0; pos[3][3].ImageY = 3; pos[3][4].ImageY = 3;

				pos[4][0].ImageX = 0; pos[4][1].ImageX = 2; pos[4][2].ImageX = 4; pos[4][3].ImageX = 2; pos[4][4].ImageX = 4;
				pos[4][0].ImageY = 4; pos[4][1].ImageY = 2; pos[4][2].ImageY = 2; pos[4][3].ImageY = 3; pos[4][4].ImageY = 1;
				break;
			}
		}
		if (!DEVIDE)
		{
			BitBlt(hdc, 0, 0, 819, 614, mem1dc, 0, 0, SRCCOPY);
			DEVIDE = true;
		}
		else
		{
			for (int i = 0; i < BY; i++)
			{
				for (int j = 0; j < BY; j++)
				{
					if (GAMESTART&&i == stdX && j == stdY) continue;
					BitBlt(hdc, i * (819.0f / BY), j* (614.0f / BY), 819.0f / BY, 614.0f / BY, mem1dc, (819.0f / BY)*pos[i][j].ImageX, (614.0f / BY)*pos[i][j].ImageY, SRCCOPY);
				}
			}
		}

		SelectObject(mem1dc, hBit);
		DeleteDC(mem1dc);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		DeleteObject(hBit);
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}