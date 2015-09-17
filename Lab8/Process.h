#pragma once

#include "Common.h"
#include "TlHelp32.h"
#include "Thread.h"
#include "Module.h"

namespace Lab8
{
	class Process
	{
	public:
		Process(PPROCESSENTRY32 data, DWORD desiredAccess);
		~Process();

		HANDLE Handle;
		DWORD Id, ThreadsCount, ParentId;
		LONG BasePriorityClass;
		WCHAR ExeFileName[MAX_STR];

		vector<Thread*> Threads;
		vector<Module*> Modules;

		int FindOwnedThreads(vector<Thread*> &threads);
		int LoadModules();
	};
}