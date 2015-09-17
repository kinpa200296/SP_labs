#pragma once

#include "Common.h"
#include "TlHelp32.h"

namespace Lab8
{
	class Thread
	{
	public:
		Thread(PTHREADENTRY32 data, DWORD desiredAccess);
		~Thread();

		HANDLE Handle;
		DWORD Id, OwnerProcessId;
		LONG BasePriorityLevel;
	};
}