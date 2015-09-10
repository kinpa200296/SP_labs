#pragma once

#include "MainWindow.h"

namespace Lab2
{
	MainWindow::MainWindow(HINSTANCE hInst, int nCmdShow, LPCWSTR appTitle, LPCWSTR mainWindowClassName,
		int x, int y, int width, int height) : Window(hInst, NULL, x, y, width, height, appTitle,
		mainWindowClassName, 0, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN)
	{
		LeftListBox = nullptr;
		RightListBox = nullptr;
		NewItemEdit = nullptr;
		ToRightButton = nullptr;
		AddButton = nullptr;
		ClearButton = nullptr;
		DeleteButton = nullptr;
	}

	MainWindow::~MainWindow()
	{
		LeftListBox->~ListBox();
		RightListBox->~ListBox();
		NewItemEdit->~Edit();
		ToRightButton->~Button();
		AddButton->~Button();
		ClearButton->~Button();
		DeleteButton->~Button();
	}

	int MainWindow::Start()
	{
		RegisterWindowClass(CS_HREDRAW | CS_VREDRAW, HBRUSH(COLOR_WINDOW), IDC_ARROW,
			MAKEINTRESOURCE(IDI_ICON), MAKEINTRESOURCE(IDI_ICON_SMALL), MAKEINTRESOURCE(IDR_MENU));

		if (!Create())
			return 1;

		bool result = true;

		result = result && AddMessage(ThisWindow, WM_DESTROY, this, ToFuncPointer(&MainWindow::OnDestroy));
		result = result && AddMessage(ThisWindow, WM_CLOSE, this, ToFuncPointer(&MainWindow::OnClose));
		result = result && AddMessage(ThisWindow, WM_CREATE, this, ToFuncPointer(&MainWindow::OnCreate));
		result = result && AddMessage(ThisWindow, WM_COMMAND, this, ToFuncPointer(&MainWindow::OnCommand));
		result = result && AddMessage(ThisWindow, WM_SIZE, this, ToFuncPointer(&MainWindow::OnResize));
		result = result && AddMessage(ThisWindow, WM_MOVE, this, ToFuncPointer(&MainWindow::OnMove));
		//result = result && AddMessage(ThisWindow, WM_PAINT, this, ToFuncPointer(&MainWindow::OnPaint));

		if (!result)
			return 2;

		LeftListBox = new ListBox(Instance, ThisWindow, 0, 0, 100, 100, IDC_LEFT_LISTBOX, 
			LBS_EXTENDEDSEL | LBS_MULTIPLESEL);
		RightListBox = new ListBox(Instance, ThisWindow, 110, 0, 100, 100, IDC_RIGHT_LISTBOX,
			LBS_EXTENDEDSEL | LBS_MULTIPLESEL);
		NewItemEdit = new Edit(Instance, ThisWindow, 0, 110, 45, 20, IDC_NEWITEM_EDIT);
		AddButton = new Button(Instance, ThisWindow, 55, 110, 45, 20, IDC_ADD_BUTTON, L"Add");
		ToRightButton = new Button(Instance, ThisWindow, 0, 0, 0, 0, IDC_TORIGHT_BUTTON, L"ToRight");
		ClearButton = new Button(Instance, ThisWindow, 0, 0, 0, 0, IDC_CLEAR_BUTTON, L"Clear");
		DeleteButton = new Button(Instance, ThisWindow, 0, 0, 0, 0, IDC_DELETE_BUTTON, L"Delete");

		result = true;

		result = result && LeftListBox->Start();
		result = result && RightListBox->Start();
		result = result && NewItemEdit->Start();
		result = result && ToRightButton->Start();
		result = result && ClearButton->Start();
		result = result && AddButton->Start();
		result = result && DeleteButton->Start();

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
		PostQuitMessage(0);
		return 0;
	}
	
	LRESULT MainWindow::OnClose(WPARAM wParam, LPARAM lParam)
	{
		PostQuitMessage(0);
		return 0;
	}

