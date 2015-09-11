#pragma once

#include "Lab2.h"

namespace Lab2
{
	class Button : public Control
	{
	public:
		Button(UINT_PTR subClassId, HINSTANCE hInst, HWND parent, int x, int y, int width, int height, int id,
			LPCWSTR text, DWORD additionalStyles = 0);
		~Button();
		int Start();
		BOOL SetText(LPCWSTR text);

		LRESULT OnCommand(WPARAM wParam, LPARAM lParam) override;
	};
}