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
		_T("ȫ�浿�� ù ��° ������"),
		WS_OVERLAPPEDWINDOW,
		100, //������ ��ġ x��ǥ
		100,//������ ��ġ y��ǥ
		1000, //������ ����ũ��
		1000, //������ ����ũ��
		NULL, //�θ� �������ڵ�
		NULL, //�޴��ڵ�
		hInstance, //�������α׷� �ν��Ͻ�
		NULL); //���� ������ ����

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
	TCHAR filter[] = _T("C++�ҽ��� ��� ����\0*.cpp;*.h\0Every File(*.*)\0*.*\0Text File\0*.txt;*.doc\0");
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
		case ID_FILENEW:		//����
			answer = MessageBox(hwnd, _T("�� ���� ���ðڽ��ϱ�?"), _T("���� ����"), MB_OKCANCEL);
			if (answer == IDOK)
			{

			}
			break;
		case ID_FILEOPEN:		//����							
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner = hwnd;
			OFN.lpstrFilter = filter;
			OFN.lpstrFile = lpstrFile;
			OFN.nMaxFile = 256;
			OFN.lpstrInitialDir = _T(".");
			if (GetOpenFileName(&OFN) != 0)
			{
				_stprintf_s(str, _T("%s ������ ���ðڽ��ϱ�?"), OFN.lpstrFile);
				answer = MessageBox(hwnd, str, _T("���⼱��"), MB_OKCANCEL);
				if (answer == IDOK)
				{
					hdc = GetDC(hwnd);
#ifdef _UNICODE
					_tfopen_s(&fPtr, OFN.lpstrFile, _T("r,ccs=UNICODE"));		//���������ϱ�->���� ����� ������ 0��ȯ
																			//filename : �������ϴ� �����̸�
#else														//fPtr : �����ּ� ����
					_tfopen_s(&file, filename, _T("r"));
#endif
					while (_fgetts(buffer, 100, fPtr) != NULL)				//fPtr���Ͽ��� �� ���� �о�� buffer�� ����, 100�� buffer����ũ��
					{														//������ ���� ���� ������ �ٴ����� buffer�� �о����
						if (buffer[_tcsclen(buffer) - 1] == _T('\n'))		//buffer�� ������ ���ڰ� ���๮���̸� 
							buffer[_tcsclen(buffer) - 1] = NULL;			//NULL�� ������
						TextOut(hdc, 0, line * 20, buffer, _tcslen(buffer));//�����쿡 ���
						line++;
					}
					fclose(fPtr);
					ReleaseDC(hwnd, hdc);
				}
			}

			break;

		case ID_FILESAVE:		//����
			hdc = GetDC(hwnd);
			memset(&SFN, 0, sizeof(OPENFILENAME));
			SFN.lStructSize = sizeof(OPENFILENAME);
			SFN.hwndOwner = hwnd;
			SFN.lpstrFilter = filter;
			SFN.lpstrFile = lpstrFile; //�����̸�
			SFN.nMaxFile = 256;
			SFN.lpstrInitialDir = _T(".");
			if (GetSaveFileName(&SFN) != 0)
			{
				_stprintf_s(str, _T("%s ������ �����Ͻðڽ��ϱ�?"), SFN.lpstrFile);
				answer = MessageBox(hwnd, str, _T("�����ϱ� ����"), MB_OKCANCEL);
				if (answer == IDOK)
				{
					_tfopen_s(&fPtr, SFN.lpstrFile, _T("wt"));
					if (fPtr == NULL)
					{
						MessageBox(hwnd, _T("�������忡��"), _T("����"), MB_OK);
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
		case ID_EDITUNDO:		//�������
			MessageBox(hwnd, _T("������ ��� �Ͻðڽ��ϱ�?"), _T("������� ����"), MB_OKCANCEL);
			break;
		case ID_EXIT:			//������
			answer = MessageBox(hwnd, _T("�����ðڽ��ϱ�?"), _T("������ ����"), MB_OKCANCEL);
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