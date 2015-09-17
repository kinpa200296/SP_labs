#pragma once

#include "Thread.h"

namespace Lab8
{
	Thread::Thread(PTHREADENTRY32 data, DWORD desiredAccess)
	{
		Id = data->th32ThreadID;
		OwnerProcessId = data->th32OwnerProcessID;
		BasePriorityLevel = data->tpBasePri;

		Handle = OpenThread(desiredAccess, FALSE, Id);
	}

	Thread::~Thread()
	{
		CloseHandle(Handle);
	}
}