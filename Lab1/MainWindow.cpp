#pragma once

#include "MainWindow.h"

namespace Lab1
{
	MainWindow::MainWindow(HINSTANCE hInst, int nCmdShow, LPCWSTR appTitle, LPCWSTR mainWindowClassName,
		int x, int y, int width, int height) : Window(hInst, NULL, x, y, width, height, appTitle,
		mainWindowClassName, 0, WS_OVERLAPPEDWINDOW)
	{
		IsMainWindow = true;
		CurTextOffset = 0;
		Speed = 5;
		Run = true;
		TimerPtr = NULL;
	}

	MainWindow::~MainWindow()
	{
	}
	
	bool MainWindow::AddWindowMessages()
	{
		bool res = Window::AddWindowMessages();

		res = res && AddMessage(ThisWindow, WM_TIMER, this, ToFuncPointer(&MainWindow::OnTimer));

		return res;
	}

	int MainWindow::Start()
	{
		RegisterWindowClass(CS_HREDRAW | CS_VREDRAW, HBRUSH(COLOR_WINDOW), MAKEINTRESOURCE(IDC_CURSOR), 
			MAKEINTRESOURCE(IDI_ICON), MAKEINTRESOURCE(IDI_ICON_SMALL));

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

	LRESULT MainWindow::OnCommand(WPARAM wParam, LPARAM lParam)
	{
		int wmId = LOWORD(wParam); 
		//int wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_QUIT:
			OnClose(wParam, lParam);
			break;
		case IDM_GO:
			Run = true;
			break;
		case IDM_STOP:
			Run = false;
			break;
		case IDM_FASTER:
			Speed++;
			break;
		case IDM_SLOWER:
			Speed--;
			break;
		default:
			return Window::OnCommand(wParam, lParam);
		}
		return 0;
	}

	LRESULT MainWindow::OnCreate(WPARAM wParam, LPARAM lParam)
	{
		SetupMenu();
		TimerPtr = SetTimer(ThisWindow, IDT_STR_MOVE, TIMER_ELAPSE, NULL);

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(ThisWindow, &ps);
		GetTextExtentPoint(hdc, Title, lstrlen(Title), &TextSize);
		EndPaint(ThisWindow, &ps);
		CurTextOffset = -TextSize.cx;
		CalcTextRect();

		return 0;
	}

	LRESULT MainWindow::OnPaint(WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(ThisWindow, &ps);
		DrawText(hdc, Title, lstrlen(Title), &TextRect, 0);
		EndPaint(ThisWindow, &ps);
		return 0;
	}

	LRESULT MainWindow::OnTimer(WPARAM wParam, LPARAM lParam)
	{
		if (wParam != IDT_STR_MOVE)
			return DefWindowProc(ThisWindow, WM_TIMER, wParam, lParam);

		CurTextOffset += Run*Speed;
		CurTextOffset = CurTextOffset >= Width ? -TextSize.cx : CurTextOffset <= -TextSize.cx ? Width : CurTextOffset;
		CalcTextRect();
		RECT tmpRect;
		tmpRect.left = 0;
		tmpRect.top = 0;
		tmpRect.bottom = Height;
		tmpRect.right = Width;
		InvalidateRect(ThisWindow, &tmpRect, TRUE);
		
		return 0;
	}

	void MainWindow::SetupMenu()
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

		SetMenu(ThisWindow, hMenu);
	}

	void MainWindow::CalcTextRect()
	{
		TextRect.bottom = (Height + TextSize.cy) / 2;
		TextRect.top = (Height - TextSize.cy) / 2;
		TextRect.left = CurTextOffset;
		TextRect.right = CurTextOffset + TextSize.cx;
	}

}