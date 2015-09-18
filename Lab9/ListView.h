#pragma once

#include "Lab9.h"

namespace Lab9
{
	class ListView : public Control
	{
	public:
		ListView(UINT_PTR subClassId, HINSTANCE hInst, HWND parent, int x, int y, int width, int height, int id,
					DWORD additional_Styles = 0);
		~ListView();

		int Start();

		void Clear();
		int AddColumn(LPWSTR text, int index, int width);
		int AddItem(LPWSTR text, int row, int column);

		LRESULT OnCommand(WPARAM wParam, LPARAM lParam) override;
	};
}