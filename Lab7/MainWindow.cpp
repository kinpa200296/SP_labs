#pragma once

#include "MainWindow.h"

namespace Lab7
{
	bool MainWindow::ThreadsNeeded[THREADS_COUNT];
	HWND MainWindow::MainWindowHWND;
	int MainWindow::CurPosX, MainWindow::CurPosY;

#ifdef USE_EVENT
	HANDLE MainWindow::Event;
#else
	CRITICAL_SECTION MainWindow::CriticalSection;
#endif

	MainWindow::MainWindow(HINSTANCE hInst, int nCmdShow, LPCWSTR appTitle, LPCWSTR mainWindowClassName,
		int x, int y, int width, int height) : Window(hInst, nullptr, x, y, width, height, appTitle,
		mainWindowClassName, 0, WS_OVERLAPPEDWINDOW)
	{
		IsMainWindow = true;
		TimerPtr = NULL;

#ifdef USE_EVENT
		WCHAR buffer[MAX_STR];
		LoadString(Instance, IDS_EVENT, buffer, MAX_STR);
		Event = CreateEvent(nullptr, TRUE, TRUE, buffer);
#else
		InitializeCriticalSection(&CriticalSection);
#endif
	}

	MainWindow::~MainWindow()
	{
#ifdef USE_EVENT
		CloseHandle(Event);
#else
		DeleteCriticalSection(&CriticalSection);
#endif
	}

	bool MainWindow::AddWindowMessages()
	{
		bool result = Window::AddWindowMessages();

		result = result && AddMessage(ThisWindow, WM_TIMER, this, ToFuncPointer(&MainWindow::OnTimer));

		return result;
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

		return 0;
	}

	BOOL MainWindow::Show(int nCmdShow)
	{
		return ShowWindow(ThisWindow, nCmdShow);
	}

	LRESULT MainWindow::OnDestroy(WPARAM wParam, LPARAM lParam)
	{
		KillTimer(ThisWindow, IDT_MAIN_TIMER);

		for (int i = 0; i < THREADS_COUNT; i++)
			ThreadsNeeded[i] = false;

		Sleep(WAIT_TIME + 2 * UPDATE_RATE);

		for (int i = 0; i < THREADS_COUNT; i++)
		{
			CloseHandle(ThreadsHandle[i]);
		}

		PostQuitMessage(0);

		return 0;
	}

	LRESULT MainWindow::OnClose(WPARAM wParam, LPARAM lParam)
	{
		return OnDestroy(wParam, lParam);
	}

	LRESULT MainWindow::OnCreate(WPARAM wParam, LPARAM lParam)
	{
		MainWindowHWND = ThisWindow;

		for (int i = 0; i < THREADS_COUNT; i++)
			ThreadsHandle[i] = CreateThread(nullptr, 0, &MainWindow::ThreadProc, LPVOID(i),
			CREATE_SUSPENDED, &ThreadsId[i]);

		TimerPtr = SetTimer(ThisWindow, IDT_MAIN_TIMER, TIMER_ELAPSE, nullptr);

		return 0;
	}

	LRESULT MainWindow::OnResize(WPARAM wParam, LPARAM lParam)
	{
		int temp = CurPosY - Height / 2;

		Window::OnResize(wParam, lParam);

		if (CurPosY == 0 && CurPosX == 0)
		{
			CurPosX = Width - FIGURE_SIZE / 2;
			CurPosY = Height / 2;

			for (int i = 0; i < THREADS_COUNT; i++)
				while (ResumeThread(ThreadsHandle[i]));
		}
		else 
		{
			CurPosY = Height / 2 + temp;
		}
		
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
		default:
			return Window::OnCommand(wParam, lParam);
		}
		return 0;
	}

	LRESULT MainWindow::OnPaint(WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(ThisWindow, &ps);
		Rectangle(hdc, CurPosX - FIGURE_SIZE / 2, CurPosY - FIGURE_SIZE / 2,
			CurPosX + FIGURE_SIZE / 2, CurPosY + FIGURE_SIZE / 2);
		EndPaint(ThisWindow, &ps);
		return 0;
	}

	LRESULT MainWindow::OnTimer(WPARAM wParam, LPARAM lParam)
	{
		if (wParam != IDT_MAIN_TIMER)
			return DefWindowProc(ThisWindow, WM_TIMER, wParam, lParam);

		CurPosX = CurPosX - SPEED > -FIGURE_SIZE / 2 ? CurPosX - SPEED : Width + FIGURE_SIZE / 2;
		CurPosX = CurPosX > Width + FIGURE_SIZE / 2 ? Width + FIGURE_SIZE / 2 : CurPosX;
		
		RECT tmpRect;
		tmpRect.left = 0;
		tmpRect.top = 0;
		tmpRect.bottom = Height;
		tmpRect.right = Width;
		InvalidateRect(ThisWindow, &tmpRect, TRUE);

		return 0;
	}

	DWORD MainWindow::ThreadProc(LPVOID param)
	{
		int threadIndex = int(param);

		ThreadsNeeded[threadIndex] = true;

		while (ThreadsNeeded[threadIndex])
		{
#ifdef USE_EVENT
			DWORD res = WaitForSingleObject(Event, UPDATE_RATE);
			if (res == WAIT_OBJECT_0)
			{
				ResetEvent(Event);
				CurPosY += threadIndex == 0 ? OFFSET : -OFFSET;
				Sleep(WAIT_TIME);
				SetEvent(Event);
				Sleep(UPDATE_RATE);
			}
#else
			EnterCriticalSection(&CriticalSection);
			CurPosY += threadIndex == 0 ? OFFSET : -OFFSET;
			Sleep(WAIT_TIME);
			LeaveCriticalSection(&CriticalSection);
			Sleep(UPDATE_RATE);
#endif
		}
		
		ExitThread(0);

		return 0;
	}
}