#pragma once

#include "RadioButtonGroup.h"

namespace Lab5Master
{
	RadioButtonGroup::RadioButtonGroup(HINSTANCE hInst, HWND parent, int x, int y,
		int width, int height, int id, LPCWSTR title, DWORD additionalStyles, LPCWSTR *buttonsText,
		int buttonsCount) : Window(hInst, parent, x, y, width, height, title, L"RadioButtonGroup",
		NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | additionalStyles)
	{
		Header = nullptr;
		Id = id;
		CheckedButton = 0;
		RadioButtonsCount = buttonsCount > MAX_RADIOBUTTONS ? MAX_RADIOBUTTONS : buttonsCount;
		for (int i = 0; i < RadioButtonsCount; i++)
		{
			RadioButtonsText[i] = LPWSTR(malloc(sizeof(WCHAR)*lstrlen(buttonsText[i])));
			lstrcpy(RadioButtonsText[i], buttonsText[i]);
		}
	}

	RadioButtonGroup::~RadioButtonGroup()
	{
		Header->~Static();

		DestroyWindow(ThisWindow);
	}

	int RadioButtonGroup::Start()
	{
		RegisterWindowClass(CS_HREDRAW | CS_VREDRAW, HBRUSH(COLOR_WINDOW));

		int res = Create();

		if (res)
		{
			return res;
		}

		Header = new Static(IDSC_STATIC, Instance, ThisWindow, 0, 0, 0, 0, Id + 1, Title);
		for (int i = 0; i < RadioButtonsCount; i++)
			RadioButtons[i] = new Button(IDSC_BUTTON, Instance, ThisWindow, 0, 0, 200, 20, Id + 2 + i,
			RadioButtonsText[i], BS_RADIOBUTTON);

		bool result = true;

		result = result && !Header->Start();
		for (int i = 0; i < RadioButtonsCount; i++)
			result = result && !RadioButtons[i]->Start();

		if (!result)
			return 3;

		CheckedButton = 1;
		Button_SetCheck(RadioButtons[CheckedButton - 1]->ThisWindow, TRUE);

		return 0;
	}

	LRESULT RadioButtonGroup::OnResize(WPARAM wParam, LPARAM lParam)
	{
		Window::OnResize(wParam, lParam);

		HDC hdc = GetWindowDC(Header->ThisWindow);
		SIZE textSize;
		GetTextExtentPoint(hdc, Header->Title, lstrlen(Header->Title), &textSize);

		MoveWindow(Header->ThisWindow, 5, 0, textSize.cx + 5, textSize.cy, TRUE);
		for (int i = 0; i < RadioButtonsCount; i++)
			MoveWindow(RadioButtons[i]->ThisWindow, 10, textSize.cy + 10 + 30 * i, Width - 20, 20, TRUE);

		return 0;
	}

	LRESULT RadioButtonGroup::OnPaint(WPARAM wParam, LPARAM lParam)
	{
		HDC hdc = GetWindowDC(Header->ThisWindow);
		SIZE textSize;
		GetTextExtentPoint(hdc, Header->Title, lstrlen(Header->Title), &textSize);

		PAINTSTRUCT ps;
		hdc = BeginPaint(ThisWindow, &ps);
		HBRUSH frameBrush = CreateSolidBrush(RGB(192, 192, 192)), oldDCBrush;
		oldDCBrush = HBRUSH(SelectObject(hdc, frameBrush));

		RECT tmpRect = ps.rcPaint;
		tmpRect.top = textSize.cy / 2;
		FrameRect(hdc, &tmpRect, frameBrush);

		SelectObject(hdc, oldDCBrush);
		DeleteObject(frameBrush);
		EndPaint(ThisWindow, &ps);
		return 0;
	}

	LRESULT RadioButtonGroup::OnCommand(WPARAM wParam, LPARAM lParam)
	{
		int wmId = LOWORD(wParam);
		//int wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_QUIT:
			SendMessage(Parent, WM_COMMAND, wParam, lParam);
			break;
		default:
			if (wmId > Id + 1 && wmId <= Id + MAX_RADIOBUTTONS + 1)
			{
				Button_SetCheck(RadioButtons[CheckedButton - 1]->ThisWindow, FALSE);
				CheckedButton = wmId - Id - 1;
				Button_SetCheck(RadioButtons[CheckedButton - 1]->ThisWindow, TRUE);
			}
			else return Window::OnCommand(wParam, lParam);
		}
		return 0;
	}
}