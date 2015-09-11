#pragma once

#include "MainWindow.h"

namespace TemplateApp
{
	MainWindow::MainWindow(HINSTANCE hInst, int nCmdShow, LPCWSTR appTitle, LPCWSTR mainWindowClassName,
		int x, int y, int width, int height) : Window(hInst, nullptr, x, y, width, height, appTitle,
		mainWindowClassName, 0, WS_OVERLAPPEDWINDOW)
	{
		IsMainWindow = true;
	}

	MainWindow::~MainWindow()
	{
	}

	int MainWindow::Start()
	{
		RegisterWindowClass(CS_HREDRAW | CS_VREDRAW, HBRUSH(COLOR_WINDOW));

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
		default:
			return Window::OnCommand(wParam, lParam);
		}
		return 0;
	}

	LRESULT MainWindow::OnPaint(WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(ThisWindow, &ps);
		DrawText(hdc, Title, lstrlen(Title), &ps.rcPaint, 0);
		EndPaint(ThisWindow, &ps);
		return 0;
	}
}