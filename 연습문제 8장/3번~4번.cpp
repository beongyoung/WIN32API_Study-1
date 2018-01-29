#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include"resource.h"
#include<winres.h>
//#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
void ReadFromFile(int WndIndex, TCHAR filename[]);
void SaveFromFile(HWND hwnd, TCHAR filename[]);

int SelectWnd;
int WndCount;
RECT rt[100];
HWND hwndChild[100];		//�� �ڽ��������� �ڵ�
static HWND hEdit[100];
TCHAR WinBuff[100][1000];	//�ҷ��� ���ϳ����� �о�鿩 ������ ���۰���
HINSTANCE hInst;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASS WndClass;

	hInst = hInstance;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = FrameWndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.lpszClassName = _T("Window Class Name");

	RegisterClass(&WndClass);

	WndClass.lpfnWndProc = ChildWndProc;
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = _T("Child Window Class Name");

	RegisterClass(&WndClass);

	hwnd = CreateWindow(					//������ ������ ����
		_T("Window Class Name"),
		_T("����ȣ"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
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
#define IDC_EDIT1 100
LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndClient;
	CLIENTCREATESTRUCT clientcreate;
	MDICREATESTRUCT mdicreate;
	static TCHAR filepath[1000], folder[100], filename[100];
	TCHAR filter[] = _T("�ؽ�Ʈ ���� (*.txt)\0*.txt\0��� ���� (*.*)\0*.*\0");
	OPENFILENAME OFN,SFN;

	switch (iMsg)
	{
	case WM_CREATE:
		clientcreate.hWindowMenu = GetSubMenu(GetMenu(hwnd), 0);
		clientcreate.idFirstChild = 100;
		hwndClient = CreateWindow(			//Ŭ���̾�Ʈ ������ ����
			_T("MDICLIENT"), NULL, WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
			0, 0, 0, 0,
			hwnd, NULL, hInst, (LPSTR)&clientcreate);
		ShowWindow(hwndClient, SW_SHOW);	//������ �����쿡 Ŭ���̾�Ʈ �����찡 ��Ÿ��
		return 0;
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILEOPEN:
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner = hwnd;
			OFN.lpstrFile = filepath;		//����ڰ� ������ ���ϰ��
			OFN.nMaxFile = 1000;			//���ϰ���� �ִ�ũ��

			OFN.lpstrFileTitle = filename;	//����ڰ� ������ ��������
			OFN.nMaxFileTitle = 100;		//���������� �ִ�ũ��

			OFN.lpstrFilter = filter;
			OFN.Flags = OFN_EXPLORER;		//������ �ϳ��� ���ð���
			OFN.lpstrInitialDir = _T(".");
			GetOpenFileName(&OFN);			//OFN����ü�� ������ ������� [����]��ȭ���ڸ� �ʱ�ȭ�ϰ�,
											//����ڰ� ������ ������ ������ ����ǰ� ���ϼ��ý� 1��ȯ
			WndCount++;
			ReadFromFile(WndCount, filepath);//���Ϸκ��� �ؽ�Ʈ�� �о���δ�
			mdicreate.szClass = _T("Child Window Class Name");
			mdicreate.szTitle = filename;
			mdicreate.hOwner = hInst;
			mdicreate.x = CW_USEDEFAULT;
			mdicreate.y = CW_USEDEFAULT;
			mdicreate.cx = CW_USEDEFAULT;
			mdicreate.cy = CW_USEDEFAULT;
			mdicreate.style = 0;
			mdicreate.lParam = 0;
			hwndChild[WndCount] = (HWND)SendMessage(hwndClient, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT)&mdicreate);
			//Ŭ���̾�Ʈ �����쿡�� mdicreate�� ������ ������ �ڽ� �����츦 ������ �޽���
			hEdit[WndCount] = CreateWindow(
				_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |	//����Ʈ ��Ʈ�� ����
				ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE, 0, 100,
				rt[WndCount].right, rt[WndCount].bottom, hwndChild[WndCount], (HMENU)IDC_EDIT1, hInst, NULL);
			GetClientRect(hwndChild[WndCount], &rt[WndCount]);
			MoveWindow(hEdit[SelectWnd], 0, 0, rt[SelectWnd].right, rt[SelectWnd].bottom, TRUE);
			return 0;
		case ID_FILESAVE:
			memset(&SFN, 0, sizeof(OPENFILENAME));
			SFN.lStructSize = sizeof(OPENFILENAME);
			SFN.hwndOwner = hwnd;
			SFN.lpstrFilter = filter;
			SFN.lpstrFile = filepath;
			SFN.nMaxFile = 1000;
			OFN.lpstrFileTitle = filename;
			OFN.nMaxFileTitle = 100;		
			SFN.lpstrInitialDir = _T(".");
			GetSaveFileName(&SFN);
			SaveFromFile(hEdit[SelectWnd], filepath);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefFrameProc(hwnd, hwndClient, iMsg, wParam, lParam);
}
LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	for (int i = 1; i <= WndCount; i++)
	{
		if (hwnd == hwndChild[i])
		{
			SelectWnd = i;
			break;
		}
	}

	switch (iMsg)
	{
	case WM_CREATE:
		break;
	case WM_PAINT:
		SetWindowText(hEdit[SelectWnd], WinBuff[SelectWnd]);
		break;
	case WM_SIZE:
		GetClientRect(hwnd, &rt[SelectWnd]);
		MoveWindow(hEdit[SelectWnd], 0, 0, rt[SelectWnd].right, rt[SelectWnd].bottom, TRUE);
		break;
	case WM_DESTROY:
		break;
	}
	return DefMDIChildProc(hwnd, iMsg, wParam, lParam);
}
void ReadFromFile(int WndIndex, TCHAR filename[])
{
	HANDLE hFile;
	DWORD size = 1000;
	hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
	ReadFile(hFile, WinBuff[WndIndex], size * sizeof(TCHAR), &size, NULL);
	WinBuff[WndIndex][size] = NULL;
	CloseHandle(hFile);
}
void SaveFromFile(HWND hwnd, TCHAR filename[])
{
	HANDLE hFile;
	LPTSTR buffer;
	int size;

	hFile = CreateFile(filename, GENERIC_WRITE, 0, 0,						//1.���� ����(���Ͽ� �ؽ�Ʈ�� �����ؾ��ϱ⶧���� GENERIC_WRITE���
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	size = GetWindowTextLength(hwnd);										//���� ũ�� �������� ����Ʈ��Ʈ�� �ؽ�Ʈ�� ���� ����
	buffer = new TCHAR[size + 1];
	memset(buffer, 0, (size + 1) * sizeof(TCHAR));
	size = GetWindowText(hwnd, (LPTSTR)buffer, size + 1);					//����Ʈ ��Ʈ���� �ؽ�Ʈ->���� �о���̱�
	buffer[size] = NULL;
	WriteFile(hFile, buffer, size * sizeof(TCHAR), (LPDWORD)&size, NULL);	//2.���� ����(����->���� ����)
	free(buffer);
	CloseHandle(hFile);
}