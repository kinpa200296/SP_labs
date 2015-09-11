#pragma once

#include "Window.h"

namespace WinApiWrap
{
	map<HWND, Window::MessageMap> Window::GlobalMessageMap;
	bool Window::ProcessMessages;
	vector<MSG> Window::QueuedMessages;

	Window::Window(HINSTANCE hInst, HWND parent, int x, int y, int width, int height, LPCWSTR title,
		LPCWSTR className, DWORD exStyle, DWORD style, HMENU menu)
	{
		Instance = hInst;
		Parent = parent;
		ThisWindow = nullptr;
		AtomIndex = 0;
		PosX = x;
		PosY = y;
		Width = width;
		Height = height;
		lstrcpy(Title, title);
		lstrcpy(ClassName, className);
		ExStyle = exStyle;
		Style = style;
		Menu = menu;
		IsMainWindow = false;
	}

	Window::~Window()
	{
		DestroyWindow(ThisWindow);
	}

	ATOM Window::RegisterWindowClass(UINT classStyle, HBRUSH backgroundBrush,
		LPCWSTR cursor, LPCWSTR icon, LPCWSTR iconSm, LPCWSTR menu)
	{
		WNDCLASSEX windowClass;

		memset(&windowClass, 0, sizeof(WNDCLASSEX));

		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.lpszClassName = ClassName;
		windowClass.lpfnWndProc = WindowProc;
		windowClass.hInstance = Instance;
		windowClass.style = classStyle;
		windowClass.hbrBackground = backgroundBrush;
		windowClass.hCursor = LoadCursor(Instance, cursor);
		windowClass.hIcon = LoadIcon(Instance, icon);
		windowClass.hIconSm = LoadIcon(Instance, iconSm);
		windowClass.lpszMenuName = menu;

		AtomIndex = RegisterClassEx(&windowClass);

		return AtomIndex;
	}

	int Window::Create()
	{
		ThisWindow = CreateWindowEx(ExStyle, ClassName, Title, Style, PosX, PosY, Width, Height,
			Parent, Menu, Instance, nullptr);

		if (!ThisWindow)
		{
			return 1;
		}

		if (!AddWindowMessages())
		{
			return 2;
		}

		return 0;
	}

	bool Window::AddWindowMessages()
	{
		bool result = true;

		result = result && AddMessage(ThisWindow, WM_DESTROY, this, ToFuncPointer(&Window::OnDestroy));
		result = result && AddMessage(ThisWindow, WM_CLOSE, this, ToFuncPointer(&Window::OnClose));
		result = result && AddMessage(ThisWindow, WM_SIZE, this, ToFuncPointer(&Window::OnResize));
		result = result && AddMessage(ThisWindow, WM_MOVE, this, ToFuncPointer(&Window::OnMove));
		result = result && AddMessage(ThisWindow, WM_CREATE, this, ToFuncPointer(&Window::OnCreate));
		result = result && AddMessage(ThisWindow, WM_COMMAND, this, ToFuncPointer(&Window::OnCommand));
		result = result && AddMessage(ThisWindow, WM_PAINT, this, ToFuncPointer(&Window::OnPaint));

		return result;
	}

	bool Window::AddMessage(HWND hwnd, UINT message, Window* window, FuncPointer function)
	{
		if (!window || !function)
		{
			return false;
		}

		Pointer ptr;
		MessageMap msgMap;
		ptr.Window = window;
		ptr.Function = function;

		map<HWND, MessageMap>::iterator iter = GlobalMessageMap.find(hwnd);

		if (iter == GlobalMessageMap.end())
		{
			msgMap.clear();
			msgMap.insert(pair<UINT, Pointer>(message, ptr));
			GlobalMessageMap.insert(pair<HWND, MessageMap>(hwnd, msgMap));
		}
		else
		{
			iter->second.insert(pair<UINT, Pointer>(message, ptr));
		}

		return true;
	}

	LRESULT Window::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			return DefWindowProc(hwnd, message, wParam, lParam);
		}

		map<HWND, MessageMap>::iterator iterGlobal = GlobalMessageMap.find(hwnd);

		if (iterGlobal == GlobalMessageMap.end())
			return DefWindowProc(hwnd, message, wParam, lParam);

		msgMap = iterGlobal->second;

		MessageMap::iterator iter = msgMap.find(message);

		if (iter == msgMap.end())
			return DefWindowProc(hwnd, message, wParam, lParam);

		ptr = iter->second;
		return (ptr.Window->*ptr.Function)(wParam, lParam);
	}

	void Window::ResentQueuedMessages()
	{
		for (vector<MSG>::iterator msg = QueuedMessages.begin(); msg < QueuedMessages.end(); ++msg)
		{
			SendMessage(msg->hwnd, msg->message, msg->wParam, msg->lParam);
		}
	}

	LRESULT Window::OnDestroy(WPARAM wParam, LPARAM lParam)
	{
		if (IsMainWindow)
		{
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(ThisWindow, WM_CREATE, wParam, lParam);
	}

	LRESULT Window::OnClose(WPARAM wParam, LPARAM lParam)
	{
		if (IsMainWindow)
		{
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(ThisWindow, WM_CREATE, wParam, lParam);
	}

	LRESULT Window::OnResize(WPARAM wParam, LPARAM lParam)
	{
		Width = LOWORD(lParam);
		Height = HIWORD(lParam);
		return 0;
	}

	LRESULT Window::OnMove(WPARAM wParam, LPARAM lParam)
	{
		PosX = LOWORD(lParam);
		PosY = HIWORD(lParam);
		return 0;
	}

	LRESULT Window::OnCreate(WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(ThisWindow, WM_CREATE, wParam, lParam);
	}

	LRESULT Window::OnCommand(WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(ThisWindow, WM_COMMAND, wParam, lParam);
	}

	LRESULT Window::OnPaint(WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(ThisWindow, WM_PAINT, wParam, lParam);
	}
}
