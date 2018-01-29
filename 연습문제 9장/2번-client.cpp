//6번소스와 동일
#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"resource.h"
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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.lpszClassName = _T("Window Class Name");

	RegisterClass(&WndClass);

	hwnd = CreateWindow(
		_T("Window Class Name"),
		_T("Client Window"),
		WS_OVERLAPPEDWINDOW,
		50,
		50,
		700,
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
	static SOCKET s;
	static SOCKADDR_IN addr = { 0 };
	static TCHAR msg[200];
	static HWND hButton1;

	
	static int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	static int shape = 0;
	static int recv_x1 = 0, recv_x2 = 0, recv_y1 = 0, recv_y2 = 0;
	static int recv_shape = 0;
	
	static int token2[5];
	static int mx = 0, my = 0;
	static bool Drag = false;
	
	int msgLen;
	char buffer[100];
	char *context;
	char *token;
	int pos_y = 0;
	int i = 0;

	switch (iMsg)
	{
	case WM_CREATE:
		hButton1 = (HWND)CreateWindow(_T("button"), _T("전송"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 300, 0, 50, 20, hwnd, (HMENU)IDC_BUTTON1, hInst, NULL);
		WSAStartup(MAKEWORD(2, 2), &wsadata);					//1.윈속 사용시작하기
		s = socket(AF_INET, SOCK_STREAM, 0);					//2.소켓 생성하기
		addr.sin_family = AF_INET;
		addr.sin_port = 20;
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		WSAAsyncSelect(s, hwnd, WM_ASYNC, FD_READ);				//서버에서 보낸 데이터를 받아야 하므로 FD_READ 이벤트에 대해 WM_ASYNC윈도우 메시지를 등록
		if (connect(s, (LPSOCKADDR)&addr, sizeof(addr)) == -1)	//3.연결 요구하기(접속실패시 종료)
			return 0;
		break;
	case WM_ASYNC:												//양방향 통신이므로 클라이언트가 다른 메시지를 처리하는 도중 서버에서 데이터를 보내면
		switch (lParam)											//WM_ASYNC와 lParam이 같이 도착한다.
		{
		case FD_READ:
			msgLen = recv(s, buffer, 100, 0);
			buffer[msgLen] = NULL;
#ifdef _UNICODE					//프로젝트의 문자집합이 유니코드로 설정되어있다면
			msgLen = MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), NULL, NULL);
			MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), msg, msgLen);
			msg[msgLen] = NULL;
#else
			strcpy_s(msg, buffer);	//msg는 TCHAR형으로 선언되었기 때문에 문자 집합에 따라 WCHAR나 char로 자동변환된다
#endif // _UNICODE
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		default:
			break;
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON1:
			if (s == INVALID_SOCKET)
			{
				MessageBox(hwnd, _T("소켓s가 INVALID_SOCKET"), _T("알림"), NULL);
				break;
			}
			_stprintf_s(buffer, _T("%d,%d,%d,%d,%d"), shape, x1, y1, x2, y2);
			send(s, buffer, 20, 0);
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
		//InvalidateRgn(hwnd, NULL, TRUE);
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
		if (msg != NULL && !Drag)
		{
			context = NULL;
			token = strtok_s(msg, ",", &context);			//context에는 분리된 후 남은 문자열이 들어간다.
			while (token != NULL)
			{
				token2[i++] = atoi(token);
				//TextOut(hdc, 0, pos_y, token, _tcslen(token));
				token = strtok_s(NULL, ",", &context);
				pos_y += 20;
			}
			recv_shape = token2[0];
			recv_x1 = token2[1];
			recv_y1 = token2[2];
			recv_x2 = token2[3];
			recv_y2 = token2[4];
			InvalidateRgn(hwnd, NULL, FALSE);
		}
		
		switch (recv_shape)
		{
		case 1:
			MoveToEx(hdc, recv_x1, recv_y1, NULL);
			LineTo(hdc, recv_x2, recv_y2);
			break;
		case 2:
			Ellipse(hdc, recv_x1, recv_y1, recv_x2, recv_y2);
			break;
		case 3:
			Rectangle(hdc, recv_x1, recv_y1, recv_x2, recv_y2);
			break;
		}
		switch (shape)
		{
		case 1:
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