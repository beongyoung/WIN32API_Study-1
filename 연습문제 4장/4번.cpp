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
	HPEN hPen, oldPen;
	static HMENU hMenu, hSubMenu;
	static int answer;
	static int centerX[10], centerY[10];
	static int circleCount = 0;
	static int mX, mY;
	static bool isClick = false;
	static bool isCopy = false;
	static bool inCircle = false;
	switch (iMsg)
	{
	case WM_CREATE:
		hMenu = GetMenu(hwnd);
		centerX[0] = 20;
		centerY[0] = 20;
		hSubMenu = GetSubMenu(hMenu, 1);
		EnableMenuItem(hSubMenu, ID_EDITCOPY, MF_GRAYED);
		EnableMenuItem(hSubMenu, ID_EDITPASTE, MF_GRAYED);
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_EDITUNDO:		//편집취소
			answer=MessageBox(hwnd, _T("편집을 취소 하시겠습니까?"), _T("편집취소 선택"), MB_OKCANCEL);
			if (answer == IDOK)
			{
				circleCount--;
				InvalidateRgn(hwnd, NULL, TRUE);
			}
			break;
		case ID_EDITCOPY:
			isCopy = true;
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		case ID_EDITPASTE:
			isCopy = false;
			isClick = false;
			if(circleCount<9)
			circleCount++;
			centerX[circleCount] = centerX[circleCount - 1] + 50;
			centerY[circleCount] = centerY[circleCount - 1] + 50;
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		case ID_EXIT:			//끝내기
			answer = MessageBox(hwnd, _T("끝내시겠습니까?"), _T("끝내기 선택"), MB_OKCANCEL);
			if (answer == IDOK)
				PostQuitMessage(0);
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		mX = LOWORD(lParam);
		mY = HIWORD(lParam);
		for (int i = 0; i <= circleCount; i++)
		{
			if (pow(mX - centerX[i], 2) + pow(mY - centerY[i], 2) < 400)
			{
				isClick = true;
				break;
			}
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EnableMenuItem(hSubMenu, ID_EDITCOPY, isClick ? MF_ENABLED : MF_GRAYED);
		EnableMenuItem(hSubMenu, ID_EDITPASTE, isCopy ? MF_ENABLED : MF_GRAYED);
		for (int i = 0; i <= circleCount; i++)
			Ellipse(hdc, centerX[i] - 20, centerY[i] - 20, centerX[i] + 20, centerY[i] + 20);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}