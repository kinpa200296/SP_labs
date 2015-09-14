#pragma once

#include "Lab5Master.h"
#include "RadioButtonGroup.h"

namespace Lab5Master
{
	class MainWindow : public Window
	{
	public:
		MainWindow(HINSTANCE hInst, int nCmdShow, LPCWSTR appTitle, LPCWSTR mainWindowClassName, 
					int x, int y, int width, int height);
		~MainWindow();

		bool AddWindowMessages() override;

		RadioButtonGroup *FigureRadioButtonGroup, *ColorRadioButtonGroup;
		Button *DrawCheckBox;
		bool DrawCheckBoxChecked;
		UINT FigureColorMessage, RequestMessage;
		WCHAR FigureColorMessageString[MAX_STR], RequestMessageString[MAX_STR];

		int Start();
		BOOL Show(int nCmdShow);

		LRESULT OnResize(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnCommand(WPARAM wParam, LPARAM lParam) override;
		LRESULT OnRequest(WPARAM wParam, LPARAM lParam);
	};
}