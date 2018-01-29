//����ŷ �ܹ�����ſ��� ������ Ŭ���̾�Ʈ�� �޽����� ������ �ڵ带 �߰��Ѵ�.
#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<stdio.h>
#include"resource.h"
#define WM_ASYNC WM_USER+2
//#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
HINSTANCE hInst;

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.lpszClassName = _T("Window Class Name");

	RegisterClass(&WndClass);

	hwnd = CreateWindow(
		_T("Window Class Name"),
		_T("Server Window"),
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

#define IDC_BUTTON1 102
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static WSADATA wsadata;
	static SOCKET s, cs;
	static TCHAR msg[200];
	static HWND hButton1;
	char buffer[100];
	static SOCKADDR_IN addr = { 0 }, c_addr;
	int  size, msgLen;
	static int pos_y = 0;
	static int mx = 0, my = 0;
	static bool Drag = false;
	static int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	static int shape = 0;

	switch (iMsg)
	{
	case WM_CREATE:
		hButton1 = (HWND)CreateWindow(_T("button"), _T("����"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 400, 0, 50, 20, hwnd, (HMENU)IDC_BUTTON1, hInst, NULL);
		WSAStartup(MAKEWORD(2, 2), &wsadata);			//1.���ӻ������ϱ�
		s = socket(AF_INET, SOCK_STREAM, 0);			//2.���� ����-Ŭ���̾�Ʈ�� �����ϱ⸦ ��ٸ��� ����
		addr.sin_family = AF_INET;
		addr.sin_port = 20;
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		bind(s, (LPSOCKADDR)&addr, sizeof(addr));		//3.�ּҿ� ���� �����ϱ�
		WSAAsyncSelect(s, hwnd, WM_ASYNC, FD_ACCEPT);	//4.����s���� ������ ������ �õ��ϴ� �̺�Ʈ�� �߻��ϸ� WM_ASYNC�� hwnd �����쿡 �߻���Ű���� ����
		if (listen(s, 5) == -1)							//5.���� �䱸 ��ٸ���
			return 0;
		break;
	case WM_ASYNC:										//WM_ASYNC�޽��� �߻��� lParam���� � �̺�Ʈ�� �߻��ߴ��� ����
		switch (lParam)
		{
		case FD_ACCEPT:									//Ŭ���̾�Ʈ�� ������ �õ��ϸ�
			size = sizeof(c_addr);						//Ŭ���̾�Ʈ�� ����ϱ� ���� ����cs�� ����� Ŭ���̾�Ʈ���� ����� �����Ų��. 
			cs = accept(s, (LPSOCKADDR)&c_addr, &size);
			WSAAsyncSelect(cs, hwnd, WM_ASYNC, FD_READ);//Ŭ���̾�Ʈ�� �޽����� ���� ������ ������ ��ٸ� �� �����Ƿ� FD_READ�� WM_ASYNC�� ���
			break;
		default:
			break;
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON1:
			if (cs == INVALID_SOCKET)
			{
				MessageBox(hwnd, _T("����cs�� INVALID_SOCKET"), _T("�˸�"), NULL);
				break;
			}
			_stprintf_s(buffer, _T("%d,%d,%d,%d,%d"), shape, x1, y1, x2, y2);
			send(cs,buffer, 20, 0);
			break;
		case ID_LINE:
			shape = 1;
			break;
		case ID_ELLIPSE:
			shape = 2;
			break;
		case ID_RECTANGLE:
			shape = 3;
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		Drag = true;
		x1 = LOWORD(lParam);
		y1 = HIWORD(lParam);
		x2 = LOWORD(lParam);
		y2 = HIWORD(lParam);
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_MOUSEMOVE:
		if (Drag)
		{
			x2 = LOWORD(lParam);
			y2 = HIWORD(lParam);
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		break;
	case WM_LBUTTONUP:
		Drag = false;
		x2 = LOWORD(lParam);
		y2 = HIWORD(lParam);
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
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