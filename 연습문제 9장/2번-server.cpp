//논블로킹 단방향통신에서 서버가 클라이언트로 메시지를 보내는 코드를 추가한다.
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
	static SOCKET s, cs;
	static TCHAR msg[200];
	static HWND hButton1;


	static SOCKADDR_IN addr = { 0 }, c_addr;
	static int recv_x1 = 0, recv_x2 = 0, recv_y1 = 0, recv_y2 = 0;
	static int recv_shape = 0;
	static int pos_y = 0;
	static int mx = 0, my = 0;

	static bool Drag = false;
	static int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	static int shape = 0;
	static int token2[5];

	int  size, msgLen;
	char buffer[100];
	char *context;
	char *token;
	int i = 0;

	switch (iMsg)
	{
	case WM_CREATE:
		hButton1 = (HWND)CreateWindow(_T("button"), _T("전송"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 300, 0, 50, 20, hwnd, (HMENU)IDC_BUTTON1, hInst, NULL);
		WSAStartup(MAKEWORD(2, 2), &wsadata);			//1.윈속사용시작하기
		s = socket(AF_INET, SOCK_STREAM, 0);			//2.소켓 생성-클라이언트가 접속하기를 기다리는 소켓
		addr.sin_family = AF_INET;
		addr.sin_port = 20;
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		bind(s, (LPSOCKADDR)&addr, sizeof(addr));		//3.주소와 소켓 연결하기
		WSAAsyncSelect(s, hwnd, WM_ASYNC, FD_ACCEPT);	//4.소켓s에서 상대방이 접속을 시도하는 이벤트가 발생하면 WM_ASYNC를 hwnd 윈도우에 발생시키위해 설정
		if (listen(s, 5) == -1)							//5.연결 요구 기다리기
			return 0;
		break;
	case WM_ASYNC:										//WM_ASYNC메시지 발생시 lParam으로 어떤 이벤트가 발생했는지 구분
		switch (lParam)
		{
		case FD_ACCEPT:									//클라이언트가 접속을 시도하면
			size = sizeof(c_addr);						//클라이언트와 통신하기 위한 소켓cs를 만들어 클라이언트와의 통신을 전담시킨다. 
			cs = accept(s, (LPSOCKADDR)&c_addr, &size);
			WSAAsyncSelect(cs, hwnd, WM_ASYNC, FD_READ);//클라이언트가 메시지를 보낼 때까지 무한정 기다릴 수 없으므로 FD_READ를 WM_ASYNC에 등록
			break;
		case FD_READ:
			msgLen = recv(cs, buffer, 100, 0);
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
			if (cs == INVALID_SOCKET)
			{
				MessageBox(hwnd, _T("소켓cs가 INVALID_SOCKET"), _T("알림"), NULL);
				break;
			}
			_stprintf_s(buffer, _T("%d,%d,%d,%d,%d"), shape, x1, y1, x2, y2);
			send(cs, buffer, 20, 0);
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
				token = strtok_s(NULL, ",", &context);
				pos_y += 20;
			}
			recv_shape = token2[0];
			recv_x1 = token2[1];
			recv_y1 = token2[2];
			recv_x2 = token2[3];
			recv_y2 = token2[4];
			InvalidateRgn(hwnd, NULL,FALSE);
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