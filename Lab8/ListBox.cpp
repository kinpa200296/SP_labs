#pragma once

#include "ListBox.h"

namespace Lab8
{
	ListBox::ListBox(UINT_PTR subClassId, HINSTANCE hInst, HWND parent, int x, int y, int width, int height, int id,
		DWORD additionalStyles) : Control(subClassId, hInst, parent, x, y, width, height, L"", WC_LISTBOX, NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL | LBS_HASSTRINGS | LBS_NOTIFY
		| additionalStyles, HMENU(id))
	{
	}

	ListBox::~ListBox()
	{
		DestroyWindow(ThisWindow);
	}

	bool ListBox::AddWindowMessages()
	{
		bool result = Control::AddWindowMessages();

		result = result && AddMessage(ThisWindow, WM_RBUTTONDOWN, this,
			ToFuncPointer(&ListBox::OnRightButtonDown));

		return result;
	}

	int ListBox::Start()
	{
		return Create();
	}

	int ListBox::AddString(LPCWSTR str)
	{
		return ListBox_AddString(ThisWindow, str);
	}

	int ListBox::DeleteString(int index)
	{
		return ListBox_DeleteString(ThisWindow, index);
	}

	int ListBox::GetString(int index, LPWSTR buffer)
	{
		return ListBox_GetText(ThisWindow, index, buffer);
	}

	void ListBox::Clear()
	{
		for (int count = 1; count > 0 && count != LB_ERR; count = ListBox_DeleteString(ThisWindow, 0));
	}

	LRESULT ListBox::OnCommand(WPARAM wParam, LPARAM lParam)
	{
		int wmId = LOWORD(wParam);
		//int wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_QUIT:
		case IDM_REFRESH:
		case IDM_CLASS_HIGH:
		case IDM_CLASS_BELOWNORMAL:
		case IDM_CLASS_ABOVENORMAL:
		case IDM_CLASS_NORMAL:
		case IDM_CLASS_IDLE:
		case IDM_CLASS_REALTIME:
		case IDM_LEVEL_ABOVENORMAL:
		case IDM_LEVEL_BELOWNORMAL:
		case IDM_LEVEL_CRITICAL:
		case IDM_LEVEL_HIGHEST:
		case IDM_LEVEL_LOWEST:
		case IDM_LEVEL_NORMAL:
		case IDM_LEVEL_IDLE:
			SendMessage(Parent, WM_COMMAND, wParam, lParam);
			break;
		default:
			return Control::OnCommand(wParam, lParam);
		}
		return 0;
	}

	LRESULT ListBox::OnRightButtonDown(WPARAM wParam, LPARAM lParam)
	{
		SendMessage(Parent, WM_COMMAND, MAKEWPARAM(Id, IDN_CONTEXT_MENU), lParam);

		return 0;
	}

	void ListBox::DisplayProcesses(vector<Process*>& processes)
	{
		for (int i = 0; i < processes.size(); i++)
			if (processes[i]->Handle != NULL)
				ListBox_AddString(ThisWindow, processes[i]->ExeFileName);
			else
			{
				WCHAR buffer[MAX_STR];
				swprintf(buffer, MAX_STR, L"%s [Access denied]", processes[i]->ExeFileName);

				ListBox_AddString(ThisWindow, buffer);
			}
		UpdateWindow(ThisWindow);
	}

	void ListBox::DisplayModules(vector<Module*>& modules)
	{
		for (int i = 0; i < modules.size(); i++)
			ListBox_AddString(ThisWindow, modules[i]->Name);
		if (modules.size() == 0)
			ListBox_AddString(ThisWindow, L"Access denied.");
		UpdateWindow(ThisWindow);
	}

	void ListBox::DisplayThreads(vector<Thread*>& threads)
	{
		for (int i = 0; i < threads.size(); i++)
		{
			WCHAR buffer[MAX_STR];
			if (threads[i]->Handle != NULL)
				swprintf(buffer, MAX_STR, L"Id: %d", threads[i]->Id);
			else swprintf(buffer, MAX_STR, L"Id: %d [Access denied]", threads[i]->Id);
			
			ListBox_AddString(ThisWindow, buffer);
		}
		UpdateWindow(ThisWindow);
	}
}