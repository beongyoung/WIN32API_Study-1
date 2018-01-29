#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<math.h>
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

	RegisterClass(&WndClass); //������ Ŭ������ Ŀ�ο� ���

	hwnd = CreateWindow( //������ �ڵ鰪 ��ȯ
		_T("Window Class Name"), //������ Ŭ���� �̸�
		_T("ȫ�浿�� ù ��° ������"), //������ Ÿ��Ʋ�̸�
		WS_OVERLAPPEDWINDOW, //������ ��Ÿ��
		100, //������ ��ġ x��ǥ
		100,//������ ��ġ y��ǥ
		1000, //������ ����ũ��
		1000, //������ ����ũ��
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
	HDC hdc;
	PAINTSTRUCT ps;
	HPEN hPen, oldPen;
	static int startX = 20, startY = 20;
	static int oldX = 20, oldY = 20, endX, endY;
	static int mx = 0, my = 0;
	static bool Drag = false;
	static int centerX = 20, centerY = 20;

	switch (iMsg)
	{
	case WM_CREATE:
		break;

	case WM_KEYDOWN:
		break;

	case WM_LBUTTONDOWN:
		Drag = true;
		mx = HIWORD(lParam);
		my = LOWORD(lParam);
		//InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_LBUTTONUP:
		Drag = false;
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		oldX = endX = 20;
		oldY = endY = 20;
		SetTimer(hwnd, 1, 10, NULL);
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_TIMER:
		if (centerX == mx&&centerY == my)
			KillTimer(hwnd, 1);
		else
		{
			if (mx > centerX)
				centerX += 1;
			else
				centerX -= 1;
			if (my > centerY)
				centerY += 1;
			else
				centerY -= 1;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hwnd);

		if (Drag)
		{
			SetROP2(hdc, R2_XORPEN);								//XOR�� ������ �������� ����
			SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));		//hdc�� ������� ���
			//SelectObject(hdc, (HBRUSH)GetStockObject(BLACK_BRUSH)); //hdc�� ������ �귯�ø� ���
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);

			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, oldX, oldY);
			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, endX, endY);
			oldX = endX;
			oldY = endY;
			//InvalidateRgn(hwnd, NULL, TRUE); ������ ����
		}
		ReleaseDC(hwnd, hdc);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		Ellipse(hdc, centerX-20, centerY-20, centerX+20, centerY+20);
		MoveToEx(hdc, startX, startY, NULL);
		LineTo(hdc, oldX, oldY);

		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}