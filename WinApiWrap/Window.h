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
		bool IsMainWindow;
		DWORD ExStyle, Style;
		HMENU Menu;
		WCHAR Title[MAX_STR], ClassName[MAX_STR];
		ATOM AtomIndex;

		static map<HWND, MessageMap> GlobalMessageMap;
		static bool ProcessMessages;
		static vector<MSG> QueuedMessages;

		Window(HINSTANCE hInst, HWND parent, int x, int y, int width, int height, LPCWSTR title, 
				LPCWSTR className, DWORD exStyle, DWORD style, HMENU menu = nullptr);
		virtual ~Window();

		int Create();

		virtual ATOM RegisterWindowClass(UINT classStyle, HBRUSH backgroundBrush,
			LPCWSTR cursor = IDC_ARROW, LPCWSTR icon = nullptr, LPCWSTR iconSm = nullptr, LPCWSTR menu = nullptr);

		virtual bool AddWindowMessages();

		virtual LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnClose(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnResize(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnMove(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnPaint(WPARAM wParam, LPARAM lParam);

		static bool AddMessage(HWND hwnd, UINT message, Window *window, FuncPointer function);

		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

		static void ResentQueuedMessages();
	};
}
