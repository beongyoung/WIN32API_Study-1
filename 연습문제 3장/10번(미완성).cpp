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
	static int headX = 60, headY = 20;
	static int tailX[11], tailY[11];
	static int tailCount = 0;
	static bool isGoing = false;
	static int Direction = 1;

	HPEN blackPen, redPen, bluePen;
	HBRUSH blackBrush, whiteBrush;
	static int foodX[10], foodY[10];

	switch (iMsg)
	{
	case WM_CREATE:
		tailX[0] = 20, tailY[0] = 20;
		for (int i = 0; i < 10; i++)
		{
			foodX[i] = ((rand() % 15) * 40) + 20; //20~560
			foodY[i] = ((rand() % 15) * 40) + 20; //20~560
		}
		break;

	case WM_KEYDOWN:
		if (wParam == VK_RIGHT && (tailX[0] + 60) <= 600) //������Ű
		{
			headX = tailX[0] + 40;
			headY = tailY[0];
			Direction = 1;
		}
		else if (wParam == VK_LEFT && (tailX[0] - 60) >= 0) //����Ű
		{
			headX = tailX[0] - 40;
			headY = tailY[0];
			Direction = 3;
		}
		else if (wParam == VK_UP && (tailY[0] - 60) >= 0) //����Ű
		{
			headX = tailX[0];
			headY = tailY[0] - 40;
			Direction = 4;
		}
		else if (wParam == VK_DOWN && (tailY[0] + 60) <= 600) //�Ʒ�Ű
		{
			headX = tailX[0];
			headY = tailY[0] + 40;
			Direction = 2;
		}
		if (wParam == VK_RETURN)
		{
			if (isGoing) isGoing = false;
			else isGoing = true;

			if (isGoing)
				SetTimer(hwnd, 1, 700, NULL);
			else
				KillTimer(hwnd, 1);
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_TIMER:
		if (Direction == 1 && (headX + 20) < 600) //����
		{
			headX += 40;
			tailX[0] = headX - 40;
		}
		else if (Direction == 3 && (headX - 20) > 0) //��
		{
			headX -= 40;
			tailX[0] = headX + 40;
		}
		else if (Direction == 4 && (headY - 20) > 0) //��
		{
			headY -= 40;
			tailY[0] = headY + 40;
		}
		else if (Direction == 2 && (headY + 20) < 600) //�Ʒ�
		{
			headY += 40;
			tailY[0] = headY - 40;
		}
		for (int i = 0; i < 10; i++) //���� ������
		{
			if (headX == foodX[i] && headY == foodY[i])
			{
				foodX[i] = foodY[i] = -10;
				tailCount++;
				if (Direction == 1)
				{
					tailX[tailCount] = tailX[tailCount - 1] - 40;
					tailY[tailCount] = tailY[tailCount - 1];
				}
				else if (Direction == 2)
				{
					tailX[tailCount] = tailX[tailCount - 1];
					tailY[tailCount] = tailY[tailCount - 1] - 40;
				}
				else if (Direction == 3)
				{
					tailX[tailCount] = tailX[tailCount - 1] + 40;
					tailY[tailCount] = tailY[tailCount - 1];
				}
				else if (Direction == 4)
				{
					tailX[tailCount] = tailX[tailCount - 1];
					tailY[tailCount] = tailY[tailCount - 1] + 40;
				}
			}
		}
		
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		Rectangle(hdc, 0, 0, 600, 600);

		blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
		blackBrush = CreateSolidBrush(BLACK_BRUSH);
		bluePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
		redPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

		SelectObject(hdc, blackPen);
		SelectObject(hdc, blackBrush);
		for (int i = 0; i < 10; i++)
		{
			Ellipse(hdc, foodX[i] - 20, foodY[i] - 20, foodX[i] + 20, foodY[i] + 20); //����
		}

		SelectObject(hdc, redPen);
		SelectObject(hdc, whiteBrush);
		Ellipse(hdc, headX - 20, headY - 20, headX + 20, headY + 20); //�Ӹ�

		SelectObject(hdc, bluePen);
		for (int i = 0; i <= tailCount; i++)
			Ellipse(hdc, tailX[i] - 20, tailY[i] - 20, tailX[i] + 20, tailY[i] + 20); //����

		DeleteObject(blackBrush);
		DeleteObject(whiteBrush);
		DeleteObject(blackPen);
		DeleteObject(redPen);
		DeleteObject(bluePen);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}