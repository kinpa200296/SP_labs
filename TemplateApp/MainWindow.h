#pragma once

#include "TemplateApp.h"

namespace TemplateApp
{
	class MainWindow : public Window
	{
	public:
		MainWindow(HINSTANCE hInst, int nCmdShow, LPCWSTR appTitle, LPCWSTR mainWindowClassName, 
					int x, int y, int width, int height);
		~MainWindow();

		int Start();
		BOOL Show(int nCmdShow);

		LRESULT OnDestroy(WPARAM, LPARAM);
		LRESULT OnClose(WPARAM, LPARAM);
		LRESULT OnCreate(WPARAM, LPARAM);
		LRESULT OnPaint(WPARAM, LPARAM);
		LRESULT OnCommand(WPARAM, LPARAM);
		LRESULT OnResize(WPARAM, LPARAM);
		LRESULT OnMove(WPARAM, LPARAM);
	};
}