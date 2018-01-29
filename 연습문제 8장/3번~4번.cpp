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
HWND hwndChild[100];		//각 자식윈도우의 핸들
static HWND hEdit[100];
TCHAR WinBuff[100][1000];	//불러온 파일내용을 읽어들여 저장할 버퍼공간
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

	hwnd = CreateWindow(					//프레임 윈도우 생성
		_T("Window Class Name"),
		_T("박정호"),
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
	TCHAR filter[] = _T("텍스트 파일 (*.txt)\0*.txt\0모든 파일 (*.*)\0*.*\0");
	OPENFILENAME OFN,SFN;

	switch (iMsg)
	{
	case WM_CREATE:
		clientcreate.hWindowMenu = GetSubMenu(GetMenu(hwnd), 0);
		clientcreate.idFirstChild = 100;
		hwndClient = CreateWindow(			//클라이언트 윈도우 생성
			_T("MDICLIENT"), NULL, WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
			0, 0, 0, 0,
			hwnd, NULL, hInst, (LPSTR)&clientcreate);
		ShowWindow(hwndClient, SW_SHOW);	//프레임 윈도우에 클라이언트 윈도우가 나타남
		return 0;
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILEOPEN:
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner = hwnd;
			OFN.lpstrFile = filepath;		//사용자가 선택한 파일경로
			OFN.nMaxFile = 1000;			//파일경로의 최대크기

			OFN.lpstrFileTitle = filename;	//사용자가 선택한 파일제목
			OFN.nMaxFileTitle = 100;		//파일제목의 최대크기

			OFN.lpstrFilter = filter;
			OFN.Flags = OFN_EXPLORER;		//파일을 하나만 선택가능
			OFN.lpstrInitialDir = _T(".");
			GetOpenFileName(&OFN);			//OFN구조체의 내용을 기반으로 [열기]대화상자를 초기화하고,
											//사용자가 선택한 파일의 정보가 저장되고 파일선택시 1반환
			WndCount++;
			ReadFromFile(WndCount, filepath);//파일로부터 텍스트를 읽어들인다
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
			//클라이언트 윈도우에게 mdicreate의 내용을 가지고 자식 윈도우를 만들라는 메시지
			hEdit[WndCount] = CreateWindow(
				_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |	//에디트 컨트롤 생성
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

	hFile = CreateFile(filename, GENERIC_WRITE, 0, 0,						//1.파일 생성(파일에 텍스트를 저장해야하기때문에 GENERIC_WRITE사용
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	size = GetWindowTextLength(hwnd);										//버퍼 크기 결정위해 에디트컨트롤 텍스트의 길이 얻어옴
	buffer = new TCHAR[size + 1];
	memset(buffer, 0, (size + 1) * sizeof(TCHAR));
	size = GetWindowText(hwnd, (LPTSTR)buffer, size + 1);					//에디트 컨트롤의 텍스트->버퍼 읽어들이기
	buffer[size] = NULL;
	WriteFile(hFile, buffer, size * sizeof(TCHAR), (LPDWORD)&size, NULL);	//2.파일 쓰기(버퍼->파일 저장)
	free(buffer);
	CloseHandle(hFile);
}