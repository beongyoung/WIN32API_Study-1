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
		_T("����ȣ"),
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

BOOL CALLBACK DlgFunc1(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)	//������ȭ����
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
			WSAStartup(MAKEWORD(2, 2), &wsadata);			//1.���ӻ������ϱ�
			s = socket(AF_INET, SOCK_STREAM, 0);			//2.���� ����-Ŭ���̾�Ʈ�� �����ϱ⸦ ��ٸ��� ����
			addr.sin_family = AF_INET;
			addr.sin_port = 20;
			addr.sin_addr.s_addr = inet_addr("127.0.0.1");
			bind(s, (LPSOCKADDR)&addr, sizeof(addr));		//3.�ּҿ� ���� �����ϱ�
			WSAAsyncSelect(s, hDlg, WM_ASYNC, FD_ACCEPT);	//4.����s���� ������ ������ �õ��ϴ� �̺�Ʈ�� �߻��ϸ� WM_ASYNC�� hwnd �����쿡 �߻���Ű���� ����
			if (listen(s, 5) == -1)							//5.���� �䱸 ��ٸ���
			{
				MessageBox(hDlg, _T("�������ӺҰ�"), _T("�������ӺҰ�"), NULL);
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

	case WM_ASYNC:										//WM_ASYNC�޽��� �߻��� lParam���� � �̺�Ʈ�� �߻��ߴ��� ����
		switch (lParam)
		{
		case FD_ACCEPT:									//Ŭ���̾�Ʈ�� ������ �õ��ϸ�
			size = sizeof(c_addr);						//Ŭ���̾�Ʈ�� ����ϱ� ���� ����cs�� ����� Ŭ���̾�Ʈ���� ����� �����Ų��. 
			cs = accept(s, (LPSOCKADDR)&c_addr, &size);
			WSAAsyncSelect(cs, hDlg, WM_ASYNC, FD_READ);//Ŭ���̾�Ʈ�� �޽����� ���� ������ ������ ��ٸ� �� �����Ƿ� FD_READ�� WM_ASYNC�� ���
			MessageBox(hDlg, _T("Ŭ���̾�Ʈ����"), _T("Ŭ���̾�Ʈ����"), NULL);
			break;
		case FD_READ:									//Ŭ���̾�Ʈ�� �޽����� ��������
			MessageBox(hDlg, _T("Ŭ���̾�Ʈ�޽�������"), _T("Ŭ���̾�Ʈ�޽�������"), NULL);
			msgLen = recv(cs, buffer, 200, 0);
			buffer[msgLen] = NULL;
#ifdef _UNICODE					//������Ʈ�� ���������� �����ڵ�� �����Ǿ��ִٸ�
			msgLen = MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), NULL, NULL);
			MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), msg, msgLen);
			msg[msgLen] = NULL;
#else
			strcpy_s(msg,buffer);	//msg�� TCHAR������ ����Ǿ��� ������ ���� ���տ� ���� WCHAR�� char�� �ڵ���ȯ�ȴ�
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
