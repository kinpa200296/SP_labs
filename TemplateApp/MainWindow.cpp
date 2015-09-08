#pragma once

#include "MainWindow.h"

namespace TemplateApp
{
	MainWindow::MainWindow(HINSTANCE hInst, int nCmdShow, LPCWSTR appTitle, LPCWSTR mainWindowClassName,
		int x, int y, int width, int height) : Window(hInst, NULL, x, y, width, height, appTitle,
		mainWindowClassName, 0, WS_OVERLAPPEDWINDOW)
	{
	}

	MainWindow::~MainWindow()
	{
	}

	int MainWindow::Start()
	{
		RegisterWindowClass(CS_HREDRAW | CS_VREDRAW, HBRUSH(COLOR_WINDOW));

		if (!Create())
			return 1;

		bool result = true;

		result = result && AddMessage(ThisWindow, WM_CLOSE, this, ToFuncPointer(&MainWindow::OnClose));
		result = result && AddMessage(ThisWindow, WM_COMMAND, this, ToFuncPointer(&MainWindow::OnCommand));
		result = result && AddMessage(ThisWindow, WM_SIZE, this, ToFuncPointer(&MainWindow::OnResize));

		if (!result)
			return 2;

		return 0;
	}

	BOOL MainWindow::Show(int nCmdShow)
	{
		return ShowWindow(ThisWindow, nCmdShow);
	}
	
	LRESULT MainWindow::OnClose(WPARAM wParam, LPARAM lParam)
	{
		PostQuitMessage(0);
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
		}
		return 0;
	}

	LRESULT MainWindow::OnResize(WPARAM wParam, LPARAM lParam)
	{
		Width = LOWORD(lParam);
		Height = HIWORD(lParam);
		return 0;
	}
}