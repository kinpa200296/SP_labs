#pragma once

#include "MainWindow.h"
#include <ctime>

namespace Lab6
{
	bool MainWindow::ThreadsNeeded[THREADS_COUNT];
	HWND MainWindow::MainWindowHWND;
	POINT MainWindow::Points[THREADS_COUNT];
	int MainWindow::ThreadsSpeed[THREADS_COUNT], MainWindow::StarSize, MainWindow::MainWindowHeight;


	MainWindow::MainWindow(HINSTANCE hInst, int nCmdShow, LPCWSTR appTitle, LPCWSTR mainWindowClassName,
		int x, int y, int width, int height) : Window(hInst, nullptr, x, y, width, height, appTitle,
		mainWindowClassName, 0, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN)
	{
		IsMainWindow = true;
		RunButton = nullptr;
		PauseButton = nullptr;
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

		RunButton = new Button(IDSC_BUTTON, Instance, ThisWindow, 0, 0, 0, 0, IDC_RUN_BUTTON, L"Run", NULL);
		PauseButton = new Button(IDSC_BUTTON, Instance, ThisWindow, 0, 0, 0, 0, IDC_PAUSE_BUTTON, L"Pause", NULL);

		bool result = true;

		result = result && !RunButton->Start();
		result = result && !PauseButton->Start();

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

	LRESULT MainWindow::OnDestroy(WPARAM wParam, LPARAM lParam)
	{
		for (int i = 0; i < THREADS_COUNT; i++)
			ThreadsNeeded[i] = false;

		Sleep(4 * UPDATE_RATE);

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
			ThreadsHandle[i] = CreateThread(NULL, 0, &MainWindow::ThreadProc, LPVOID(i),
				CREATE_SUSPENDED, &ThreadsId[i]);

		return 0;
	}

	LRESULT MainWindow::OnResize(WPARAM wParam, LPARAM lParam)
	{
		Window::OnResize(wParam, lParam);

		MainWindowHeight = Height;

		for (int i = 0; i < THREADS_COUNT; i++)
		{
			int temp = Width / THREADS_COUNT;
			Points[i].x = temp / 2 + temp * i;
			StarSize = temp / 2 - 10;
		}

		MoveWindow(RunButton->ThisWindow, 10, Height - 30, (Width - 30) / 2, 20, TRUE);
		MoveWindow(PauseButton->ThisWindow, 20 + (Width - 30) / 2, Height - 30, (Width - 30) / 2, 20, TRUE);

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
		case IDC_RUN_BUTTON:
		case IDM_RUN:
			while (ResumeThread(ThreadsHandle[0]))
			{
				for (int i = 1; i < THREADS_COUNT; i++)
					ResumeThread(ThreadsHandle[i]);
			}
			break;
		case IDC_PAUSE_BUTTON:
		case IDM_PAUSE:
			for (int i = 0; i < THREADS_COUNT; i++)
				SuspendThread(ThreadsHandle[i]);
			break;
		default:
			return Window::OnCommand(wParam, lParam);
		}
		return 0;
	}
	
	DWORD MainWindow::ThreadProc(LPVOID param)
	{
		int threadIndex = int(param);
		Points[threadIndex].y = 0;

		ThreadsNeeded[threadIndex] = true;

		srand(time(nullptr)*threadIndex);
		ThreadsSpeed[threadIndex] = 1 + rand() % MAX_SPEED;

		int starSize = 0;
		POINT point = Points[threadIndex];

		while (ThreadsNeeded[threadIndex])
		{
			HDC hdc = GetDC(MainWindowHWND);

			if (starSize != 0)
				DrawStar(hdc, point.x, point.y, starSize, RGB(240, 240, 240));
			Points[threadIndex].y = (Points[threadIndex].y + ThreadsSpeed[threadIndex]) % MainWindowHeight;
			point = Points[threadIndex];
			starSize = StarSize;
			DrawStar(hdc, point.x, point.y, starSize, RGB(255, 50, 177));

			ReleaseDC(MainWindowHWND, hdc);

			Sleep(UPDATE_RATE);
		}
		
		ExitThread(0);

		return 0;
	}

	void MainWindow::DrawStar(HDC hdc, int centerX, int centerY, int size, COLORREF color)
	{
		HBRUSH brush = CreateSolidBrush(color), oldBrush;
		HPEN pen = CreatePen(NULL, 1, color), oldPen;
		oldBrush = HBRUSH(SelectObject(hdc, brush));
		oldPen = HPEN(SelectObject(hdc, pen));
		POINT points[5];
		
		for (int i = 1; i <= 5; i++)
		{
			points[i - 1].x = int(centerX + size*cos(2 * PI / 5 * (2 * i) + PI * 0.3));
			points[i - 1].y = int(centerY + size*sin(2 * PI / 5 * (2 * i) + PI * 0.3));
		}
		Polygon(hdc, points, 5);
		for (int i = 1; i <= 5; i++)
		{
			points[i - 1].x = int(centerX + (size / 2)*cos(2 * PI / 5 * (1 * i) + PI * 0.3));
			points[i - 1].y = int(centerY + (size / 2)*sin(2 * PI / 5 * (1 * i) + PI * 0.3));
		}
		Polygon(hdc, points, 5);
		SelectObject(hdc, oldBrush);
		SelectObject(hdc, oldPen);
		DeleteObject(brush);
		DeleteObject(pen);
	}
}