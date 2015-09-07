#pragma once

#include "Application.h"

using namespace TemplateApp;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	Application app(hInstance, nCmdShow);

	app.Run();

	return 0;
}
