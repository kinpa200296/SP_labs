#pragma once

#include "Lab5Slave.h"

namespace Lab5Slave
{
	class MainWindow : public Window
	{
	public:
		MainWindow(HINSTANCE hInst, int nCmdShow, LPCWSTR appTitle, LPCWSTR mainWindowClassName, 
					int x, int y, int width, int height);
		~MainWindow();

		bool AddWindowMessages() override;

		int TargetX, TargetY, SelectedFigure, SelectedColor;
		UINT FigureColorMessage, RequestMessage;
		WCHAR FigureColorMessageString[MAX_STR], RequestMessageString[MAX_STR];
		bool DrawCheckBoxChecked;

		int Start();
		BOOL Show(int nCmdShow);

		LRESULT OnPaint(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnCommand(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnLeftButtonDown(WPARAM wParam, LPARAM lParam);
		LRESULT OnFigureColor(WPARAM wParam, LPARAM lParam);
	};
}