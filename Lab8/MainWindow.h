#pragma once

#include "Lab8.h"
#include "ListBox.h"

namespace Lab8
{
	class MainWindow : public Window
	{
	public:
		MainWindow(HINSTANCE hInst, int nCmdShow, LPCWSTR appTitle, LPCWSTR mainWindowClassName, 
					int x, int y, int width, int height);
		~MainWindow();

		ListBox *ProcessesListBox, *ThreadsListBox, *ModulesListBox;
		vector<Process*> Processes;
		vector<Thread*> Threads;

		int Start();
		BOOL Show(int nCmdShow);

		LRESULT OnResize(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnCommand(WPARAM wParam, LPARAM lParam) override;

		int LoadSnapshot(DWORD desiredAccess = PROCESS_QUERY_LIMITED_INFORMATION |
			PROCESS_SET_INFORMATION);
		int LoadThreads(DWORD desiredAccess = THREAD_SET_LIMITED_INFORMATION |
			THREAD_QUERY_LIMITED_INFORMATION);

		void ChangeProcessClass(int messageId);
		void ChangeThreadPriority(int messageId);

		void ProcessesContextMenu(LPARAM lParam);
		void ThreadsContextMenu(LPARAM lParam);
	};
}