#pragma once

#include "Window.h"

#define IDSC_BUTTON 0
#define IDSC_EDIT 1
#define IDSC_LISTBOX 2
#define IDSC_STATIC 3

namespace WinApiWrap
{
	class Control : public Window
	{
	public:
		UINT_PTR SubClassId;
		int Id;

		Control(UINT_PTR subClassId, HINSTANCE hInst, HWND parent, int x, int y, int width, int height, LPCWSTR title,
			LPCWSTR className, DWORD exStyle, DWORD style, HMENU menu = nullptr);
		~Control();

		virtual BOOL SetSubClass();
		bool AddWindowMessages() override;

		static LRESULT CALLBACK ControlProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam,
			UINT_PTR subClassId, DWORD_PTR dataReference);

		LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
		LRESULT OnClose(WPARAM wParam, LPARAM lParam);
		LRESULT OnResize(WPARAM wParam, LPARAM lParam);
		LRESULT OnMove(WPARAM wParam, LPARAM lParam);
		LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
		LRESULT OnCommand(WPARAM wParam, LPARAM lParam);
		LRESULT OnPaint(WPARAM wParam, LPARAM lParam);
	};
}
