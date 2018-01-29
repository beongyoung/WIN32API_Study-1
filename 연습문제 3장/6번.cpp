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
	HDC hdc;
	PAINTSTRUCT ps;
	HPEN hPen, oldPen;
	static RECT catRect = { 100,100,150,150 };
	static RECT ratRect;
	static bool isClick = false;
	static float m = 0;
	switch (iMsg)
	{
	case WM_CREATE:
		break;

	case WM_KEYDOWN:
		break;
	case WM_LBUTTONDOWN:
		isClick = true;
		ratRect = { LOWORD(lParam),HIWORD(lParam),LOWORD(lParam) + 20,HIWORD(lParam) + 20 };
		InvalidateRgn(hwnd, NULL, TRUE);
		SetTimer(hwnd, 1, 10, NULL);
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hwnd);
		if (isClick)
		{
			ratRect = { LOWORD(lParam),HIWORD(lParam),LOWORD(lParam) + 20,HIWORD(lParam) + 20 };
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
			catRect.right = catRect.left + 50;
			catRect.bottom = catRect.top + 50;

			if (ratRect.left > catRect.left)
				catRect.left += 1;
			else
				catRect.left -= 1;

			if (ratRect.top > catRect.top)
				catRect.top += 1;
			else
				catRect.top -= 1;
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		DrawText(hdc, _T("�����"), _tcslen(_T("�����")), &catRect, DT_CENTER | DT_VCENTER);
		if (isClick)
			DrawText(hdc, _T("��"), _tcslen(_T("��")), &ratRect, DT_CENTER | DT_VCENTER);

		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}