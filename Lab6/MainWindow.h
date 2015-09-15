#pragma once

#include "Lab6.h"
#include "Button.h"

namespace Lab6
{
	class MainWindow : public Window
	{
	public:
		MainWindow(HINSTANCE hInst, int nCmdShow, LPCWSTR appTitle, LPCWSTR mainWindowClassName, 
					int x, int y, int width, int height);
		~MainWindow();

		Button *RunButton, *PauseButton;
		HANDLE ThreadsHandle[THREADS_COUNT];
		DWORD ThreadsId[THREADS_COUNT];

		int Start();
		BOOL Show(int nCmdShow);

		LRESULT OnDestroy(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnClose(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnCreate(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnResize(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnCommand(WPARAM wParam, LPARAM lParam) override;

		static DWORD WINAPI ThreadProc(LPVOID param);
		static void DrawStar(HDC hdc, int centerX, int centerY, int size, COLORREF color);

		static bool ThreadsNeeded[THREADS_COUNT];
		static HWND MainWindowHWND;
		static POINT Points[THREADS_COUNT];
		static int ThreadsSpeed[THREADS_COUNT], StarSize, MainWindowHeight;
	};
}