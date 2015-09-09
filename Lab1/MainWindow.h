#pragma once

#include "Lab1.h"

namespace Lab1
{
	class MainWindow : public Window
	{
	public:
		MainWindow(HINSTANCE hInst, int nCmdShow, LPCWSTR appTitle, LPCWSTR mainWindowClassName, 
					int x, int y, int width, int height);
		~MainWindow();

		int CurTextOffset, Speed;
		SIZE TextSize;
		RECT TextRect;
		UINT_PTR TimerPtr;
		bool Run;

		int Start();
		BOOL Show(int nCmdShow);
		void SetupMenu();
		void CalcTextRect();

		LRESULT OnDestroy(WPARAM, LPARAM);
		LRESULT OnClose(WPARAM, LPARAM);
		LRESULT OnCommand(WPARAM, LPARAM);
		LRESULT OnResize(WPARAM, LPARAM);
		LRESULT OnCreate(WPARAM, LPARAM);
		LRESULT OnPaint(WPARAM, LPARAM);
		LRESULT OnTimer(WPARAM, LPARAM);
	};
}