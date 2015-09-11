#pragma once

#include "Edit.h"

namespace Lab2
{
	Edit::Edit(HINSTANCE hInst, HWND parent, int x, int y, int width, int height, int id, DWORD additionalStyles) :
		Window(hInst, parent, x, y, width, height, L"", WC_EDIT, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		ES_AUTOHSCROLL | ES_LEFT | additionalStyles, HMENU(id))
	{
	}

	Edit::~Edit()
	{
		DestroyWindow(ThisWindow);
	}

	int Edit::Start()
	{
		return Create();
	}

	BOOL Edit::SetText(LPCWSTR text)
	{
		return Edit_SetText(ThisWindow, text);
	}

	int Edit::GetText(LPWSTR buffer, int bufferSize)
	{
		return Edit_GetText(ThisWindow, buffer, bufferSize);
	}

	int Edit::GetTextLength()
	{
		return Edit_GetTextLength(ThisWindow);
	}

	BOOL Edit::Clear()
	{
		return SetText(L"");
	}
}