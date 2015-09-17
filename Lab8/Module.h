#pragma once

#include "Common.h"
#include "TlHelp32.h"

namespace Lab8
{
	class Module
	{
	public:
		Module(PMODULEENTRY32 data);
		~Module();

		DWORD ProcessId, Size;
		BYTE *ModuleBaseAdress;
		HMODULE Handle;

		WCHAR Name[MAX_STR], Path[MAX_STR];
	};
}