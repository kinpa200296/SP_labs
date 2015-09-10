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

		LRESULT OnDestroy(WPARAM, LPARAM);
		LRESULT OnClose(WPARAM, LPARAM);
		LRESULT OnCreate(WPARAM, LPARAM);
		LRESULT OnPaint(WPARAM, LPARAM);
		LRESULT OnCommand(WPARAM, LPARAM);
		LRESULT OnResize(WPARAM, LPARAM);
		LRESULT OnMove(WPARAM, LPARAM);
	};
}