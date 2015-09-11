#pragma once

#include "Lab2.h"

namespace Lab2
{
	class Button : public Window
	{
	public:
		Button(HINSTANCE hInst, HWND parent, int x, int y, int width, int height, int id,
			LPCWSTR text, DWORD additionalStyles = 0);
		~Button();
		int Start();
		BOOL SetText(LPCWSTR text);
	};
}