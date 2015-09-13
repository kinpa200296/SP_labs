#pragma once

#include "MainWindow.h"

namespace Lab4
{
	MainWindow::MainWindow(HINSTANCE hInst, int nCmdShow, LPCWSTR appTitle, LPCWSTR mainWindowClassName,
		int x, int y, int width, int height) : Window(hInst, nullptr, x, y, width, height, appTitle,
		mainWindowClassName, 0, WS_OVERLAPPEDWINDOW)
	{
		IsMainWindow = true;
		CurPosX = (width - BITMAP_SIZE) / 2;
		CurPosY = (height - BITMAP_SIZE) / 2;
		CurTargetX = (width - BITMAP_SIZE) / 2;
		CurTargetY = (height - BITMAP_SIZE) / 2;
		TimerPtr = NULL;
		CurFrame = 1;
		Speed = 5;

		Mario1DC = CreateCompatibleDC(nullptr);
		Mario2DC = CreateCompatibleDC(nullptr);
		Mario3DC = CreateCompatibleDC(nullptr);
		Mario4DC = CreateCompatibleDC(nullptr);
		HBITMAP bitmap = HBITMAP(LoadImage(Instance, MAKEINTRESOURCE(IDB_MARIO1), IMAGE_BITMAP,
			BITMAP_SIZE, BITMAP_SIZE, LR_DEFAULTCOLOR));
		SelectObject(Mario1DC, bitmap);
		bitmap = HBITMAP(LoadImage(Instance, MAKEINTRESOURCE(IDB_MARIO2), IMAGE_BITMAP,
			BITMAP_SIZE, BITMAP_SIZE, LR_DEFAULTCOLOR));
		SelectObject(Mario2DC, bitmap);
		bitmap = HBITMAP(LoadImage(Instance, MAKEINTRESOURCE(IDB_MARIO3), IMAGE_BITMAP,
			BITMAP_SIZE, BITMAP_SIZE, LR_DEFAULTCOLOR));
		SelectObject(Mario3DC, bitmap);
		bitmap = HBITMAP(LoadImage(Instance, MAKEINTRESOURCE(IDB_MARIO4), IMAGE_BITMAP,
			BITMAP_SIZE, BITMAP_SIZE, LR_DEFAULTCOLOR));
		SelectObject(Mario4DC, bitmap);
	}

	MainWindow::~MainWindow()
	{
		KillTimer(ThisWindow, IDT_MAIN_TIMER);

		DeleteObject(Mario1DC);
		DeleteObject(Mario2DC);
		DeleteObject(Mario3DC);
		DeleteObject(Mario4DC);
	}
	
	bool MainWindow::AddWindowMessages()
	{
		bool result = Window::AddWindowMessages();

		result = result && AddMessage(ThisWindow, WM_TIMER, this, ToFuncPointer(&MainWindow::OnTimer));
		result = result && AddMessage(ThisWindow, WM_LBUTTONDOWN, this,
			ToFuncPointer(&MainWindow::OnLeftButtonDown));

		return result;
	}

	int MainWindow::Start()
	{
		RegisterWindowClass(CS_HREDRAW | CS_VREDRAW, HBRUSH(COLOR_WINDOW), IDC_ARROW,
			MAKEINTRESOURCE(IDI_ICON), MAKEINTRESOURCE(IDI_ICON_SMALL), MAKEINTRESOURCE(IDR_MENU));

		int res = Create();

		if (res)
		{
			return res;
		}

		return 0;
	}

	BOOL MainWindow::Show(int nCmdShow)
	{
		return ShowWindow(ThisWindow, nCmdShow);
	}

	LRESULT MainWindow::OnCreate(WPARAM wParam, LPARAM lParam)
	{
		TimerPtr = SetTimer(ThisWindow, IDT_MAIN_TIMER, TIMER_ELAPSE, nullptr);

		return 0;
	}

	LRESULT MainWindow::OnCommand(WPARAM wParam, LPARAM lParam)
	{
		int wmId = LOWORD(wParam); 
		//int wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_QUIT:
			OnClose(wParam, lParam);
			break;
		default:
			return Window::OnCommand(wParam, lParam);
		}
		return 0;
	}

	LRESULT MainWindow::OnPaint(WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(ThisWindow, &ps);
		switch (CurFrame)
		{
		case 1:
			BitBlt(hdc, CurPosX, CurPosY, BITMAP_SIZE, BITMAP_SIZE, Mario1DC, 0, 0, SRCCOPY);
			break;
		case 2:
			BitBlt(hdc, CurPosX, CurPosY, BITMAP_SIZE, BITMAP_SIZE, Mario2DC, 0, 0, SRCCOPY);
			break;
		case 3:
			BitBlt(hdc, CurPosX, CurPosY, BITMAP_SIZE, BITMAP_SIZE, Mario3DC, 0, 0, SRCCOPY);
			break;
		case 4:
			BitBlt(hdc, CurPosX, CurPosY, BITMAP_SIZE, BITMAP_SIZE, Mario4DC, 0, 0, SRCCOPY);
			break;
		}
		EndPaint(ThisWindow, &ps);
		return 0;
	}

	LRESULT MainWindow::OnTimer(WPARAM wParam, LPARAM lParam)
	{
		if (wParam != IDT_MAIN_TIMER)
			return DefWindowProc(ThisWindow, WM_TIMER, wParam, lParam);
		
		if (CurTargetX == CurPosX && CurTargetY == CurPosY)
			CurFrame = 1;
		else CurFrame = CurFrame == 4 ? 1 : CurFrame + 1;

		CurPosX += CurTargetX - CurPosX > 0 ? Speed : CurTargetX - CurPosX < 0 ? -Speed : 0;
		CurPosY += CurTargetY - CurPosY > 0 ? Speed : CurTargetY - CurPosY < 0 ? -Speed : 0;

		CurPosX = abs(CurTargetX - CurPosX) < Speed ? CurTargetX : CurPosX;
		CurPosY = abs(CurTargetY - CurPosY) < Speed ? CurTargetY : CurPosY;

		RECT tmpRect;
		tmpRect.left = 0;
		tmpRect.top = 0;
		tmpRect.bottom = Height;
		tmpRect.right = Width;
		InvalidateRect(ThisWindow, &tmpRect, TRUE);

		return 0;
	}

	LRESULT MainWindow::OnLeftButtonDown(WPARAM wParam, LPARAM lParam)
	{
		CurTargetX = LOWORD(lParam) - BITMAP_SIZE / 2;
		CurTargetY = HIWORD(lParam) - BITMAP_SIZE / 2;
		return 0;
	}
}