	LRESULT MainWindow::OnCreate(WPARAM wParam, LPARAM lParam)
	{
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
		case IDM_ADD:
		case IDC_ADD_BUTTON:
			AddAction();
			break;
		case IDM_TORIGHT:
		case IDC_TORIGHT_BUTTON:
			ToRigthAction();
			break;
		case IDM_DELETE:
		case IDC_DELETE_BUTTON:
			DeleteAction();
			break;
		case IDM_CLEAR:
		case IDC_CLEAR_BUTTON:
			ClearAction();
			break;
		case IDM_CLEAR_SEL_LEFT:
			LeftListBox->ClearSelection();
			break;
		case IDM_CLEAR_SEL_RIGHT:
			RightListBox->ClearSelection();
			break;
		}
		return 0;
	}

	LRESULT MainWindow::OnResize(WPARAM wParam, LPARAM lParam)
	{
		Width = LOWORD(lParam);
		Height = HIWORD(lParam);
		int width = Width - 30, height = Height - 80;
		MoveWindow(LeftListBox->ThisWindow, 10, 10, width / 2, height, TRUE);
		MoveWindow(RightListBox->ThisWindow, width /2 + 20, 10, width / 2, height, TRUE);
		MoveWindow(NewItemEdit->ThisWindow, 10, height + 20, width / 3 * 2, 20, TRUE);
		MoveWindow(AddButton->ThisWindow, 20 + width / 3 * 2, height + 20, width / 3, 20, TRUE);
		MoveWindow(ToRightButton->ThisWindow, 10, height + 50, width / 3, 20, TRUE);
		MoveWindow(DeleteButton->ThisWindow, 15 + width / 3, height + 50, width / 3, 20, TRUE);
		MoveWindow(ClearButton->ThisWindow, 20 + width / 3 * 2, height + 50, width / 3, 20, TRUE);
		return 0;
	}

	LRESULT MainWindow::OnMove(WPARAM wParam, LPARAM lParam)
	{
		PosX = LOWORD(lParam);
		PosY = HIWORD(lParam);
		return 0;
	}

	LRESULT MainWindow::OnPaint(WPARAM wParam, LPARAM lParam)
	{
		/*PAINTSTRUCT ps;
		HDC hdc = BeginPaint(ThisWindow, &ps);
		EndPaint(ThisWindow, &ps);*/
		return 0;
	}
	
	void MainWindow::AddAction()
	{
		WCHAR buffer[MAX_STR];
		NewItemEdit->GetText(buffer, MAX_STR);
		if (!LeftListBox->AddStringUnique(buffer))
		{
			MessageBox(ThisWindow, L"Left listbox already has that string.", L"Error", MB_OK);
		}
		else
		{
			NewItemEdit->Clear();
		}
	}

	void MainWindow::ToRigthAction()
	{
		WCHAR buffer[MAX_STR];
		vector<int> selection = LeftListBox->GetSelectedItemsIndex();
		bool res = true;
		for (int i = 0; i < selection.size(); i++)
		{
			LeftListBox->GetString(selection[i], buffer);
			res = RightListBox->AddStringUnique(buffer) && res;
		}
		if (!res)
		{
			MessageBox(ThisWindow, L"Operation completed. But some of the selected strings were already present in the right listbox.",
				L"Warning", MB_OK);
		}
	}

	void MainWindow::DeleteAction()
	{
		vector<int> leftSelection = LeftListBox->GetSelectedItemsIndex();
		vector<int> rightSelection = RightListBox->GetSelectedItemsIndex();
		for (int i = 0, tmp = 0; i < leftSelection.size(); i++, tmp++)
		{
			LeftListBox->DeleteString(leftSelection[i] - tmp);
		}
		for (int i = 0, tmp = 0; i < rightSelection.size(); i++, tmp++)
		{
			RightListBox->DeleteString(rightSelection[i] - tmp);
		}
		LeftListBox->ClearSelection();
		RightListBox->ClearSelection();
	}

	void MainWindow::ClearAction()
	{
		LeftListBox->Clear();
		RightListBox->Clear();
	}
}