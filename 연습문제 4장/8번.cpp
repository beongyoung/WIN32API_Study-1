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
	HPEN hPen,orgPen;
	HBRUSH hBrush,orgBrush;
	CHOOSECOLOR COLOR;
	static COLORREF temp[16], PenColor = RGB(0, 0, 0), FaceColor = RGB(255, 255, 255);

	static bool isClick = false;
	static int mX1, mY1, mX2, mY2, mX3, mY3;
	static int FigureCount = 0;
	struct Object
	{
		int X1, Y1, X2, Y2;
		int Figure = 0;
		bool Selected = false;
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
				for (int i = 0; i < FigureCount; i++)
				{
					if (object[i].Selected)
					{
						object[i].PenColor = COLOR.rgbResult;
						break;
					}
				}
			}
			InvalidateRgn(hwnd, NULL, TRUE);
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
				for (int i = 0; i < FigureCount; i++)
				{
					if (object[i].Selected)
					{
						object[i].FaceColor = COLOR.rgbResult;
						break;
					}
				}
			}
			InvalidateRgn(hwnd, NULL, TRUE);
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
		object[FigureCount].X1 = mX1 = LOWORD(lParam);
		object[FigureCount].Y1 = mY1 = HIWORD(lParam);

		isClick = true;
		break;
	case WM_LBUTTONUP:
		//isClick = false;
		object[FigureCount].X2 = mX2 = LOWORD(lParam);
		object[FigureCount].Y2 = mY2 = HIWORD(lParam);
		InvalidateRgn(hwnd, NULL, TRUE);
		FigureCount++;
		break;
	case WM_RBUTTONDOWN:
		mX3 = LOWORD(lParam);
		mY3 = HIWORD(lParam);
		for (int i = 0; i < FigureCount; i++)
			object[i].Selected = false;
		for (int i = 0; i < FigureCount; i++)
		{
			if (object[i].Figure == 3 && mX3 > object[i].X1&&mX3<object[i].X2&&mY3>object[i].Y1&&mY3 < object[i].Y2)//사각형안을 클릭하면
			{
				object[i].Selected = object[i].Selected?false:true;
				break;
			}
		}
		InvalidateRgn(hwnd, NULL, TRUE);
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
			if (object[i].Selected)
			{
				orgPen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
				SelectObject(hdc, orgPen);
				orgBrush = CreateSolidBrush(RGB(255, 255, 255));
				SelectObject(hdc, orgBrush);
				Rectangle(hdc, object[i].X1 - 10, object[i].Y1 - 10, object[i].X1, object[i].Y1);
				Rectangle(hdc, object[i].X2, object[i].Y1 - 10, object[i].X2 + 10, object[i].Y1);
				Rectangle(hdc, object[i].X1 - 10, object[i].Y2, object[i].X1, object[i].Y2 + 10);
				Rectangle(hdc, object[i].X2, object[i].Y2, object[i].X2 + 10, object[i].Y2 + 10);
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