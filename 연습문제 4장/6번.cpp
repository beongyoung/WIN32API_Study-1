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
		_T("ȫ�浿�� ù ��° ������"),
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
	HPEN hPen;
	HBRUSH hBrush;
	CHOOSECOLOR COLOR;
	static COLORREF temp[16], PenColor = RGB(0, 0, 0), FaceColor = RGB(255, 255, 255);

	static bool isClick = false;
	static int mX1, mY1, mX2, mY2;
	static int FigureCount = 0;
	struct Object
	{
		int X1, Y1, X2, Y2;
		int Figure = 0;
		COLORREF PenColor = RGB(0, 0, 0), FaceColor = RGB(255, 255, 255);
	};
	static Object object[100];

	switch (iMsg)
	{
	case WM_CREATE:
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

				for (int i = FigureCount; i < 100; i++)
					object[i].PenColor = COLOR.rgbResult;
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
				for (int i = FigureCount; i < 100; i++)
					object[i].FaceColor = COLOR.rgbResult;
			}
			break;
		case ID_LINE:
			for (int i = FigureCount; i < 100; i++)
				object[i].Figure = 1;
			break;
		case ID_ELLIPSE:
			for (int i = FigureCount; i < 100; i++)
				object[i].Figure = 2;
			break;
		case ID_RECTANGLE:
			for (int i = FigureCount; i < 100; i++)
				object[i].Figure = 3;
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		//hdc = GetDC(hwnd);
		isClick = true;
		object[FigureCount].X1 = mX1 = LOWORD(lParam);
		object[FigureCount].Y1 = mY1 = HIWORD(lParam);

		//ReleaseDC(hwnd, hdc);
		break;
	case WM_LBUTTONUP:
		isClick = false;
		object[FigureCount].X2 = mX2 = LOWORD(lParam);
		object[FigureCount].Y2 = mY2 = HIWORD(lParam);
		InvalidateRgn(hwnd, NULL, TRUE);
		FigureCount++;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);


		for (int i = 0; i <= FigureCount; i++)
		{
			hPen = CreatePen(PS_SOLID, 1, object[i].PenColor);
			SelectObject(hdc, hPen);
			hBrush = CreateSolidBrush(object[i].FaceColor);
			SelectObject(hdc, hBrush);
			switch (object[i].Figure)
			{
			case 1:
				MoveToEx(hdc, object[i].X1, object[i].Y1, NULL);
				LineTo(hdc, object[i].X2, object[i].Y2);
				break;
			case 2:
				Ellipse(hdc, object[i].X1, object[i].Y1, object[i].X2, object[i].Y2);
				break;
			case 3:
				Rectangle(hdc, object[i].X1, object[i].Y1, object[i].X2, object[i].Y2);
				break;
			}
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}