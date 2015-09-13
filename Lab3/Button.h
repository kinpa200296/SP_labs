#pragma once

#include "Lab3.h"

namespace Lab3
{
	class Button : public Control
	{
	public:
		Button(UINT_PTR subClassId, HINSTANCE hInst, HWND parent, int x, int y, int width, int height, int id,
			LPCWSTR text, DWORD additionalStyles = 0);
		~Button();

		HDC DrawButtonDC, ClearButtonDC;

		int Start();
		BOOL SetText(LPCWSTR text);

		LRESULT OnCommand(WPARAM wParam, LPARAM lParam) override;
		LRESULT OwnerDrawProc(WPARAM wParam, LPARAM lParam);
	};
}