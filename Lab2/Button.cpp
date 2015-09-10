#pragma once

#include "Button.h"

namespace Lab2
{
	Button::Button(HINSTANCE hInst, HWND parent, int x, int y, int width, int height, int id, 
		LPCWSTR text, DWORD additionalStyles) : Window(hInst, parent, x, y, width, height, text, WC_BUTTON, NULL, 
		WS_CHILD | WS_VISIBLE | BS_CENTER | BS_VCENTER | BS_PUSHBUTTON | BS_TEXT | additionalStyles, HMENU(id))
	{
	}

	Button::~Button()
	{
		DestroyWindow(ThisWindow);
	}

	bool Button::Start()
	{
		return Create();
	}

	BOOL Button::SetText(LPCWSTR text)
	{
		return Button_SetText(ThisWindow, text);
	}
}