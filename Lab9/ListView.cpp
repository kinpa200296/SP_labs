#pragma once

#include "ListView.h"

namespace Lab9
{
	ListView::ListView(UINT_PTR subClassId, HINSTANCE hInst, HWND parent, int x, int y, int width, int height,
		int id, DWORD additionalStyles) : Control(subClassId, hInst, parent, x, y, width, height, L"", WC_LISTVIEW,
		NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL | LVS_REPORT | LVS_EDITLABELS |
		LVS_NOSORTHEADER | additionalStyles, HMENU(id))
	{
	}

	ListView::~ListView()
	{
		DestroyWindow(ThisWindow);
	}

	int ListView::Start()
	{
		return Create();
	}

	void ListView::Clear()
	{
		ListView_DeleteAllItems(ThisWindow);
	}

	int ListView::AddColumn(LPWSTR text, int index, int width)
	{
		LVCOLUMN lvc;
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.iSubItem = index;
		lvc.pszText = text;
		lvc.cx = width;
		lvc.fmt = LVCFMT_LEFT;
		return ListView_InsertColumn(ThisWindow, index, &lvc);
	}

	int ListView::AddItem(LPWSTR text, int row, int column)
	{
		int nextAvailableRow = ListView_GetItemCount(ThisWindow);

		if (row > nextAvailableRow || row < 0)
		{
			row = nextAvailableRow;
			LV_ITEM lvi;
			memset(&lvi, 0, sizeof(LV_ITEM));
			lvi.mask = LVIF_TEXT;
			lvi.iItem = row;
			ListView_InsertItem(ThisWindow, &lvi);
		}
		ListView_SetItemText(ThisWindow, row, column, text);
		return row;
	}

	LRESULT ListView::OnCommand(WPARAM wParam, LPARAM lParam)
	{
		int wmId = LOWORD(wParam);
		//int wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_QUIT:
		case IDM_RESTART:
			SendMessage(Parent, WM_COMMAND, wParam, lParam);
			break;
		default:
			return Control::OnCommand(wParam, lParam);
		}
		return 0;
	}
}