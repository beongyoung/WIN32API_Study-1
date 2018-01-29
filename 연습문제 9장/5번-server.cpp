//����ŷ �ܹ�����ſ��� ������ Ŭ���̾�Ʈ�� �޽����� ������ �ڵ带 �߰��Ѵ�.
#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<stdio.h>
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
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = _T("Window Class Name");

	RegisterClass(&WndClass);

	hwnd = CreateWindow(
		_T("Window Class Name"),
		_T("Server Window"),
		WS_OVERLAPPEDWINDOW,
		50,
		50,
		600,
		600,
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
#define GAP 25
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static WSADATA wsadata;
	static SOCKET s, cs;
	static TCHAR msg[10];
	static HWND hButton1;


	static SOCKADDR_IN addr = { 0 }, c_addr;
	static int recv_x1 = 0, recv_x2 = 0, recv_y1 = 0, recv_y2 = 0;
	static int recv_shape = 0;
	static int pos_y = 0;
	static int mx = 0, my = 0;

	static bool Drag = false;
	static bool MyTurn = TRUE;
	static int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	static int shape = 0;
	static int token2[2];

	HPEN redPen, blackPen, bluePen;
	static int  mX, mY;
	static int circle[20][20] = { 0, };
	static int recv_circle[20][20] = { 0, };

	int  size, msgLen;
	char buffer[100];
	char *context;
	char *token;
	int i = 0;

	switch (iMsg)
	{
	case WM_CREATE:
		WSAStartup(MAKEWORD(2, 2), &wsadata);			//1.���ӻ������ϱ�
		s = socket(AF_INET, SOCK_STREAM, 0);			//2.���� ����-Ŭ���̾�Ʈ�� �����ϱ⸦ ��ٸ��� ����
		addr.sin_family = AF_INET;
		addr.sin_port = 20;
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		bind(s, (LPSOCKADDR)&addr, sizeof(addr));		//3.�ּҿ� ���� �����ϱ�
		WSAAsyncSelect(s, hwnd, WM_ASYNC, FD_ACCEPT);	//4.����s���� ������ ������ �õ��ϴ� �̺�Ʈ�� �߻��ϸ� WM_ASYNC�� hwnd �����쿡 �߻���Ű���� ����
		if (listen(s, 5) == -1)							//5.���� �䱸 ��ٸ���
			return 0;
		break;
	case WM_ASYNC:										//WM_ASYNC�޽��� �߻��� lParam���� � �̺�Ʈ�� �߻��ߴ��� ����
		switch (lParam)
		{
		case FD_ACCEPT:									//Ŭ���̾�Ʈ�� ������ �õ��ϸ�
			size = sizeof(c_addr);						//Ŭ���̾�Ʈ�� ����ϱ� ���� ����cs�� ����� Ŭ���̾�Ʈ���� ����� �����Ų��. 
			cs = accept(s, (LPSOCKADDR)&c_addr, &size);
			WSAAsyncSelect(cs, hwnd, WM_ASYNC, FD_READ);//Ŭ���̾�Ʈ�� �޽����� ���� ������ ������ ��ٸ� �� �����Ƿ� FD_READ�� WM_ASYNC�� ���
			break;
		case FD_READ:
			recv(cs, buffer, 100, 0);
			if (!_tcscmp(buffer, _T("-1")))
			{
				MessageBox(hwnd, _T("�ƽ��׿�. �����ϴ�"), _T("������ �ѹ� ��"), NULL);
				MyTurn = FALSE;
				return 0;
			}
			strcpy_s(msg, buffer);
			context = NULL;
			token = strtok_s(msg, ",", &context);			//context���� �и��� �� ���� ���ڿ��� ����.
			while (token != NULL)
			{
				token2[i++] = atoi(token);
				//TextOut(hdc, 0, pos_y, token, _tcslen(token));
				token = strtok_s(NULL, ",", &context);
				pos_y += 20;
			}
			recv_circle[token2[0]][token2[1]] = 1;
			MyTurn = TRUE;
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		default:
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		if (MyTurn)
		{
			int sum[8] = { 0, };
			mX = LOWORD(lParam);
			mY = HIWORD(lParam);
			if ((mY / GAP) < 20 && (mX / GAP) < 20 && circle[mY / GAP][mX / GAP] == 0 && recv_circle[mY / GAP][mX / GAP] == 0)
			{
				circle[mY / GAP][mX / GAP] = 1;
				_stprintf_s(buffer, _T("%d,%d"), mY / GAP, mX / GAP);
				send(cs, (LPSTR)buffer, 10, 0);
				MyTurn = FALSE;

				for (int i = 1; i <= 4; i++)
				{
					if (mY / GAP - i >= 0 && mY / GAP + i < 20)		//���Ʒ�
					{
						sum[0] += circle[mY / GAP - i][mX / GAP];
						sum[1] += circle[mY / GAP + i][mX / GAP];
					}
					if (mY / GAP - i >= 0 && mY / GAP + i < 20 && mX / GAP - i >= 0 && mX / GAP + i < 20)		//��������,���ʾƷ�
					{
						sum[2] += circle[mY / GAP - i][mX / GAP + i];
						sum[3] += circle[mY / GAP + i][mX / GAP - i];
					}
					if (mX / GAP - i >= 0 && mX / GAP + i < 20)		//������,����
					{
						sum[4] += circle[mY / GAP][mX / GAP + i];
						sum[5] += circle[mY / GAP][mX / GAP - i];
					}
					if (mY / GAP - i >= 0 && mY / GAP + i < 20 && mX / GAP - i >= 0 && mX / GAP + i < 20)		//�����ʾƷ�,������
					{
						sum[6] += circle[mY / GAP + i][mX / GAP + i];
						sum[7] += circle[mY / GAP - i][mX / GAP - i];
					}
				}

			}
			InvalidateRgn(hwnd, NULL, TRUE);
			if (sum[0] + sum[1] >= 4 || sum[2] + sum[3] >= 4 || sum[4] + sum[5] >= 4 || sum[6] + sum[7] >= 4)
			{
				MessageBox(hwnd, _T("���� �մϴ�. ����� �̰���ϴ�"), _T("�¸� ����"), NULL);
				MyTurn = FALSE;
				_stprintf_s(buffer, _T("-1"));
				send(cs, (LPSTR)buffer, 10, 0);
			}
		}
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
		for (int i = 0; i < 21; i++)			//--------------������
		{
			MoveToEx(hdc, 0, i * GAP, NULL);
			LineTo(hdc, GAP * 20, i * GAP);
		}
		for (int i = 0; i < 21; i++)
		{
			MoveToEx(hdc, i * GAP, 0, NULL);
			LineTo(hdc, i * GAP, GAP * 20);
		}									//--------------������

		redPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				SelectObject(hdc, redPen);
				if (circle[i][j])
					Ellipse(hdc, GAP * j, GAP * i, GAP * j + GAP, GAP * i + GAP);
			}
		}

		bluePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				SelectObject(hdc, bluePen);
				if (recv_circle[i][j])
					Ellipse(hdc, GAP * j, GAP * i, GAP * j + GAP, GAP * i + GAP);
			}
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