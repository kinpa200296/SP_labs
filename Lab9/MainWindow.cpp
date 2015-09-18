#pragma once

#include "MainWindow.h"

namespace Lab9
{
	bool MainWindow::SearchAgain, MainWindow::ThreadNeeded;

	MainWindow::MainWindow(HINSTANCE hInst, int nCmdShow, LPCWSTR appTitle, LPCWSTR mainWindowClassName,
		int x, int y, int width, int height) : Window(hInst, nullptr, x, y, width, height, appTitle,
		mainWindowClassName, 0, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN)
	{
		IsMainWindow = true;
		MainListView = nullptr;
		ThreadHandle = INVALID_HANDLE_VALUE;
		ThreadId = -1;
	}

	MainWindow::~MainWindow()
	{
	}

	int MainWindow::Start()
	{
		RegisterWindowClass(CS_HREDRAW | CS_VREDRAW, HBRUSH(COLOR_WINDOW), IDC_ARROW,
			MAKEINTRESOURCE(IDI_ICON), MAKEINTRESOURCE(IDI_ICON_SMALL), MAKEINTRESOURCE(IDR_MENU));

		int res = Create();

		if (res)
		{
			return res;
		}

		MainListView = new ListView(IDSC_LISTVIEW, Instance, ThisWindow, 0, 0, 0, 0, IDC_MAIN_LISTVIEW);

		bool result = true;

		result = result && !MainListView->Start();

		if (!result)
			return 3;

		return 0;
	}

	BOOL MainWindow::Show(int nCmdShow)
	{
		return ShowWindow(ThisWindow, nCmdShow);
	}

	LRESULT MainWindow::OnDestroy(WPARAM wParam, LPARAM lParam)
	{
		ThreadNeeded = false;
		Sleep(DELAY);
		CloseHandle(ThreadHandle);
		PostQuitMessage(0);
		return 0;
	}

	LRESULT MainWindow::OnClose(WPARAM wParam, LPARAM lParam)
	{
		return OnDestroy(wParam, lParam);
	}

	LRESULT MainWindow::OnCreate(WPARAM wParam, LPARAM lParam)
	{
		MainListView->AddColumn(L"Key", 0, 0);
		MainListView->AddColumn(L"Name", 1, 0);
		MainListView->AddColumn(L"Value", 2, 0);

		ThreadHandle = CreateThread(NULL, 0, &MainWindow::ThreadProc, LPVOID(MainListView),
			0, &ThreadId);

		return 0;
	}

	LRESULT MainWindow::OnResize(WPARAM wParam, LPARAM lParam)
	{
		Window::OnResize(wParam, lParam);

		MoveWindow(MainListView->ThisWindow, 10, 10, Width - 20, Height - 20, TRUE);
		ListView_SetColumnWidth(MainListView->ThisWindow, 0, 4 * (Width - 20) / 10);
		ListView_SetColumnWidth(MainListView->ThisWindow, 1, 2 * (Width - 20) / 10);
		ListView_SetColumnWidth(MainListView->ThisWindow, 2, 4 * (Width - 20) / 10);

		return 0;
	}

	LRESULT MainWindow::OnCommand(WPARAM wParam, LPARAM lParam)
	{
		int wmId = LOWORD(wParam); 
		//int wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_QUIT:
			OnClose(wParam, lParam);
			break;
		case IDM_RESTART:
			SearchAgain = true;
			break;
		default:
			return Window::OnCommand(wParam, lParam);
		}
		return 0;
	}

	DWORD MainWindow::ThreadProc(LPVOID param)
	{
		ThreadNeeded = true;
		SearchAgain = true;
		ListView *listView = (ListView*)param;

		while (ThreadNeeded)
		{
			if (SearchAgain)
			{
				listView->Clear();
				SearchAgain = false;
				SearchRegistry(HKEY_CURRENT_USER, L"", listView);
			}
		}

		ExitThread(0);

		return 0;
	}

	void MainWindow::SearchRegistry(HKEY root, LPWSTR subkey, ListView *listView)
	{
		HKEY key;
		int resultCode = RegOpenKeyEx(root, subkey, 0, KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS, &key);
		if (resultCode != 0)
			return;

		int index = 0;
		WCHAR name[10 * MAX_STR];
		while (RegEnumKey(key, index++, name, 10240) != ERROR_NO_MORE_ITEMS && !SearchAgain && ThreadNeeded)
		{
			if (lstrlen(subkey) == 0)
				SearchRegistry(root, name, listView);
			else
			{
				WCHAR path[10 * MAX_STR];
				swprintf(path, 10 * MAX_STR, L"%s\\%s", subkey, name);
				SearchRegistry(root, path, listView);
			}
		}

		index = 0;
		BYTE data[20 * MAX_STR];
		DWORD namesize = 10 * MAX_STR, type, size = 20 * MAX_STR;
		while (RegEnumValue(key, index++, name, &namesize, NULL, &type, data, &size) != ERROR_NO_MORE_ITEMS
			&& !SearchAgain && ThreadNeeded)
		{
			if (type == REG_SZ)
			{
				LPWSTR path = LPWSTR(data);
				WCHAR tmp[MAX_STR];
				lstrcpy(tmp, L":\\");
				if (lstrlen(path) != 0 && tmp[0] == path[1] && tmp[1] == path[2] && PathFileExists(path) == FALSE)
				{
					int newRow = listView->AddItem(subkey, -1, 0);
					listView->AddItem(name, newRow, 1);
					listView->AddItem(path, newRow, 2);
				}
			}
			namesize = size = 1024;
		}
		RegCloseKey(key);
	}
}