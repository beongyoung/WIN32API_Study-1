#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include"resource.h"
#include<winres.h>
//#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
HINSTANCE hInst;
void FileWrite(HWND hwnd, LPTSTR filename)
{
	HANDLE hFile;
	LPTSTR buffer;
	int size;

	hFile = CreateFile(filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	size = GetWindowTextLength(hwnd);
	buffer = new TCHAR[size + 1];
	memset(buffer, 0, (size + 1) * sizeof(TCHAR));
	GetWindowText(hwnd, buffer, size + 1);
	buffer[size] = NULL;
	WriteFile(hFile, buffer, (size) * sizeof(TCHAR), (LPDWORD)&size, NULL);
	free(buffer);
	CloseHandle(hFile);

}
void FileRead(HWND hwnd, LPTSTR filename)
{
	HANDLE hFile;
	LPTSTR buffer; 
	DWORD size, CharNum;
	int fileSize;

	hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
	if (hFile == INVALID_HANDLE_VALUE) return;
	fileSize =GetFileSize(hFile, &size);
	buffer = new TCHAR[(int)fileSize / sizeof(TCHAR) + 1];
	memset(buffer, 0, fileSize);
	ReadFile(hFile, buffer, fileSize, &CharNum, NULL);
	buffer[(int)fileSize / sizeof(TCHAR)] = NULL;
	SetWindowText(hwnd, buffer);
	free(buffer);
	CloseHandle(hFile);
}
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
		_T("박정호"),
		WS_OVERLAPPEDWINDOW,
		50,
		50,
		1000,
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
#define IDC_EDIT1 100
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	RECT rect;
	PAINTSTRUCT ps;
	OPENFILENAME OFN,SFN;
	static TCHAR filepath[1000], filename[100];
	TCHAR filter[] = _T("텍스트 파일 (*.txt)\0*.txt\0모든 파일 (*.*)\0*.*\0");
	static HWND hEdit;

	switch (iMsg)
	{
	case WM_CREATE:
		GetClientRect(hwnd, &rect);
		hEdit = CreateWindow(
			_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |	
			ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE, 0, 100,
			rect.right, rect.bottom, hwnd, (HMENU)IDC_EDIT1, hInst, NULL);
		break;
	case WM_SIZE:
		GetClientRect(hwnd, &rect);
		MoveWindow(hEdit, 0, 0, rect.right, rect.bottom, TRUE);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILEOPEN:
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner = hwnd;
			OFN.lpstrFile = filepath;
			OFN.nMaxFile = 1000;
			OFN.lpstrFileTitle = filename;
			OFN.nMaxFileTitle = 100;
			OFN.lpstrFilter = filter;
			OFN.Flags = OFN_EXPLORER;
			OFN.lpstrInitialDir = _T(".");
			GetOpenFileName(&OFN);

			FileRead(hEdit, filepath);
			break;
		case ID_FILESAVE:
			memset(&SFN, 0, sizeof(OPENFILENAME));
			SFN.lStructSize = sizeof(OPENFILENAME);
			SFN.hwndOwner = hwnd;
			SFN.lpstrFilter = filter;
			SFN.lpstrFile = filepath;
			SFN.nMaxFile = 1000;
			SFN.lpstrInitialDir = _T(".");
			GetSaveFileName(&SFN);

			FileWrite(hEdit, filepath);
			break;
		}
		break;
	case WM_PAINT:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}