#include <Windows.h>
#include<tchar.h>
#include<string.h>
#include<math.h>
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

	RegisterClass(&WndClass); //윈도우 클래스를 커널에 등록

	hwnd = CreateWindow( //윈도우 핸들값 반환
		_T("Window Class Name"), //윈도우 클래스 이름
		_T("홍길동의 첫 번째 윈도우"), //윈도우 타이틀이름
		WS_OVERLAPPEDWINDOW, //윈도우 스타일
		100, //윈도우 위치 x좌표
		100,//윈도우 위치 y좌표
		1200, //윈도우 가로크기
		700, //윈도우 세로크기
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
	HPEN hPen, oldPen;
	static int centerX = 0, mX = 0, centerY = 0, mY = 0;
	static bool InCircle = false;
	static bool Drag= false;
	HBRUSH hBrush, orgBrush;
	static int red[30] = { 0, }, green[30] = { 255, }, blue[30] = { 0, };
	static int x[30], y[30], R[30];

	switch (iMsg)
	{
	case WM_CREATE:
		for (int i = 0; i < 30; i++)
		{
			red[i] = 0;
			green[i] = 255;
			blue[i] = 0;
			x[i] = rand() % 1200;
			y[i] = rand() % 700;
			R[i] = rand() % 70 + 10;
		}
		SetTimer(hwnd, 1, 100, NULL);
		break;

	case WM_KEYDOWN:
		break;
	case WM_LBUTTONDOWN:
		Drag = true;
		mX = LOWORD(lParam);	
		mY = HIWORD(lParam);

		if ((pow(mX - centerX, 2) + pow(mY - centerY, 2)) < pow(50, 2))
			InCircle = true;

		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_LBUTTONUP:
		mX = LOWORD(lParam);
		mY = HIWORD(lParam);

		InCircle = false;
		Drag = false;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_TIMER:
		//InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_MOUSEMOVE:
		if (InCircle)
		{
			centerX = mX = LOWORD(lParam);
			centerY = mY = HIWORD(lParam);
		}
		for (int i = 0; i < 30; i++)
		{
			if (pow(centerX - x[i], 2) + pow(centerY - y[i], 2) < pow(50 + R[i], 2))
			{
				red[i] = 255;
				green[i] = 0;
				blue[i] = 0;
			}
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		orgBrush= CreateSolidBrush(RGB(255,255,255));
		for (int i = 0; i < 30; i++)
		{
			hBrush = CreateSolidBrush(RGB(red[i], green[i], blue[i]));		
			SelectObject(hdc, hBrush);		
			//SelectObject(hdc, (HBRUSH)GetStockObject(RGB(0, 255, 0)));
			Ellipse(hdc, x[i] - R[i], y[i]- R[i], x[i] + R[i], y[i]+ R[i]);
		}
		SelectObject(hdc, orgBrush);				
		Ellipse(hdc, centerX - 50, centerY - 50, centerX + 50, centerY + 50); //주인공


		DeleteObject(hBrush);				
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}