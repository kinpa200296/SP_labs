#pragma once

#include "MainWindow.h"

namespace Lab5Master
{
	MainWindow::MainWindow(HINSTANCE hInst, int nCmdShow, LPCWSTR appTitle, LPCWSTR mainWindowClassName,
		int x, int y, int width, int height) : Window(hInst, nullptr, x, y, width, height, appTitle,
		mainWindowClassName, 0, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN)
	{
		IsMainWindow = true;
		FigureRadioButtonGroup = nullptr;
		ColorRadioButtonGroup = nullptr;
		DrawCheckBox = nullptr;
		DrawCheckBoxChecked = false;
		LoadString(Instance, IDS_MESSAGESTRING, FigureColorMessageString, MAX_STR);
		FigureColorMessage = RegisterWindowMessage(FigureColorMessageString);
		LoadString(Instance, IDS_REQUESTSTRING, RequestMessageString, MAX_STR);
		RequestMessage = RegisterWindowMessage(RequestMessageString);
	}

	MainWindow::~MainWindow()
	{
	}

	bool MainWindow::AddWindowMessages()
	{
		bool result = Window::AddWindowMessages();

		result = result && AddMessage(ThisWindow, RequestMessage, this,
			ToFuncPointer(&MainWindow::OnRequest));

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

		LPWSTR figures[4] = { L"Rhombus", L"Square", L"Circle", L"Star" };
		LPWSTR colors[3] = { L"Red", L"Green", L"Blue" };

		FigureRadioButtonGroup = new RadioButtonGroup(Instance, ThisWindow, 0, 0, 0, 0,
			IDC_FIGURERADIOBUTTONGROUP, L"Figures", NULL, (const wchar_t**)figures, 4);
		ColorRadioButtonGroup = new RadioButtonGroup(Instance, ThisWindow, 0, 0, 0, 0,
			IDC_COLORRADIOBUTTONGROUP, L"Colors", NULL, (const wchar_t**)colors, 3);
		DrawCheckBox = new Button(IDSC_BUTTON, Instance, ThisWindow, 0, 0, 0, 0, IDC_DRAWCHECKBOX,
			L"Draw", BS_CHECKBOX);

		bool result = true;

		result = result && !FigureRadioButtonGroup->Start();
		result = result && !ColorRadioButtonGroup->Start();
		result = result && !DrawCheckBox->Start();

		if (!result)
			return 3;

		return 0;
	}

	BOOL MainWindow::Show(int nCmdShow)
	{
		return ShowWindow(ThisWindow, nCmdShow);
	}
	
	LRESULT MainWindow::OnResize(WPARAM wParam, LPARAM lParam)
	{
		Window::OnResize(wParam, lParam);

		MoveWindow(FigureRadioButtonGroup->ThisWindow, 10, 10, (Width - 30) / 2, Height - 50, TRUE);
		MoveWindow(ColorRadioButtonGroup->ThisWindow, 20 + (Width - 30) / 2, 10, (Width - 30) / 2,
			Height - 50, TRUE);
		MoveWindow(DrawCheckBox->ThisWindow, 10, Height - 40, Width, 20, TRUE);

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
		case IDC_DRAWCHECKBOX:
			DrawCheckBoxChecked = !DrawCheckBoxChecked;
			Button_SetCheck(DrawCheckBox->ThisWindow, DrawCheckBoxChecked);
			break;
		default:
			return Window::OnCommand(wParam, lParam);
		}
		return 0;
	}

	LRESULT MainWindow::OnRequest(WPARAM wParam, LPARAM lParam)
	{
		SendMessage(HWND_BROADCAST, FigureColorMessage, WPARAM(DrawCheckBoxChecked),
			MAKELPARAM(FigureRadioButtonGroup->CheckedButton, ColorRadioButtonGroup->CheckedButton));
		return 0;
	}
}