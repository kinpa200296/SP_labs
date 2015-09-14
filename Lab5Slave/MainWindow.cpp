#pragma once

#include "MainWindow.h"

namespace Lab5Slave
{
	MainWindow::MainWindow(HINSTANCE hInst, int nCmdShow, LPCWSTR appTitle, LPCWSTR mainWindowClassName,
		int x, int y, int width, int height) : Window(hInst, nullptr, x, y, width, height, appTitle,
		mainWindowClassName, 0, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN)
	{
		IsMainWindow = true;
		TargetX = 0;
		TargetY = 0;
		LoadString(Instance, IDS_MESSAGESTRING, FigureColorMessageString, MAX_STR);
		FigureColorMessage = RegisterWindowMessage(FigureColorMessageString);
		LoadString(Instance, IDS_REQUESTSTRING, RequestMessageString, MAX_STR);
		RequestMessage = RegisterWindowMessage(RequestMessageString);
		DrawCheckBoxChecked = false;
		SelectedFigure = 0;
		SelectedColor = 0;
	}

	MainWindow::~MainWindow()
	{
	}

	bool MainWindow::AddWindowMessages()
	{
		bool result = Window::AddWindowMessages();

		result = result && AddMessage(ThisWindow, FigureColorMessage, this, 
			ToFuncPointer(&MainWindow::OnFigureColor));
		result = result && AddMessage(ThisWindow, WM_LBUTTONDOWN, this,
			ToFuncPointer(&MainWindow::OnLeftButtonDown));

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
			return WinApiWrap::Window::OnCommand(wParam, lParam);
		}
		return 0;
	}

	LRESULT MainWindow::OnPaint(WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(ThisWindow, &ps);
		if (DrawCheckBoxChecked)
		{
			HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0)), oldBrush;
			HPEN pen = CreatePen(NULL, 1, RGB(0, 0, 0)), oldPen;
			switch (SelectedColor)
			{
			case ID_RED:
				brush = CreateSolidBrush(RGB(255, 0, 0));
				pen = CreatePen(NULL, 1, RGB(255, 0, 0));
				break;
			case ID_GREEN:
				brush = CreateSolidBrush(RGB(0, 255, 0));
				pen = CreatePen(NULL, 1, RGB(0, 255, 0));
				break;
			case ID_BLUE:
				brush = CreateSolidBrush(RGB(0, 0, 255));
				pen = CreatePen(NULL, 1, RGB(0, 0, 255));
				break;
			}
			oldBrush = HBRUSH(SelectObject(hdc, brush));
			oldPen = HPEN(SelectObject(hdc, pen));
			POINT points[10];
			switch (SelectedFigure)
			{
			case ID_RHOMBUS:
				points[0].x = TargetX; points[0].y = TargetY - FIGURE_SIZE / 2;
				points[1].x = TargetX + FIGURE_SIZE / 2; points[1].y = TargetY;
				points[2].x = TargetX; points[2].y = TargetY + FIGURE_SIZE / 2;
				points[3].x = TargetX - FIGURE_SIZE / 2; points[3].y = TargetY;
				Polygon(hdc, points, 4);
				break;
			case ID_SQUARE:
				Rectangle(hdc, TargetX - FIGURE_SIZE / 2, TargetY - FIGURE_SIZE / 2,
					TargetX + FIGURE_SIZE / 2, TargetY + FIGURE_SIZE / 2);
				break;
			case ID_CIRCLE:
				Ellipse(hdc, TargetX - FIGURE_SIZE / 2, TargetY - FIGURE_SIZE / 2,
					TargetX + FIGURE_SIZE / 2, TargetY + FIGURE_SIZE / 2);
				break;
			case ID_STAR:
				for (int i = 1; i <= 5; i++)
				{
					points[i - 1].x = int(TargetX + FIGURE_SIZE*cos(2 * PI / 5 * (2 * i) + PI * 0.3));
					points[i - 1].y = int(TargetY + FIGURE_SIZE*sin(2 * PI / 5 * (2 * i) + PI * 0.3));
				}
				Polygon(hdc, points, 5);
				for (int i = 1; i <= 5; i++)
				{
					points[i - 1].x = int(TargetX + (FIGURE_SIZE / 2)*cos(2 * PI / 5 * (1 * i) + PI * 0.3));
					points[i - 1].y = int(TargetY + (FIGURE_SIZE / 2)*sin(2 * PI / 5 * (1 * i) + PI * 0.3));
				}
				Polygon(hdc, points, 5);
				break;
			}
			SelectObject(hdc, oldBrush);
			SelectObject(hdc, oldPen);
			DeleteObject(brush);
			DeleteObject(pen);
		}
		EndPaint(ThisWindow, &ps);
		return 0;
	}

	LRESULT MainWindow::OnLeftButtonDown(WPARAM wParam, LPARAM lParam)
	{
		TargetX = LOWORD(lParam);
		TargetY = HIWORD(lParam);

		SendMessage(HWND_BROADCAST, RequestMessage, NULL, NULL);

		RECT tmpRect;
		tmpRect.left = tmpRect.top = 0;
		tmpRect.bottom = Height; tmpRect.right = Width;
		InvalidateRect(ThisWindow, &tmpRect, TRUE);

		return 0;
	}

	LRESULT MainWindow::OnFigureColor(WPARAM wParam, LPARAM lParam)
	{
		DrawCheckBoxChecked = wParam != 0;
		SelectedFigure = LOWORD(lParam);
		SelectedColor = HIWORD(lParam);
		return 0;
	}
}