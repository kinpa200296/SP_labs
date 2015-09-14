#pragma once

#include "Application.h"

namespace Lab5Master
{
	Application::Application(HINSTANCE hInst, int nCmdShow)
	{
		AppInstance = hInst;
		CmdShow = nCmdShow;
		GlobalWindow = nullptr;
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
		int x = (ScreenWidth - WIDTH) / 2;
		int y = (ScreenHeight - HEIGHT) / 2;

		GlobalWindow = new MainWindow(AppInstance, CmdShow, AppTitle, MainWindowClass, x, y, WIDTH, HEIGHT);
		return GlobalWindow->Start();
	}

	int Application::Run()
	{
		MSG msg;
		HACCEL hAccelTable;

		hAccelTable = LoadAccelerators(AppInstance, MAKEINTRESOURCE(IDR_HOTKEYS));

		Window::GlobalMessageMap.clear();
		Window::ProcessMessages = false;
		Window::QueuedMessages.clear();

		int res = CreateWindows();
		if (res != 0)
			return res;

		Window::ProcessMessages = true;
		Window::ResentQueuedMessages();
		
		GlobalWindow->Show(CmdShow);

		while (GetMessage(&msg, nullptr, 0, 0))
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
