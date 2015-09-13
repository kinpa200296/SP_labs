#pragma once

#include "Lab4.h"

namespace Lab4
{
	class MainWindow : public Window
	{
	public:
		MainWindow(HINSTANCE hInst, int nCmdShow, LPCWSTR appTitle, LPCWSTR mainWindowClassName, 
					int x, int y, int width, int height);
		~MainWindow();

		int CurPosX, CurPosY, CurTargetX, CurTargetY, CurFrame, Speed;
		UINT_PTR TimerPtr;
		HDC Mario1DC, Mario2DC, Mario3DC, Mario4DC;

		bool AddWindowMessages() override;

		int Start();
		BOOL Show(int nCmdShow);

		LRESULT OnCreate(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnPaint(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnCommand(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnTimer(WPARAM wParam, LPARAM lParam);
		LRESULT OnLeftButtonDown(WPARAM wParam, LPARAM lParam);
	};
}