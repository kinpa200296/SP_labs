#pragma once

#include "Lab7.h"

namespace Lab7
{
	class MainWindow : public Window
	{
	public:
		MainWindow(HINSTANCE hInst, int nCmdShow, LPCWSTR appTitle, LPCWSTR mainWindowClassName, 
					int x, int y, int width, int height);
		~MainWindow();

		HANDLE ThreadsHandle[THREADS_COUNT];
		DWORD ThreadsId[THREADS_COUNT];
		UINT_PTR TimerPtr;

		bool AddWindowMessages() override;

		int Start();
		BOOL Show(int nCmdShow);

		LRESULT OnDestroy(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnClose(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnCreate(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnResize(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnPaint(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnCommand(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnTimer(WPARAM wParam, LPARAM lParam);

		static DWORD WINAPI ThreadProc(LPVOID param);

		static bool ThreadsNeeded[THREADS_COUNT];
		static HWND MainWindowHWND;
		static int CurPosX, CurPosY;

#ifdef USE_EVENT
		static HANDLE Event;
#else
		static CRITICAL_SECTION CriticalSection;
#endif

	};
}