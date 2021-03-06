#pragma once

#include "ListBox.h"

namespace Lab2
{
	ListBox::ListBox(UINT_PTR subClassId, HINSTANCE hInst, HWND parent, int x, int y, int width, int height, int id,
		DWORD additionalStyles) : Control(subClassId, hInst, parent, x, y, width, height, L"", WC_LISTBOX, NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL | LBS_HASSTRINGS | additionalStyles, HMENU(id))
	{
	}

	ListBox::~ListBox()
	{
		DestroyWindow(ThisWindow);
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
		ClearSelection();
		for (int count = 1; count > 0 && count != LB_ERR; count = ListBox_DeleteString(ThisWindow, 0));
	}

	void ListBox::ClearSelection()
	{
		vector<int> selection = GetSelectedItemsIndex();
		for (unsigned int i = 0; i < selection.size(); i++)
			ListBox_SetSel(ThisWindow, FALSE, selection[i]);
	}

	bool ListBox::AddStringUnique(LPCWSTR str)
	{
		int index = ListBox_FindStringExact(ThisWindow, -1, str);

		if (index != LB_ERR)
		{
			WCHAR buffer[MAX_STR];
			GetString(index, buffer);
			if (!lstrcmp(buffer, str))
				return false;
		}

		if (index == LB_ERR && !lstrlen(str))
			return false;

		AddString(str);
		return true;
	}

	vector<int> ListBox::GetSelectedItemsIndex()
	{
		int count = ListBox_GetSelCount(ThisWindow);
		vector<int> res(count);
		if (count != 0)
			ListBox_GetSelItems(ThisWindow, count, &res[0]);
		return res;
	}

	LRESULT ListBox::OnCommand(WPARAM wParam, LPARAM lParam)
	{
		int wmId = LOWORD(wParam);
		//int wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_QUIT:
		case IDM_ADD:
		case IDM_TORIGHT:
		case IDM_DELETE:
		case IDM_CLEAR:
		case IDM_CLEAR_SEL_LEFT:
		case IDM_CLEAR_SEL_RIGHT:
			SendMessage(Parent, WM_COMMAND, wParam, lParam);
			break;
		default:
			return Control::OnCommand(wParam, lParam);
		}
		return 0;
	}
}