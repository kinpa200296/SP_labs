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
		ThisWindow = NULL;
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

	bool Window::Create()
	{
		ThisWindow = CreateWindowEx(ExStyle, ClassName, Title, Style, PosX, PosY, Width, Height,
			Parent, Menu, Instance, NULL);

		if (!ThisWindow)
		{
			return false;
		}

		return true;
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
}
