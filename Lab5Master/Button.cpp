#pragma once

#include "Button.h"

namespace Lab5Master
{
	Button::Button(UINT_PTR subClassId, HINSTANCE hInst, HWND parent, int x, int y, int width, int height, int id,
		LPCWSTR text, DWORD additionalStyles) : Control(subClassId, hInst, parent, x, y, width, height, text,
		WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | BS_LEFT | BS_VCENTER | BS_PUSHBUTTON | BS_TEXT | additionalStyles,
		HMENU(id))
	{
	}

	Button::~Button()
	{
		DestroyWindow(ThisWindow);
	}

	int Button::Start()
	{
		return Create();
	}

	BOOL Button::SetText(LPCWSTR text)
	{
		return Button_SetText(ThisWindow, text);
	}

	LRESULT Button::OnCommand(WPARAM wParam, LPARAM lParam)
	{
		int wmId = LOWORD(wParam);
		//int wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_QUIT:
			SendMessage(Parent, WM_COMMAND, wParam, lParam);
			break;
		default:
			return Control::OnCommand(wParam, lParam);
		}
		return 0;
	}
}