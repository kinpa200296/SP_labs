#pragma once

#include "Application.h"

namespace TemplateApp
{
	Application::Application(HINSTANCE hInst, int nCmdShow)
	{
		AppInstance = hInst;
		CmdShow = nCmdShow;
		GlobalWindow = NULL;
		LoadString(AppInstance, IDS_TITLE, AppTitle, MAX_STR);
		LoadString(AppInstance, IDC_MAINWINDOWCLASS, MainWindowClass, MAX_STR);
		ReadSystemMetrics();
	}
	
	Application::~Application()
	{
		GlobalWindow->~MainWindow();
	}

	int Application::CreateWindows()
	{
		GlobalWindow = new MainWindow(AppInstance, CmdShow, AppTitle, MainWindowClass, 0, 0, WIDTH, HEIGHT);
		return GlobalWindow->Start();
	}

	int Application::Run()
	{
		MSG msg;
		HACCEL hAccelTable;

		hAccelTable = LoadAccelerators(AppInstance, MAKEINTRESOURCE(IDR_HOTKEYS));

		int res = CreateWindows();
		if (res != 0)
			return res;
		
		GlobalWindow->Show(CmdShow);

		while (GetMessage(&msg, NULL, 0, 0))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		Window::GlobalMessageMap.clear();

		return msg.wParam;
	}

	void Application::ReadSystemMetrics()
	{
		ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	}

}
