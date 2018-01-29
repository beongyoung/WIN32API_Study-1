#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<math.h>
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

	RegisterClass(&WndClass); //������ Ŭ������ Ŀ�ο� ���

	hwnd = CreateWindow( //������ �ڵ鰪 ��ȯ
		_T("Window Class Name"), //������ Ŭ���� �̸�
		_T("ȫ�浿�� ù ��° ������"), //������ Ÿ��Ʋ�̸�
		WS_OVERLAPPEDWINDOW, //������ ��Ÿ��
		100, //������ ��ġ x��ǥ
		100,//������ ��ġ y��ǥ
		1000, //������ ����ũ��
		700, //������ ����ũ��
		NULL, //�θ� �������ڵ�
		NULL, //�޴��ڵ�
		hInstance, //�������α׷� �ν��Ͻ�
		NULL); //���� ������ ����

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
	HDC hdc,memdc;
	PAINTSTRUCT ps;
	HPEN hPen, oldPen;
	static int catX = 100, catY = 100,ratX,ratY;
	static RECT ratRect;
	static bool isClick = false;
	static float m = 0;
	static HBITMAP RatBit,CatBit,oldBit;

	switch (iMsg)
	{
	case WM_CREATE:
		RatBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP7));
		CatBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP6));
		break;

	case WM_KEYDOWN:
		break;
	case WM_LBUTTONDOWN:
		isClick = true;
		ratX = LOWORD(lParam);
		ratY = HIWORD(lParam);
		InvalidateRgn(hwnd, NULL, TRUE);
		SetTimer(hwnd, 1, 10, NULL);
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hwnd);
		if (isClick)
		{
			ratX = LOWORD(lParam);
			ratY = HIWORD(lParam);
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		ReleaseDC(hwnd, hdc);
		break;
	case WM_LBUTTONUP:
		isClick = false;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_TIMER:
		if (isClick)
		{
			if (ratX> catX)
				catX += 1;
			else
				catX -= 1;

			if (ratY> catY)
				catY += 1;
			else
				catY -= 1;
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		oldBit=(HBITMAP)SelectObject(memdc, RatBit);
		if (isClick)
			BitBlt(hdc, ratX, ratY, 68, 49, memdc, 0, 0, SRCCOPY);
		SelectObject(memdc, CatBit);
		BitBlt(hdc, catX, catY, 67, 47, memdc, 0, 0, SRCCOPY);
		SelectObject(memdc, oldBit);
		DeleteDC(memdc);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		DeleteObject(CatBit);
		DeleteObject(RatBit);
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}