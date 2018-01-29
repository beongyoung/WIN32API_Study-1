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
	HPEN hPen, oldPen;
	static int startX = 20, startY = 20;
	static int oldX = 20, oldY = 20, endX, endY;
	static int mx = 0, my = 0;
	static bool Drag = false;
	static int centerX = 20, centerY = 20;

	switch (iMsg)
	{
	case WM_CREATE:
		break;

	case WM_KEYDOWN:
		break;

	case WM_LBUTTONDOWN:
		Drag = true;
		mx = HIWORD(lParam);
		my = LOWORD(lParam);
		//InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_LBUTTONUP:
		Drag = false;
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		oldX = endX = 20;
		oldY = endY = 20;
		SetTimer(hwnd, 1, 10, NULL);
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_TIMER:
		if (centerX == mx&&centerY == my)
			KillTimer(hwnd, 1);
		else
		{
			if (mx > centerX)
				centerX += 1;
			else
				centerX -= 1;
			if (my > centerY)
				centerY += 1;
			else
				centerY -= 1;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hwnd);

		if (Drag)
		{
			SetROP2(hdc, R2_XORPEN);								//XOR을 래스터 연산으로 설정
			SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));		//hdc에 흰색펜을 등록
			//SelectObject(hdc, (HBRUSH)GetStockObject(BLACK_BRUSH)); //hdc에 검은색 브러시를 등록
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);

			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, oldX, oldY);
			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, endX, endY);
			oldX = endX;
			oldY = endY;
			//InvalidateRgn(hwnd, NULL, TRUE); 깜빡임 방지
		}
		ReleaseDC(hwnd, hdc);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		Ellipse(hdc, centerX-20, centerY-20, centerX+20, centerY+20);
		MoveToEx(hdc, startX, startY, NULL);
		LineTo(hdc, oldX, oldY);

		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}