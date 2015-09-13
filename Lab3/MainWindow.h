#pragma once

#include "Lab3.h"
#include "Button.h"

namespace Lab3
{
	class MainWindow : public Window
	{
	public:
		MainWindow(HINSTANCE hInst, int nCmdShow, LPCWSTR appTitle, LPCWSTR mainWindowClassName, 
					int x, int y, int width, int height);
		~MainWindow();

		Button *DrawButton, *ClearButton;
		bool DisplayPicture;

		bool AddWindowMessages() override;

		int Start();
		BOOL Show(int nCmdShow);

		LRESULT OnResize(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnPaint(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnCommand(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnDrawItem(WPARAM wParam, LPARAM lParam);

		void ChangePictureState(bool display);
		void DrawPicture(HDC hdc);
	};
}