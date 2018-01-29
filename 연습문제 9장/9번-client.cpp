#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<stdio.h>
#include<math.h>
#include"resource.h"
#include<iostream>
#define WM_ASYNC WM_USER+2

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgFunc2(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
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
	HWND hDlg2;

	switch (iMsg)
	{
	case WM_CREATE:
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_CLIENTDIALOG:
			hDlg2 = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hwnd, DlgFunc2);
			ShowWindow(hDlg2, SW_SHOW);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

BOOL CALLBACK DlgFunc2(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)		//클라 대화상자
{
	static TCHAR str[200], msg[200];

	static HWND hList;
	static WSADATA wsadata1;
	static SOCKET s1;
	static SOCKADDR_IN addr1 = { 0 };

	char buffer[200];
	int  size, msgLen;

	switch (iMsg)
	{
	case WM_INITDIALOG:
		hList = GetDlgItem(hDlg, IDC_LIST2);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_SERVERCONNECT:
			WSAStartup(MAKEWORD(2, 2), &wsadata1);					//1.윈속 사용시작하기
			s1 = socket(AF_INET, SOCK_STREAM, 0);					//2.소켓 생성하기
			addr1.sin_family = AF_INET;
			addr1.sin_port = 20;
			addr1.sin_addr.s_addr = inet_addr("127.0.0.1");
			WSAAsyncSelect(s1, hDlg, WM_ASYNC, FD_READ);				//서버에서 보낸 데이터를 받아야 하므로 FD_READ 이벤트에 대해 WM_ASYNC윈도우 메시지를 등록
			
			if (connect(s1, (LPSOCKADDR)&addr1, sizeof(addr1)) == -1)	//3.연결 요구하기(접속실패시 종료)
			{
			MessageBox(hDlg, _T("접속실패"), _T("접속실패"), NULL);
			return 0;
			}
			
			break;
		case IDOK2:
			GetDlgItemText(hDlg, IDC_EDIT2, str, 200);
			SetDlgItemText(hDlg, IDC_EDIT2, _T(""));
			if (_tcscmp(str, _T("")) == 0)
				break;
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)str);

			break;
		case IDCANCEL2:
			closesocket(s1);
			WSACleanup();
			DestroyWindow(hDlg);
			break;
		}
	case WM_ASYNC:										//WM_ASYNC메시지 발생시 lParam으로 어떤 이벤트가 발생했는지 구분
		switch (lParam)
		{
		case FD_READ:									//서버가 메시지를 보내오면
			msgLen = recv(s1, buffer, 200, 0);
			buffer[msgLen] = NULL;
#ifdef _UNICODE					//프로젝트의 문자집합이 유니코드로 설정되어있다면
			msgLen = MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), NULL, NULL);
			MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), msg, msgLen);
			msg[msgLen] = NULL;
#else
			strcpy_s(msg, buffer);	//msg는 TCHAR형으로 선언되었기 때문에 문자 집합에 따라 WCHAR나 char로 자동변환된다
#endif // _UNICODE
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)msg);
			break;
		default:
			break;
		}
	case WM_DESTROY:
		closesocket(s1);
		WSACleanup();
		DestroyWindow(hDlg);
		break;
	}

	return 0;
}