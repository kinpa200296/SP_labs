#include "Lab1Pure.h"

HINSTANCE hInst;
WCHAR AppTitle[MAX_STR], AppClass[MAX_STR];
int ScreenWidth, ScreenHeight, WindowWidth, WindowHeight;
bool run = true;
int speed = 5, CurOffset = 0;
RECT TextRect;
SIZE TextSize;

void SetupMenu(HWND hwnd)
{
	HMENU hMenu, hSubMenu;

	hMenu = CreateMenu();

	hSubMenu = CreatePopupMenu();
	AppendMenu(hSubMenu, MF_STRING, IDM_QUIT, L"E&xit");
	AppendMenu(hMenu, MF_STRING | MF_POPUP, UINT(hSubMenu), L"&File");

	hSubMenu = CreatePopupMenu();
	AppendMenu(hSubMenu, MF_STRING, IDM_GO, L"&Go");
	AppendMenu(hSubMenu, MF_STRING, IDM_STOP, L"&Stop");
	AppendMenu(hSubMenu, MF_STRING, IDM_SLOWER, L"S&lower");
	AppendMenu(hSubMenu, MF_STRING, IDM_FASTER, L"&Faster");
	AppendMenu(hMenu, MF_STRING | MF_POPUP, UINT(hSubMenu), L"&Actions");

	SetMenu(hwnd, hMenu);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId = LOWORD(wParam);
	//int wmEvent = HIWORD(wParam);
	HDC hdc;
	PAINTSTRUCT ps;
	RECT TmpRect;

	switch (message)
	{
	case WM_CREATE:
		SetupMenu(hwnd);
		SetTimer(hwnd, IDT_STR_MOVE, TIMER_ELAPSE, NULL);
		hdc = BeginPaint(hwnd, &ps);
		GetTextExtentPoint(hdc, AppTitle, lstrlen(AppTitle), &TextSize);
		EndPaint(hwnd, &ps);
		CurOffset = -TextSize.cx;
		break;
	case WM_TIMER:
		if (wParam == IDT_STR_MOVE)
		{
			CurOffset += run*speed;
			if (CurOffset >= WindowWidth)
				CurOffset = -TextSize.cx;
			else if (CurOffset <= -TextSize.cx)
					CurOffset = WindowWidth;
			TextRect.bottom = (WindowHeight + TextSize.cy) / 2;
			TextRect.top = (WindowHeight - TextSize.cy) / 2;
			TextRect.left = CurOffset;
			TextRect.right = CurOffset + TextSize.cx;
			TmpRect.left = 0;
			TmpRect.top = 0;
			TmpRect.bottom = WindowHeight;
			TmpRect.right = WindowWidth;
			InvalidateRect(hwnd, &TmpRect, TRUE);
		}
		break;
	case WM_COMMAND:
		switch (wmId)
		{
		case IDM_QUIT:
			DestroyWindow(hwnd);
			break;
		case IDM_GO:
			run = true;
			break;
		case IDM_STOP:
			run = false;
			break;
		case IDM_SLOWER:
			speed--;
			break;
		case IDM_FASTER:
			speed++;
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		WindowWidth = LOWORD(lParam);
		WindowHeight = HIWORD(lParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		DrawText(hdc, AppTitle, lstrlen(AppTitle), &TextRect, 0);
		EndPaint(hwnd, &ps);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

ATOM RegAppWindowCLass()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
	wcex.lpszClassName = AppClass;
	wcex.hCursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR));
	wcex.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON));
	wcex.hIconSm = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_SMALL));
	wcex.lpszMenuName = NULL;

	return RegisterClassEx(&wcex);
}

BOOL CreateAppWindow(int nCmdShow)
{
	ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	int x = (ScreenWidth - WIDTH) / 2, y = (ScreenHeight - HEIGHT) / 2;


	HWND hwnd = CreateWindow(AppClass, AppTitle, WS_OVERLAPPEDWINDOW, x, y, WIDTH, HEIGHT, NULL, NULL, hInst, NULL);

	if (!hwnd)
	{
		return FALSE;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	return TRUE;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	hInst = hInstance;
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	HACCEL hAccelTable;

	hAccelTable = LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_HOTKEYS));

	LoadString(hInst, IDC_APP_CLASS, AppClass, MAX_STR);
	LoadString(hInst, IDS_APP_TITLE, AppTitle, MAX_STR);

	RegAppWindowCLass();
	if (!CreateAppWindow(nCmdShow))
	{
		return 1;
	}

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	return msg.wParam;
}