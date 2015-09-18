#pragma once

#include "Lab9.h"
#include "ListView.h"

namespace Lab9
{
	class MainWindow : public Window
	{
	public:
		MainWindow(HINSTANCE hInst, int nCmdShow, LPCWSTR appTitle, LPCWSTR mainWindowClassName, 
					int x, int y, int width, int height);
		~MainWindow();

		ListView *MainListView;
		HANDLE ThreadHandle;
		DWORD ThreadId;

		int Start();
		BOOL Show(int nCmdShow);

		LRESULT OnDestroy(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnClose(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnCreate(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnResize(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnCommand(WPARAM wParam, LPARAM lParam) override;

		static DWORD WINAPI ThreadProc(LPVOID param);
		static void SearchRegistry(HKEY root, LPWSTR subkey, ListView *listView);
		static bool SearchAgain, ThreadNeeded;
	};
}