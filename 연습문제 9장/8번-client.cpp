//6���ҽ��� ����
#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
HINSTANCE hInst;
#define WM_ASYNC WM_USER+2

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;
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
		_T("Client Window"),
		WS_OVERLAPPEDWINDOW,
		50,
		50,
		900,
		380,
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
	static WSADATA wsadata;
	static SOCKET s;
	static SOCKADDR_IN addr = { 0 };

	static bool MyTurn = FALSE;

	HPEN blackPen;
	HBRUSH hBrush;
	static int red[30] = { 0, }, green[30] = { 255, }, blue[30] = { 0, };

	static int server_x = 20, server_y = 300;
	static int client_x = 820, client_y = 300;

	static int server_boom_x = 20, server_boom_y = 300;
	static int client_boom_x = 820, client_boom_y = 300;

	int msgLen;
	char buffer[100];
	int i = 0;

	switch (iMsg)
	{
	case WM_CREATE:
		WSAStartup(MAKEWORD(2, 2), &wsadata);					//1.���� �������ϱ�
		s = socket(AF_INET, SOCK_STREAM, 0);					//2.���� �����ϱ�
		addr.sin_family = AF_INET;
		addr.sin_port = 20;
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		WSAAsyncSelect(s, hwnd, WM_ASYNC, FD_READ);				//�������� ���� �����͸� �޾ƾ� �ϹǷ� FD_READ �̺�Ʈ�� ���� WM_ASYNC������ �޽����� ���
		if (connect(s, (LPSOCKADDR)&addr, sizeof(addr)) == -1)	//3.���� �䱸�ϱ�(���ӽ��н� ����)
			return 0;
		break;
	case WM_ASYNC:												//����� ����̹Ƿ� Ŭ���̾�Ʈ�� �ٸ� �޽����� ó���ϴ� ���� �������� �����͸� ������
		switch (lParam)											//WM_ASYNC�� lParam�� ���� �����Ѵ�.
		{
		case FD_READ:
			recv(s, buffer, 100, 0);
			if (!_tcscmp(buffer, _T("-1")))				//�߻�޽��� ����
			{
				SetTimer(hwnd, 1, 15, NULL);
				MyTurn = TRUE;
				return 0;
			}
			else if (!_tcscmp(buffer, _T("2")))				//������ �̵� �޽��� ����
			{
				server_x += 10;
				InvalidateRgn(hwnd, NULL, TRUE);
				return 0;
			}
			else if (!_tcscmp(buffer, _T("1")))				//�����̵� �޽��� ����
			{
				server_x -= 10;
				InvalidateRgn(hwnd, NULL, TRUE);
				return 0;
			}
			break;
		default:
			break;
		}
		break;
	case WM_KEYDOWN:
		if (wParam == VK_SPACE&&MyTurn)
		{
			SetTimer(hwnd, 2, 15, NULL);
			_stprintf_s(buffer, _T("-1"));		//�߻� �޽���
			send(s, (LPSTR)buffer, 10, 0);
			MyTurn = FALSE;
		}
		else if (wParam == VK_RIGHT)
		{
			client_x += 10;
			client_boom_x = client_x;
			_stprintf_s(buffer, _T("2"));		//������ �̵� �޽���
			send(s, (LPSTR)buffer, 10, 0);
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		else if (wParam == VK_LEFT)
		{
			client_x -= 10;
			client_boom_x = client_x;
			_stprintf_s(buffer, _T("1"));		//���� �̵� �޽���
			send(s, (LPSTR)buffer, 10, 0);
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			server_boom_x += 10;
			server_boom_y = 0.001f*pow(server_boom_x - 400, 2) + 150;
			InvalidateRgn(hwnd, NULL, TRUE);
			if (server_boom_y >= 400)
			{
				server_boom_x = server_x;
				server_boom_y = server_y;
				KillTimer(hwnd, 1);
			}
			break;
		case 2:
			client_boom_x -= 10;
			client_boom_y = 0.001f*pow(client_boom_x - 400, 2) + 150;
			InvalidateRgn(hwnd, NULL, TRUE);
			if (client_boom_y >= 400)
			{
				client_boom_x = client_x;
				client_boom_y = client_y;
				KillTimer(hwnd, 2);
			}
			break;
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		hBrush = CreateSolidBrush(RGB(0, 0, 0));
		SelectObject(hdc, hBrush);
		Ellipse(hdc, server_x - 20, server_y - 20, server_x + 20, server_y + 20);

		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hdc, hBrush);
		Ellipse(hdc, client_x - 20, client_y - 20, client_x + 20, client_y + 20);

		hBrush = CreateSolidBrush(RGB(0, 0, 255));
		SelectObject(hdc, hBrush);
		Ellipse(hdc, server_boom_x - 5, server_boom_y - 5, server_boom_x + 5, server_boom_y + 5);

		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		SelectObject(hdc, hBrush);
		Ellipse(hdc, client_boom_x - 5, client_boom_y - 5, client_boom_x + 5, client_boom_y + 5);

		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		closesocket(s);
		WSACleanup();
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}