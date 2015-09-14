#pragma once

#include "MainWindow.h"

namespace Lab5Master
{
	class Application
	{
	public:
		HINSTANCE AppInstance;
		WCHAR MainWindowClass[MAX_STR], AppTitle[MAX_STR];
		int CmdShow, ScreenWidth, ScreenHeight;
		MainWindow *GlobalWindow;

		Application(HINSTANCE hInst, int nCmdShow);
		~Application();

		void ReadSystemMetrics();
		int CreateWindows();
		int Run();
	};
}