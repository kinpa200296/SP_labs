#pragma once

#include "Application.h"

using namespace Lab8;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	Application app(hInstance, nCmdShow);

	return app.Run();;
}
