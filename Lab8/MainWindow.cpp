#pragma once

#include "MainWindow.h"

namespace Lab8
{
	MainWindow::MainWindow(HINSTANCE hInst, int nCmdShow, LPCWSTR appTitle, LPCWSTR mainWindowClassName,
		int x, int y, int width, int height) : Window(hInst, nullptr, x, y, width, height, appTitle,
		mainWindowClassName, 0, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN)
	{
		IsMainWindow = true;
		ProcessesListBox = nullptr;
		ThreadsListBox = nullptr;
		ModulesListBox = nullptr;
	}

	MainWindow::~MainWindow()
	{
		ProcessesListBox->~ListBox();
		ThreadsListBox->~ListBox();
		ModulesListBox->~ListBox();
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

		ProcessesListBox = new ListBox(IDSC_LISTBOX, Instance, ThisWindow, 0, 0, 0, 0,
			IDC_PROCESSES_LISTBOX, 0);
		ThreadsListBox = new ListBox(IDSC_LISTBOX, Instance, ThisWindow, 0, 0, 0, 0,
			IDC_THREADS_LISTBOX, 0);
		ModulesListBox = new ListBox(IDSC_LISTBOX, Instance, ThisWindow, 0, 0, 0, 0,
			IDC_MODULES_LISTBOX, 0);

		bool result = true;

		result = result && !ProcessesListBox->Start();
		result = result && !ThreadsListBox->Start();
		result = result && !ModulesListBox->Start();

		if (!result)
		{
			return 3;
		}

		return 0;
	}

	BOOL MainWindow::Show(int nCmdShow)
	{
		return ShowWindow(ThisWindow, nCmdShow);
	}

	LRESULT MainWindow::OnResize(WPARAM wParam, LPARAM lParam)
	{
		Window::OnResize(wParam, lParam);

		MoveWindow(ProcessesListBox->ThisWindow, 10, 10, (Width - 30) / 2, Height - 20, TRUE);
		MoveWindow(ThreadsListBox->ThisWindow, 20 + (Width - 30) / 2, 10, (Width - 30) / 2,
			(Height - 30) / 2, TRUE);
		MoveWindow(ModulesListBox->ThisWindow, 20 + (Width - 30) / 2, 20 + (Height - 30) / 2,
			(Width - 30) / 2, (Height - 30) / 2, TRUE);
		
		return 0;
	}

