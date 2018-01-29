#include <Windows.h>
#include<tchar.h>
#include<stdio.h>
#include<math.h>
#include "resource.h"
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;

	WNDCLASS WndClass;
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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	RegisterClass(&WndClass);

	hwnd = CreateWindow(
		_T("Window Class Name"),
		_T("홍길동의 첫 번째 윈도우"),
		WS_OVERLAPPEDWINDOW,
		100, //윈도우 위치 x좌표
		100,//윈도우 위치 y좌표
		1000, //윈도우 가로크기
		1000, //윈도우 세로크기
		NULL, //부모 윈도우핸들
		NULL, //메뉴핸들
		hInstance, //응용프로그램 인스턴스
		NULL); //생성 윈도우 정보

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
	OPENFILENAME OFN;
	OPENFILENAME SFN;
	int answer;
	TCHAR filter[] = _T("C++소스와 헤더 파일\0*.cpp;*.h\0Every File(*.*)\0*.*\0Text File\0*.txt;*.doc\0");
	TCHAR lpstrFile[100] = _T("");
	static TCHAR str[100];
	static int line = 0;
	FILE *fPtr;
	TCHAR buffer[500];

	switch (iMsg)
	{
	case WM_CREATE:
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILENEW:		//새글
			answer = MessageBox(hwnd, _T("새 글을 여시겠습니까?"), _T("새글 선택"), MB_OKCANCEL);
			if (answer == IDOK)
			{

			}
			break;
		case ID_FILEOPEN:		//열기							
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner = hwnd;
			OFN.lpstrFilter = filter;
			OFN.lpstrFile = lpstrFile;
			OFN.nMaxFile = 256;
			OFN.lpstrInitialDir = _T(".");
			if (GetOpenFileName(&OFN) != 0)
			{
				_stprintf_s(str, _T("%s 파일을 여시겠습니까?"), OFN.lpstrFile);
				answer = MessageBox(hwnd, str, _T("열기선택"), MB_OKCANCEL);
				if (answer == IDOK)
				{
					hdc = GetDC(hwnd);
#ifdef _UNICODE
					_tfopen_s(&fPtr, OFN.lpstrFile, _T("r,ccs=UNICODE"));		//파일저장하기->파일 제대로 열리면 0반환
																			//filename : 열고자하는 파일이름
#else														//fPtr : 파일주소 저장
					_tfopen_s(&file, filename, _T("r"));
#endif
					while (_fgetts(buffer, 100, fPtr) != NULL)				//fPtr파일에서 한 줄을 읽어와 buffer에 저장, 100은 buffer공간크기
					{														//파일의 끝을 만날 때까지 줄단위로 buffer에 읽어들임
						if (buffer[_tcsclen(buffer) - 1] == _T('\n'))		//buffer의 마지막 문자가 개행문자이면 
							buffer[_tcsclen(buffer) - 1] = NULL;			//NULL로 변경후
						TextOut(hdc, 0, line * 20, buffer, _tcslen(buffer));//윈도우에 출력
						line++;
					}
					fclose(fPtr);
					ReleaseDC(hwnd, hdc);
				}
			}

			break;

		case ID_FILESAVE:		//저장
			hdc = GetDC(hwnd);
			memset(&SFN, 0, sizeof(OPENFILENAME));
			SFN.lStructSize = sizeof(OPENFILENAME);
			SFN.hwndOwner = hwnd;
			SFN.lpstrFilter = filter;
			SFN.lpstrFile = lpstrFile; //파일이름
			SFN.nMaxFile = 256;
			SFN.lpstrInitialDir = _T(".");
			if (GetSaveFileName(&SFN) != 0)
			{
				_stprintf_s(str, _T("%s 파일을 저장하시겠습니까?"), SFN.lpstrFile);
				answer = MessageBox(hwnd, str, _T("저장하기 선택"), MB_OKCANCEL);
				if (answer == IDOK)
				{
					_tfopen_s(&fPtr, SFN.lpstrFile, _T("wt"));
					if (fPtr == NULL)
					{
						MessageBox(hwnd, _T("파일저장에러"), _T("에러"), MB_OK);
						break;
					}
					for (int i = 0; i < 256; i++)
					{
						_fputts(&buffer[i], fPtr);
						if (feof(fPtr) != 0)
							break;
					}
					fclose(fPtr);
				}
			}
			ReleaseDC(hwnd, hdc);
			break;
		case ID_EDITUNDO:		//편집취소
			MessageBox(hwnd, _T("편집을 취소 하시겠습니까?"), _T("편집취소 선택"), MB_OKCANCEL);
			break;
		case ID_EXIT:			//끝내기
			answer = MessageBox(hwnd, _T("끝내시겠습니까?"), _T("끝내기 선택"), MB_OKCANCEL);
			if (answer == IDOK)
				PostQuitMessage(0);
			break;
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		//TextOut(hdc, 0, 0, _T("helloHelloWorld"), _tcslen(_T("helloHelloWorld")));
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}