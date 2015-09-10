#pragma once

#include "Lab2.h"

namespace Lab2
{
	class Edit : public Window
	{
	public:
		Edit(HINSTANCE hInst, HWND parent, int x, int y, int width, int height, int id, DWORD additionalStyles = 0);
		~Edit();
		bool Start();
		BOOL SetText(LPCWSTR text);
		int GetText(LPWSTR buffer, int bufferSize);
		int GetTextLength();
		BOOL Clear();
	};
}