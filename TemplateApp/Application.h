#pragma once

#include "MainWindow.h"

namespace TemplateApp
{
	class Application
	{
	public:
		HINSTANCE AppInstance;
		WCHAR MainWindowClass[MAX_STR], AppTitle[MAX_STR];
		int CmdShow;
		MainWindow *GlobalWindow;

		Application(HINSTANCE hInst, int nCmdShow);
		~Application();

		void CreateWindows();
		int Run();
	};
}