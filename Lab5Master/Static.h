#pragma once

#include "Lab5Master.h"

namespace Lab5Master
{
	class Static : public Control
	{
	public:
		Static(UINT_PTR subClassId, HINSTANCE hInst, HWND parent, int x, int y, int width, int height, int id,
			LPCWSTR text, DWORD additionalStyles = 0);
		~Static();
		int Start();

		LRESULT OnCommand(WPARAM wParam, LPARAM lParam) override;
	};
}