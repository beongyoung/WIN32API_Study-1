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
		200, //윈도우 위치 x좌표
		300,//윈도우 위치 y좌표
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
	static int x = 20, y = 20;
	static int centerx = 140, centery = 140;
	static RECT rectView;
	static bool isGoing = false;

	switch (iMsg)
	{
	case WM_CREATE:
		GetClientRect(hwnd, &rectView); //윈도우 클라이언트 영역을 계산해 RECT변수에 저장
		break;

	case WM_KEYDOWN:
		if (wParam == VK_RIGHT && (x + 20) < 600)
			x += 40;
		else if (wParam == VK_LEFT && (x - 20) > 0)
			x -= 40;
		else if (wParam == VK_UP && (y - 20) > 0)
			y -= 40;
		else if (wParam == VK_DOWN && (y + 20) < 600)
			y += 40;
		if(wParam== VK_RETURN)
		{
			if (isGoing) isGoing = false;
			else isGoing = true;

			if (isGoing)
				SetTimer(hwnd, 1, 70, NULL);
			else
				KillTimer(hwnd, 1);
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_TIMER:
		if((x+20)<600) x += 10;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		Rectangle(hdc, 0, 0, 600, 600);
		Ellipse(hdc, x - 20, y - 20, x + 20, y + 20);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}