	LRESULT MainWindow::OnCommand(WPARAM wParam, LPARAM lParam)
	{
		int wmId = LOWORD(wParam); 
		int wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_QUIT:
			OnClose(wParam, lParam);
			break;
		case IDM_REFRESH:
			LoadSnapshot();
			ProcessesListBox->Clear();
			ProcessesListBox->DisplayProcesses(Processes);
			ModulesListBox->Clear();
			ThreadsListBox->Clear();
			break;
		case IDC_PROCESSES_LISTBOX:
			if (wmEvent == LBN_SELCHANGE)
			{
				int index = ListBox_GetCurSel(ProcessesListBox->ThisWindow);
				if (index == LB_ERR)
					break;
				ModulesListBox->Clear();
				ThreadsListBox->Clear();
				ModulesListBox->DisplayModules(Processes[index]->Modules);
				ThreadsListBox->DisplayThreads(Processes[index]->Threads);
				break;
			}
			if (wmEvent == IDN_CONTEXT_MENU)
			{
				ProcessesContextMenu(lParam);
			}
			break;
		case IDC_THREADS_LISTBOX:
			if (wmEvent == IDN_CONTEXT_MENU)
			{
				ThreadsContextMenu(lParam);
			}
			break;
		case IDM_CLASS_HIGH:
		case IDM_CLASS_ABOVENORMAL:
		case IDM_CLASS_BELOWNORMAL:
		case IDM_CLASS_NORMAL:
		case IDM_CLASS_IDLE:
		case IDM_CLASS_REALTIME:
			ChangeProcessClass(wmId);
			break;
		case IDM_LEVEL_ABOVENORMAL:
		case IDM_LEVEL_BELOWNORMAL:
		case IDM_LEVEL_CRITICAL:
		case IDM_LEVEL_HIGHEST:
		case IDM_LEVEL_LOWEST:
		case IDM_LEVEL_NORMAL:
		case IDM_LEVEL_IDLE:
			ChangeThreadPriority(wmId);
			break;
		default:
			return Window::OnCommand(wParam, lParam);
		}
		return 0;
	}

	int MainWindow::LoadSnapshot(DWORD desiredAccess)
	{
		for (int i = 0; i < Processes.size(); i++)
		{
			Processes[i]->~Process();
		}
		Processes.clear();

		LoadThreads();

		HANDLE snapshotHandle;
		PROCESSENTRY32 processData;

		snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (snapshotHandle == INVALID_HANDLE_VALUE)
		{
			return 1;
		}

		processData.dwSize = sizeof(PROCESSENTRY32);

		if (!Process32First(snapshotHandle, &processData))
		{
			CloseHandle(snapshotHandle);
			return 2;
		}

		do
		{
			Process *process = new Process(&processData, desiredAccess);

			process->FindOwnedThreads(Threads);

			process->LoadModules();

			Processes.push_back(process);
		} while (Process32Next(snapshotHandle, &processData));

		CloseHandle(snapshotHandle);
		Threads.clear();
		return 0;
	}

	int MainWindow::LoadThreads(DWORD desiredAccess)
	{
		HANDLE snapshotHandle;
		THREADENTRY32 threadData;

		snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
		if (snapshotHandle == INVALID_HANDLE_VALUE)
		{
			return 3;
		}

		threadData.dwSize = sizeof(THREADENTRY32);

		if (!Thread32First(snapshotHandle, &threadData))
		{
			CloseHandle(snapshotHandle);
			return 4;
		}

		do
		{
			Thread *thread = new Thread(&threadData, desiredAccess);
			Threads.push_back(thread);
		} while (Thread32Next(snapshotHandle, &threadData));

		CloseHandle(snapshotHandle);
		return 0;
	}

	void MainWindow::ChangeProcessClass(int messageId)
	{
		int index = ListBox_GetCurSel(ProcessesListBox->ThisWindow);
		if (index == LB_ERR)
			return;
		if (Processes[index]->Handle == NULL)
			return;
		BOOL res = TRUE;
		switch (messageId)
		{
		case IDM_CLASS_IDLE:
			res = SetPriorityClass(Processes[index]->Handle, IDLE_PRIORITY_CLASS);
			break;
		case IDM_CLASS_BELOWNORMAL:
			res = SetPriorityClass(Processes[index]->Handle, BELOW_NORMAL_PRIORITY_CLASS);
			break;
		case IDM_CLASS_NORMAL:
			res = SetPriorityClass(Processes[index]->Handle, NORMAL_PRIORITY_CLASS);
			break;
		case IDM_CLASS_ABOVENORMAL:
			res = SetPriorityClass(Processes[index]->Handle, ABOVE_NORMAL_PRIORITY_CLASS);
			break;
		case IDM_CLASS_HIGH:
			res = SetPriorityClass(Processes[index]->Handle, HIGH_PRIORITY_CLASS);
			break;
		case IDM_CLASS_REALTIME:
			res = SetPriorityClass(Processes[index]->Handle, REALTIME_PRIORITY_CLASS);
			break;
		}
		if (res == FALSE)
		{
			MessageBox(ThisWindow, L"Looks like Windows doesn't want to do this", L"Ooops", MB_OK);
			return;
		}
		MainWindow::OnCommand(MAKEWPARAM(IDM_REFRESH, 0), 0);
		ListBox_SetCurSel(ProcessesListBox->ThisWindow, index);
		ModulesListBox->Clear();
		ThreadsListBox->Clear();
		ModulesListBox->DisplayModules(Processes[index]->Modules);
		ThreadsListBox->DisplayThreads(Processes[index]->Threads);
	}

	void MainWindow::ChangeThreadPriority(int messageId)
	{
		int threadIndex = ListBox_GetCurSel(ThreadsListBox->ThisWindow);
		int processIndex = ListBox_GetCurSel(ProcessesListBox->ThisWindow);
		if (threadIndex == LB_ERR || processIndex == LB_ERR)
			return;
		if (Processes[processIndex]->Threads[threadIndex]->Handle == NULL ||
			Processes[processIndex]->Handle == NULL)
			return;
		BOOL res = TRUE;
		switch (messageId)
		{
		case IDM_LEVEL_LOWEST:
			res = SetThreadPriority(Processes[processIndex]->Threads[threadIndex]->Handle, THREAD_PRIORITY_LOWEST);
			break;
		case IDM_LEVEL_BELOWNORMAL:
			res = SetThreadPriority(Processes[processIndex]->Threads[threadIndex]->Handle,
				THREAD_PRIORITY_BELOW_NORMAL);
			break;
		case IDM_LEVEL_NORMAL:
			res = SetThreadPriority(Processes[processIndex]->Threads[threadIndex]->Handle, THREAD_PRIORITY_NORMAL);
			break;
		case IDM_LEVEL_ABOVENORMAL:
			res = SetThreadPriority(Processes[processIndex]->Threads[threadIndex]->Handle,
				THREAD_PRIORITY_ABOVE_NORMAL);
			break;
		case IDM_LEVEL_HIGHEST:
			res = SetThreadPriority(Processes[processIndex]->Threads[threadIndex]->Handle, THREAD_PRIORITY_HIGHEST);
			break;
		case IDM_LEVEL_IDLE:
			res = SetThreadPriority(Processes[processIndex]->Threads[threadIndex]->Handle, THREAD_PRIORITY_IDLE);
			break;
		case IDM_LEVEL_CRITICAL:
			res = SetThreadPriority(Processes[processIndex]->Threads[threadIndex]->Handle,
				THREAD_PRIORITY_TIME_CRITICAL);
			break;
		}
		if (res == FALSE)
		{
			MessageBox(ThisWindow, L"Looks like Windows doesn't want to do this", L"Ooops", MB_OK);
			return;
		}
		MainWindow::OnCommand(MAKEWPARAM(IDM_REFRESH, 0), 0);
		processIndex = min(processIndex, Processes.size() - 1);
		threadIndex = min(Processes[processIndex]->Threads.size() - 1, threadIndex);
		ListBox_SetCurSel(ProcessesListBox->ThisWindow, processIndex);
		ModulesListBox->Clear();
		ThreadsListBox->Clear();
		ModulesListBox->DisplayModules(Processes[processIndex]->Modules);
		ThreadsListBox->DisplayThreads(Processes[processIndex]->Threads);
		ListBox_SetCurSel(ThreadsListBox->ThisWindow, threadIndex);
	}

	void MainWindow::ProcessesContextMenu(LPARAM lParam)
	{
		SCROLLINFO scrollInfo;
		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask = SIF_POS;
		GetScrollInfo(ProcessesListBox->ThisWindow, SB_VERT, &scrollInfo);

		int index = HIWORD(lParam) / ListBox_GetItemHeight(ProcessesListBox->ThisWindow, 0) + scrollInfo.nPos;
		if (index >= Processes.size())
			return;

		ListBox_SetCurSel(ProcessesListBox->ThisWindow, index);
		ModulesListBox->Clear();
		ThreadsListBox->Clear();
		ModulesListBox->DisplayModules(Processes[index]->Modules);
		ThreadsListBox->DisplayThreads(Processes[index]->Threads);

		HMENU menu = LoadMenu(Instance, MAKEINTRESOURCE(IDR_PROCESS_MENU));
		HMENU subMenu = GetSubMenu(menu, 0);

		if (Processes[index]->Handle == NULL)
		{
			ModifyMenu(subMenu, IDM_CLASS_IDLE, MF_BYCOMMAND | MF_GRAYED, IDM_CLASS_IDLE, L"Idle");
			ModifyMenu(subMenu, IDM_CLASS_BELOWNORMAL, MF_BYCOMMAND | MF_GRAYED, IDM_CLASS_BELOWNORMAL,
				L"Below Normal");
			ModifyMenu(subMenu, IDM_CLASS_NORMAL, MF_BYCOMMAND | MF_GRAYED, IDM_CLASS_NORMAL, L"Normal");
			ModifyMenu(subMenu, IDM_CLASS_ABOVENORMAL, MF_BYCOMMAND | MF_GRAYED, IDM_CLASS_ABOVENORMAL,
				L"Above Normal");
			ModifyMenu(subMenu, IDM_CLASS_HIGH, MF_BYCOMMAND | MF_GRAYED, IDM_CLASS_HIGH, L"High");
			ModifyMenu(subMenu, IDM_CLASS_REALTIME, MF_BYCOMMAND | MF_GRAYED, IDM_CLASS_REALTIME, L"Real Time");
		}
		else 
		{
			DWORD priorityClass = GetPriorityClass(Processes[index]->Handle);
			switch (priorityClass)
			{
			case IDLE_PRIORITY_CLASS:
				ModifyMenu(subMenu, IDM_CLASS_IDLE, MF_BYCOMMAND | MF_CHECKED, IDM_CLASS_IDLE, L"Idle");
				break;
			case BELOW_NORMAL_PRIORITY_CLASS:
				ModifyMenu(subMenu, IDM_CLASS_BELOWNORMAL, MF_BYCOMMAND | MF_CHECKED, IDM_CLASS_BELOWNORMAL,
					L"Below Normal");
				break;
			case NORMAL_PRIORITY_CLASS:
				ModifyMenu(subMenu, IDM_CLASS_NORMAL, MF_BYCOMMAND | MF_CHECKED, IDM_CLASS_NORMAL, L"Normal");
				break;
			case ABOVE_NORMAL_PRIORITY_CLASS:
				ModifyMenu(subMenu, IDM_CLASS_ABOVENORMAL, MF_BYCOMMAND | MF_CHECKED, IDM_CLASS_ABOVENORMAL,
					L"Above Normal");
				break;
			case HIGH_PRIORITY_CLASS:
				ModifyMenu(subMenu, IDM_CLASS_HIGH, MF_BYCOMMAND | MF_CHECKED, IDM_CLASS_HIGH, L"High");
				break;
			case REALTIME_PRIORITY_CLASS:
				ModifyMenu(subMenu, IDM_CLASS_REALTIME, MF_BYCOMMAND | MF_CHECKED, IDM_CLASS_REALTIME, L"Real Time");
				break;
			}
		}

		POINT point;
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);
		ClientToScreen(ProcessesListBox->ThisWindow, &point);

		TrackPopupMenu(subMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON,
			point.x, point.y, 0, ThisWindow, nullptr);

		DestroyMenu(menu);
	}

	void MainWindow::ThreadsContextMenu(LPARAM lParam)
	{
		SCROLLINFO scrollInfo;
		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask = SIF_POS;
		GetScrollInfo(ThreadsListBox->ThisWindow, SB_VERT, &scrollInfo);

		int threadIndex = HIWORD(lParam) / ListBox_GetItemHeight(ThreadsListBox->ThisWindow, 0) + scrollInfo.nPos;
		ListBox_SetCurSel(ThreadsListBox->ThisWindow, threadIndex);

		int processIndex = ListBox_GetCurSel(ProcessesListBox->ThisWindow);

		if (processIndex == LB_ERR)
			return;

		if (threadIndex >= Processes[processIndex]->Threads.size())
			return;
		
		HMENU menu = LoadMenu(Instance, MAKEINTRESOURCE(IDR_THREAD_MENU));
		HMENU subMenu = GetSubMenu(menu, 0);

		if (Processes[processIndex]->Threads[threadIndex]->Handle == NULL)
		{
			ModifyMenu(subMenu, IDM_LEVEL_LOWEST, MF_BYCOMMAND | MF_GRAYED, IDM_LEVEL_LOWEST, L"Lowest");
			ModifyMenu(subMenu, IDM_LEVEL_BELOWNORMAL, MF_BYCOMMAND | MF_GRAYED, IDM_LEVEL_BELOWNORMAL,
				L"Below Normal");
			ModifyMenu(subMenu, IDM_LEVEL_NORMAL, MF_BYCOMMAND | MF_GRAYED, IDM_LEVEL_NORMAL, L"Normal");
			ModifyMenu(subMenu, IDM_LEVEL_ABOVENORMAL, MF_BYCOMMAND | MF_GRAYED, IDM_LEVEL_ABOVENORMAL,
				L"Above Normal");
			ModifyMenu(subMenu, IDM_LEVEL_HIGHEST, MF_BYCOMMAND | MF_GRAYED, IDM_LEVEL_HIGHEST, L"Highest");
			ModifyMenu(subMenu, IDM_LEVEL_IDLE, MF_BYCOMMAND | MF_GRAYED, IDM_LEVEL_IDLE, L"Idle");
			ModifyMenu(subMenu, IDM_LEVEL_CRITICAL, MF_BYCOMMAND | MF_GRAYED, IDM_LEVEL_CRITICAL, L"Critical");
		}
		else
		{
			DWORD threadPriority = GetThreadPriority(Processes[processIndex]->Threads[threadIndex]->Handle);
			switch (threadPriority)
			{
			case THREAD_PRIORITY_IDLE:
				ModifyMenu(subMenu, IDM_LEVEL_IDLE, MF_BYCOMMAND | MF_CHECKED, IDM_LEVEL_IDLE, L"Idle");
				break;
			case THREAD_PRIORITY_LOWEST:
				ModifyMenu(subMenu, IDM_LEVEL_LOWEST, MF_BYCOMMAND | MF_CHECKED, IDM_LEVEL_LOWEST, L"Lowest");
				break;
			case THREAD_PRIORITY_BELOW_NORMAL:
				ModifyMenu(subMenu, IDM_LEVEL_BELOWNORMAL, MF_BYCOMMAND | MF_CHECKED, IDM_LEVEL_BELOWNORMAL,
					L"Below Normal");
				break;
			case THREAD_PRIORITY_NORMAL:
				ModifyMenu(subMenu, IDM_LEVEL_NORMAL, MF_BYCOMMAND | MF_CHECKED, IDM_LEVEL_NORMAL, L"Normal");
				break;
			case THREAD_PRIORITY_ABOVE_NORMAL:
				ModifyMenu(subMenu, IDM_LEVEL_ABOVENORMAL, MF_BYCOMMAND | MF_CHECKED, IDM_LEVEL_ABOVENORMAL,
					L"Above Normal");
				break;
			case THREAD_PRIORITY_HIGHEST:
				ModifyMenu(subMenu, IDM_LEVEL_HIGHEST, MF_BYCOMMAND | MF_CHECKED, IDM_LEVEL_HIGHEST, L"Highest");
				break;
			case THREAD_PRIORITY_TIME_CRITICAL:
				ModifyMenu(subMenu, IDM_LEVEL_CRITICAL, MF_BYCOMMAND | MF_CHECKED, IDM_LEVEL_CRITICAL, L"Critical");
				break;
			}
		}

		POINT point;
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);
		ClientToScreen(ThreadsListBox->ThisWindow, &point);

		TrackPopupMenu(subMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON,
			point.x, point.y, 0, ThisWindow, nullptr);

		DestroyMenu(menu);
	}
}