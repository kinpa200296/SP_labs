#pragma once

#include "MainWindow.h"

namespace Lab3
{
	MainWindow::MainWindow(HINSTANCE hInst, int nCmdShow, LPCWSTR appTitle, LPCWSTR mainWindowClassName,
		int x, int y, int width, int height) : Window(hInst, nullptr, x, y, width, height, appTitle,
		mainWindowClassName, 0, WS_OVERLAPPEDWINDOW)
	{
		IsMainWindow = true;
		DisplayPicture = false;
		DrawButton = nullptr;
		ClearButton = nullptr;
	}

	MainWindow::~MainWindow()
	{
		DrawButton->~Button();
		ClearButton->~Button();
	}


	bool MainWindow::AddWindowMessages()
	{
		bool result = Window::AddWindowMessages();

		result = result && AddMessage(ThisWindow, WM_DRAWITEM, this, ToFuncPointer(&MainWindow::OnDrawItem));

		return result;
	}

	int MainWindow::Start()
	{
		RegisterWindowClass(CS_HREDRAW | CS_VREDRAW, HBRUSH(COLOR_WINDOW), IDC_ARROW, MAKEINTRESOURCE(IDI_ICON), 
			MAKEINTRESOURCE(IDI_ICON_SMALL), MAKEINTRESOURCE(IDR_MENU));

		int res = Create();

		if (res)
		{
			return res;
		}

		DrawButton = new Button(IDSC_BUTTON, Instance, ThisWindow, 0, 0, 0, 0, IDC_DRAW_BUTTON, L"Draw",
			BS_OWNERDRAW);
		ClearButton = new Button(IDSC_BUTTON, Instance, ThisWindow, 0, 0, 0, 0, IDC_CLEAR_BUTTON, L"Clear",
			BS_OWNERDRAW);

		bool result = true;

		result = result && !DrawButton->Start();
		result = result && !ClearButton->Start();

		if (!result)
			return 3;

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
		case IDM_DRAW:
		case IDC_DRAW_BUTTON:
			ChangePictureState(true);
			break;
		case IDM_CLEAR:
		case IDC_CLEAR_BUTTON:
			ChangePictureState(false);
			break;
		default:
			return Window::OnCommand(wParam, lParam);
		}
		return 0;
	}

	LRESULT MainWindow::OnResize(WPARAM wParam, LPARAM lParam)
	{
		Window::OnResize(wParam, lParam);
		
		MoveWindow(DrawButton->ThisWindow, 10, Height - 30, (Width - 30) / 2, 20, TRUE);
		MoveWindow(ClearButton->ThisWindow, 20 + (Width - 30) / 2, Height - 30, (Width - 30) / 2, 20, TRUE);

		return 0;
	}

	LRESULT MainWindow::OnPaint(WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(ThisWindow, &ps);
		if (DisplayPicture)
		{
			DrawPicture(hdc);
		}
		EndPaint(ThisWindow, &ps);
		return 0;
	}

	LRESULT MainWindow::OnDrawItem(WPARAM wParam, LPARAM lParam)
	{
		if (wParam == IDC_DRAW_BUTTON)
			return DrawButton->OwnerDrawProc(wParam, lParam);
		if (wParam == IDC_CLEAR_BUTTON)
			return ClearButton->OwnerDrawProc(wParam, lParam);
		return DefWindowProc(ThisWindow, WM_DRAWITEM, wParam, lParam);
	}

	void MainWindow::ChangePictureState(bool display)
	{
		DisplayPicture = display;
		RECT tmpRect;

		tmpRect.top = 0;
		tmpRect.left = 0;
		tmpRect.right = Width;
		tmpRect.bottom = Height;

		InvalidateRect(ThisWindow, &tmpRect, TRUE);
	}

	void MainWindow::DrawPicture(HDC hdc)
	{
		HGDIOBJ penBlack = CreatePen(NULL, 1, RGB(0, 0, 0));
		HGDIOBJ penRed = CreatePen(NULL, 1, RGB(255, 0, 0));
		HGDIOBJ penYellow = CreatePen(NULL, 1, RGB(255, 255, 0));
		HGDIOBJ penGreen = CreatePen(NULL, 1, RGB(0, 128, 0));

		HGDIOBJ brushBlack = CreateSolidBrush(RGB(0, 0, 0));
		HGDIOBJ brushGray = CreateSolidBrush(RGB(128, 128, 128));
		HGDIOBJ brushGreen = CreateSolidBrush(RGB(0, 128, 0));
		HGDIOBJ brushBlue = CreateSolidBrush(RGB(108, 207, 254));
		HGDIOBJ brushYellow = CreateSolidBrush(RGB(255, 255, 0));

		const int LEFT = 25, TOP = 100;

		HGDIOBJ dcPen = SelectObject(hdc, penRed);
		HGDIOBJ dcBrush = SelectObject(hdc, brushGreen);
		Rectangle(hdc, LEFT + 200, TOP + 100, LEFT + 400, TOP + 240);

		SelectObject(hdc, brushBlack);
		Rectangle(hdc, LEFT + 240, TOP + 170, LEFT + 270, TOP + 240);

		SelectObject(hdc, brushBlue);
		Rectangle(hdc, LEFT + 320, TOP + 150, LEFT + 360, TOP + 190);

		SelectObject(hdc, brushBlack);
		SelectObject(hdc, penBlack);
		POINT points[4];

		points[0].x = LEFT + 250;
		points[0].y = TOP + 80;

		points[1].x = LEFT + 250;
		points[1].y = TOP + 50;

		points[2].x = LEFT + 270;
		points[2].y = TOP + 50;

		points[3].x = LEFT + 270;
		points[3].y = TOP + 80;

		Polygon(hdc, points, 4);

		SelectObject(hdc, brushGray);

		points[0].x = LEFT + 200;
		points[0].y = TOP + 100;
		points[1].x = LEFT + 300;
		points[1].y = TOP + 50;
		points[2].x = LEFT + 400;
		points[2].y = TOP + 100;
		Polygon(hdc, points, 3);

		SelectObject(hdc, brushBlue);
		Ellipse(hdc, LEFT + 290, TOP + 70, LEFT + 310, TOP + 90);

		SelectObject(hdc, penYellow);
		SelectObject(hdc, brushYellow);

		Ellipse(hdc, LEFT + 450, TOP + 10, LEFT + 500, TOP + 60);

		SelectObject(hdc, brushBlue);
		SelectObject(hdc, penBlack);
		MoveToEx(hdc, LEFT + 320, TOP + 170, nullptr);
		LineTo(hdc, LEFT + 360, TOP + 170);

		MoveToEx(hdc, LEFT + 340, TOP + 150, nullptr);
		LineTo(hdc, LEFT + 340, TOP + 190);

		SelectObject(hdc, penGreen);
		Arc(hdc, LEFT + 100, TOP + 200,	LEFT + 200, TOP + 280, LEFT + 150, TOP + 200, LEFT + 100, TOP + 240);
		Arc(hdc, LEFT + 100, TOP + 150, LEFT + 200, TOP + 330, LEFT + 150, TOP + 200, LEFT + 100, TOP + 240);
		Arc(hdc, LEFT, TOP + 200, LEFT + 100, TOP + 280, LEFT + 100, TOP + 240,	LEFT + 50, TOP + 200);
		Arc(hdc, LEFT, TOP + 150, LEFT + 100, TOP + 330, LEFT + 100, TOP + 240, LEFT + 50, TOP + 200);

		SelectObject(hdc, dcPen);
		SelectObject(hdc, dcBrush);

		DeleteObject(penBlack);
		DeleteObject(penGreen);
		DeleteObject(penRed);
		DeleteObject(penYellow);

		DeleteObject(brushBlack);
		DeleteObject(brushBlue);
		DeleteObject(brushGray);
		DeleteObject(brushGreen);
		DeleteObject(brushYellow);
	}
}