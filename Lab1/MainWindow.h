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

		bool AddWindowMessages() override;
		int Start();
		BOOL Show(int nCmdShow);
		void SetupMenu();
		void CalcTextRect();

		LRESULT OnCommand(WPARAM, LPARAM) override;
		LRESULT OnCreate(WPARAM, LPARAM) override;
		LRESULT OnPaint(WPARAM, LPARAM) override;
		LRESULT OnTimer(WPARAM, LPARAM);
	};
}