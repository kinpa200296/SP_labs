#pragma once

#include "Lab8.h"

namespace Lab8
{
	class ListBox : public Control
	{
	public:
		ListBox(UINT_PTR subClassId, HINSTANCE hInst, HWND parent, int x, int y, int width, int height, int id,
					DWORD additional_Styles = 0);
		~ListBox();

		bool AddWindowMessages() override;

		int Start();

		int AddString(LPCWSTR str);
		int DeleteString(int index);
		int GetString(int index, LPWSTR buffer);
		void Clear();

		LRESULT OnCommand(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnRightButtonDown(WPARAM wParam, LPARAM lParam);

		void DisplayProcesses(vector<Process*> &processes);
		void DisplayModules(vector<Module*> &modules);
		void DisplayThreads(vector<Thread*> &threads);
	};
}