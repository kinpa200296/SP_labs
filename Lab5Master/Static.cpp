#pragma once

#include "Static.h"

namespace Lab5Master
{
	Static::Static(UINT_PTR subClassId, HINSTANCE hInst, HWND parent, int x, int y, int width, int height,
		int id, LPCWSTR text, DWORD additionalStyles) : Control(SubClassId, hInst, parent, x, y, width, height,
		text, WC_STATIC, NULL, WS_CHILD | WS_VISIBLE | SS_CENTER | additionalStyles, HMENU(id))
	{
	}

	Static::~Static()
	{
		DestroyWindow(ThisWindow);
	}

	int Static::Start()
	{
		return Create();
	}

	LRESULT Static::OnCommand(WPARAM wParam, LPARAM lParam)
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