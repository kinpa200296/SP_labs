#pragma once

#include "Lab2.h"
#include "Button.h"
#include "Edit.h"
#include "ListBox.h"

namespace Lab2
{
	class MainWindow : public Window
	{
	public:
		MainWindow(HINSTANCE hInst, int nCmdShow, LPCWSTR appTitle, LPCWSTR mainWindowClassName, 
					int x, int y, int width, int height);
		~MainWindow();

		ListBox *LeftListBox, *RightListBox;
		Edit *NewItemEdit;
		Button *ToRightButton, *ClearButton, *AddButton, *DeleteButton;

		int Start();
		BOOL Show(int nCmdShow);
		void AddAction();
		void ToRigthAction();
		void DeleteAction();
		void ClearAction();

		LRESULT OnCommand(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnResize(WPARAM wParam, LPARAM lParam) override;
	};
}