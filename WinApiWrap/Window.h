#pragma once

#include "TargetVer.h"
#include "Common.h"

namespace WinApiWrap
{
	typedef LRESULT *FuncPointer(WPARAM, LPARAM);

	#define ToFuncPointer(FuncPtr) reinterpret_cast<FuncPointer>(FuncPtr)

	class Window
	{
	public:
		typedef LRESULT (Window::*FuncPointer)(WPARAM, LPARAM);

		#define ToFuncPointer(FuncPtr) reinterpret_cast<FuncPointer>(FuncPtr)

		struct Pointer
		{
			Window *Window;
			FuncPointer Function;
		};
		typedef map<UINT, Pointer> MessageMap;

		HWND ThisWindow, Parent;
		HINSTANCE Instance;
		int PosX, PosY, Width, Height;
		DWORD ExStyle, Style;
		HMENU Menu;
		WCHAR Title[MAX_STR], ClassName[MAX_STR];
		ATOM AtomIndex;

		static map<HWND, MessageMap> GlobalMessageMap;

		Window(HINSTANCE hInst, HWND parent, int x, int y, int width, int height, LPCWSTR title, 
				LPCWSTR className, DWORD exStyle, DWORD style, HMENU menu = NULL);
		virtual ~Window();

		bool Create();

		virtual ATOM RegisterWindowClass(UINT classStyle, HBRUSH backgroundBrush,
			LPCWSTR cursor = IDC_ARROW, LPCWSTR icon = NULL, LPCWSTR iconSm = NULL, LPCWSTR menu = NULL);

		static bool AddMessage(HWND hwnd, UINT message, Window *window, FuncPointer function);

		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
}
