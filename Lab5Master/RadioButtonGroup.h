#pragma once

#include "Lab5Master.h"
#include "Static.h"
#include "Button.h"

namespace Lab5Master
{
	class RadioButtonGroup : public Window
	{
	public:
		RadioButtonGroup(HINSTANCE hInst, HWND parent, int x, int y, int width, int height, int id,
			LPCWSTR title, DWORD additionalStyles, LPCWSTR *buttonsText, int buttonsCount);
		~RadioButtonGroup();

		int Id, CheckedButton, RadioButtonsCount;
		LPWSTR RadioButtonsText[MAX_RADIOBUTTONS];
		Static *Header;
		Button *RadioButtons[MAX_RADIOBUTTONS];

		int Start();

		LRESULT OnResize(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnPaint(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnCommand(WPARAM wParam, LPARAM lParam) override;
	};
}