#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<stdio.h>
#include<math.h>
#include"resource.h"
#include<iostream>
#define WM_ASYNC WM_USER+2

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgFunc1(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU2);
	WndClass.lpszClassName = _T("Window Class Name");

	RegisterClass(&WndClass);

	hwnd = CreateWindow(
		_T("Window Class Name"),
		_T("박정호"),
		WS_OVERLAPPEDWINDOW,
		50,
		50,
		1000,
		500,
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
	HWND hDlg1;

	switch (iMsg)
	{
	case WM_CREATE:
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_SERVERDIALOG:
			hDlg1 = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, DlgFunc1);
			ShowWindow(hDlg1, SW_SHOW);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

BOOL CALLBACK DlgFunc1(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)	//서버대화상자
{
	static TCHAR str[200],msg[200];

	static HWND hList;
	static WSADATA wsadata;
	static SOCKET s, cs;
	static SOCKADDR_IN addr = { 0 }, c_addr;

	char buffer[200];
	int  size, msgLen;

	switch (iMsg)
	{
	case WM_INITDIALOG:
		hList = GetDlgItem(hDlg, IDC_LIST1);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{	
		case IDC_SERVERSTART:
			WSAStartup(MAKEWORD(2, 2), &wsadata);			//1.윈속사용시작하기
			s = socket(AF_INET, SOCK_STREAM, 0);			//2.소켓 생성-클라이언트가 접속하기를 기다리는 소켓
			addr.sin_family = AF_INET;
			addr.sin_port = 20;
			addr.sin_addr.s_addr = inet_addr("127.0.0.1");
			bind(s, (LPSOCKADDR)&addr, sizeof(addr));		//3.주소와 소켓 연결하기
			WSAAsyncSelect(s, hDlg, WM_ASYNC, FD_ACCEPT);	//4.소켓s에서 상대방이 접속을 시도하는 이벤트가 발생하면 WM_ASYNC를 hwnd 윈도우에 발생시키위해 설정
			if (listen(s, 5) == -1)							//5.연결 요구 기다리기
			{
				MessageBox(hDlg, _T("서버접속불가"), _T("서버접속불가"), NULL);
				return 0;

			}
			break;

		case IDOK:
			GetDlgItemText(hDlg, IDC_EDIT1, str, 200);
			SetDlgItemText(hDlg, IDC_EDIT1, _T(""));
			if (_tcscmp(str, _T("")) == 0)
				break;
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)str);
			break;
		case IDCANCEL:
			closesocket(s);
			WSACleanup();
			DestroyWindow(hDlg);
			break;
		}

	case WM_ASYNC:										//WM_ASYNC메시지 발생시 lParam으로 어떤 이벤트가 발생했는지 구분
		switch (lParam)
		{
		case FD_ACCEPT:									//클라이언트가 접속을 시도하면
			size = sizeof(c_addr);						//클라이언트와 통신하기 위한 소켓cs를 만들어 클라이언트와의 통신을 전담시킨다. 
			cs = accept(s, (LPSOCKADDR)&c_addr, &size);
			WSAAsyncSelect(cs, hDlg, WM_ASYNC, FD_READ);//클라이언트가 메시지를 보낼 때까지 무한정 기다릴 수 없으므로 FD_READ를 WM_ASYNC에 등록
			MessageBox(hDlg, _T("클라이언트접속"), _T("클라이언트접속"), NULL);
			break;
		case FD_READ:									//클라이언트가 메시지를 보내오면
			MessageBox(hDlg, _T("클라이언트메시지도착"), _T("클라이언트메시지도착"), NULL);
			msgLen = recv(cs, buffer, 200, 0);
			buffer[msgLen] = NULL;
#ifdef _UNICODE					//프로젝트의 문자집합이 유니코드로 설정되어있다면
			msgLen = MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), NULL, NULL);
			MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), msg, msgLen);
			msg[msgLen] = NULL;
#else
			strcpy_s(msg,buffer);	//msg는 TCHAR형으로 선언되었기 때문에 문자 집합에 따라 WCHAR나 char로 자동변환된다
#endif // _UNICODE
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)msg);
			break;
		default:
			break;
		}
		break;
	case WM_DESTROY:
		closesocket(s);
		WSACleanup();
		DestroyWindow(hDlg);
		break;
	}

	return 0;
}
