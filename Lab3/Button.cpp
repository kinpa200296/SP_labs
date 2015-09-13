#pragma once

#include "Button.h"

namespace Lab3
{
	Button::Button(UINT_PTR subClassId, HINSTANCE hInst, HWND parent, int x, int y, int width, int height, int id,
		LPCWSTR text, DWORD additionalStyles) : Control(subClassId, hInst, parent, x, y, width, height, text,
		WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | BS_CENTER | BS_VCENTER | BS_PUSHBUTTON | BS_TEXT | additionalStyles,
		HMENU(id))
	{
		DrawButtonDC = CreateCompatibleDC(nullptr);
		ClearButtonDC = CreateCompatibleDC(nullptr);
		HBITMAP bitmap = HBITMAP(LoadImage(Instance, MAKEINTRESOURCE(IDB_DRAW), IMAGE_BITMAP,
			PIC_SIZE, PIC_SIZE, LR_DEFAULTCOLOR));
		SelectObject(DrawButtonDC, bitmap);
		bitmap = HBITMAP(LoadImage(Instance, MAKEINTRESOURCE(IDB_CLEAR), IMAGE_BITMAP,
			PIC_SIZE, PIC_SIZE, LR_DEFAULTCOLOR));
		SelectObject(ClearButtonDC, bitmap);
	}

	Button::~Button()
	{
		DeleteDC(DrawButtonDC);
		DeleteDC(ClearButtonDC);
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

	LRESULT Button::OwnerDrawProc(WPARAM wParam, LPARAM lParam)
	{
		LPDRAWITEMSTRUCT pdis = LPDRAWITEMSTRUCT(lParam);

		int offsetY = (Height - PIC_SIZE) / 2;
		int offsetX = offsetY + 5;
		SIZE textSize;
		GetTextExtentPoint(pdis->hDC, Title, lstrlen(Title), &textSize);
		RECT rect = pdis->rcItem;
		rect.left = (Width - textSize.cx - PIC_SIZE - 2 * offsetX) / 2;
		rect.top = offsetY;
		HDC hdc = pdis->hDC;
		FrameRect(pdis->hDC, &pdis->rcItem, HBRUSH(GetStockObject(BLACK_BRUSH)));
		if (wParam == IDC_DRAW_BUTTON)
			BitBlt(pdis->hDC, rect.left + offsetX, offsetY, PIC_SIZE, PIC_SIZE, DrawButtonDC, 0, 0, SRCCOPY);
		else BitBlt(pdis->hDC, rect.left + offsetX, offsetY, PIC_SIZE, PIC_SIZE, ClearButtonDC, 0, 0, SRCCOPY);
		rect.left += PIC_SIZE + 2 * offsetX;
		DrawText(hdc, Title, lstrlen(Title), &rect, 0);

		return TRUE;
	}
}