#pragma once

#include "Module.h"

namespace Lab8
{
	Module::Module(PMODULEENTRY32 data)
	{
		ProcessId = data->th32ProcessID;
		Size = data->modBaseSize;
		ModuleBaseAdress = data->modBaseAddr;
		Handle = data->hModule;
		lstrcpy(Name, data->szModule);
		lstrcpy(Path, data->szExePath);
	}

	Module::~Module()
	{
	}
}