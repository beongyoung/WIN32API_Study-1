//6���ҽ��� ����
#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
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
		500,
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
	HDC hdc;
	PAINTSTRUCT ps;
	static WSADATA wsadata;
	static SOCKET s;
	static SOCKADDR_IN addr = { 0 };
	static TCHAR msg[200];
	int msgLen;
	char buffer[100];
	static int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	static int shape = 0;
	char *context;
	char *token;
	static int token2[5];
	int pos_y = 0;
	int i = 0;
	int length = 0;
	TCHAR tempstr[50];
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
			msgLen = recv(s, buffer, 100, 0);
			buffer[msgLen] = NULL;
#ifdef _UNICODE					//������Ʈ�� ���������� �����ڵ�� �����Ǿ��ִٸ�
			msgLen = MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), NULL, NULL);
			MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), msg, msgLen);
			msg[msgLen] = NULL;
#else
			strcpy_s(msg, buffer);	//msg�� TCHAR������ ����Ǿ��� ������ ���� ���տ� ���� WCHAR�� char�� �ڵ���ȯ�ȴ�
#endif // _UNICODE
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		default:
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if (msg != NULL)
		{
			context = NULL;
			token = strtok_s(msg, ",", &context);			//context���� �и��� �� ���� ���ڿ��� ����.
			while (token != NULL)
			{
				token2[i++] = atoi(token);
				//TextOut(hdc, 0, pos_y, token, _tcslen(token));
				token = strtok_s(NULL, ",", &context);
				pos_y += 20;
			}
			shape = token2[0];
			x1 = token2[1];
			y1 = token2[2];
			x2 = token2[3];
			y2 = token2[4];
			//InvalidateRgn(hwnd, NULL,TRUE);
			switch (shape)
			{
			case 1:
				SetROP2(hdc, R2_XORPEN);								//XOR�� ������ �������� ����
				SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));		//hdc�� ������� ���
																		//SelectObject(hdc, (HBRUSH)GetStockObject(BLACK_BRUSH)); //hdc�� ������ �귯�ø� ���
				MoveToEx(hdc, x1, y1, NULL);
				LineTo(hdc, x2, y2);
				break;
			case 2:
				Ellipse(hdc, x1, y1, x2, y2);
				break;
			case 3:
				Rectangle(hdc, x1, y1, x2, y2);
				break;
			}
		}
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