#pragma once

#include "Lab2.h"

namespace Lab2
{
	class ListBox : public Window
	{
	public:
		ListBox(HINSTANCE hInst, HWND parent, int x, int y, int width, int height, int id, 
					DWORD additional_Styles = 0);
		~ListBox();
		bool Start();
		int AddString(LPCWSTR str);
		int DeleteString(int index);
		int GetString(int index, LPWSTR buffer);
		void Clear();
		void ClearSelection();
		bool AddStringUnique(LPCWSTR str);
		vector<int> GetSelectedItemsIndex();
	};
}