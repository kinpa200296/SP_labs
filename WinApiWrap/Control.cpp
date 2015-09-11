#pragma once

#include "Control.h"

namespace WinApiWrap
{
	Control::Control(UINT_PTR subClassId, HINSTANCE hInst, HWND parent, int x, int y, int width, int height,
		LPCWSTR title, LPCWSTR className, DWORD exStyle, DWORD style, HMENU menu) : Window(hInst, parent, x, y,
		width, height, title, className, exStyle, style, menu)
	{
		SubClassId = subClassId;
	}

	Control::~Control()
	{
	}
	
	BOOL Control::SetSubClass()
	{
		return SetWindowSubclass(ThisWindow, ControlProc, SubClassId, 0);
	}
	
	bool Control::AddWindowMessages()
	{
		if (SetSubClass() == TRUE)
			return Window::AddWindowMessages();
		return false;
	}

	LRESULT Control::ControlProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam,
		UINT_PTR subClassId, DWORD_PTR dataReference)
	{
		Pointer ptr;
		MessageMap msgMap;

		if (!ProcessMessages)
		{
			MSG msg;
			msg.hwnd = hwnd;
			msg.message = message;
			msg.wParam = wParam;
			msg.lParam = lParam;
			QueuedMessages.push_back(msg);
			return DefSubclassProc(hwnd, message, wParam, lParam);
		}

		map<HWND, MessageMap>::iterator iterGlobal = GlobalMessageMap.find(hwnd);

		if (iterGlobal == GlobalMessageMap.end())
			return DefSubclassProc(hwnd, message, wParam, lParam);

		msgMap = iterGlobal->second;

		MessageMap::iterator iter = msgMap.find(message);

		if (iter == msgMap.end())
			return DefSubclassProc(hwnd, message, wParam, lParam);

		ptr = iter->second;
		return (ptr.Window->*ptr.Function)(wParam, lParam);
	}

	LRESULT Control::OnDestroy(WPARAM wParam, LPARAM lParam)
	{
		if (IsMainWindow)
		{
			PostQuitMessage(0);
			return 0;
		}
		return DefSubclassProc(ThisWindow, WM_DESTROY, wParam, lParam);
	}

	LRESULT Control::OnClose(WPARAM wParam, LPARAM lParam)
	{
		if (IsMainWindow)
		{
			PostQuitMessage(0);
			return 0;
		}
		return DefSubclassProc(ThisWindow, WM_CLOSE, wParam, lParam);
	}

	LRESULT Control::OnResize(WPARAM wParam, LPARAM lParam)
	{
		Width = LOWORD(lParam);
		Height = HIWORD(lParam);
		return DefSubclassProc(ThisWindow, WM_SIZE, wParam, lParam);
	}

	LRESULT Control::OnMove(WPARAM wParam, LPARAM lParam)
	{
		PosX = LOWORD(lParam);
		PosY = HIWORD(lParam);
		return DefSubclassProc(ThisWindow, WM_MOVE, wParam, lParam);
	}

	LRESULT Control::OnCreate(WPARAM wParam, LPARAM lParam)
	{
		return DefSubclassProc(ThisWindow, WM_CREATE, wParam, lParam);
	}

	LRESULT Control::OnCommand(WPARAM wParam, LPARAM lParam)
	{
		return DefSubclassProc(ThisWindow, WM_COMMAND, wParam, lParam);
	}

	LRESULT Control::OnPaint(WPARAM wParam, LPARAM lParam)
	{
		return DefSubclassProc(ThisWindow, WM_PAINT, wParam, lParam);
	}
}