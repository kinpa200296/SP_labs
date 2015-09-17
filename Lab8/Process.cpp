#pragma once

#include "Process.h"

namespace Lab8
{
	Process::Process(PPROCESSENTRY32W data, DWORD desiredAccess)
	{
		Id = data->th32ProcessID;
		ParentId = data->th32ParentProcessID;
		BasePriorityClass = data->pcPriClassBase;
		ThreadsCount = data->cntThreads;
		lstrcpy(ExeFileName, data->szExeFile);

		Handle = OpenProcess(desiredAccess, FALSE, Id);
	}

	Process::~Process()
	{
		for (int i = 0; i < Threads.size(); i++)
		{
			Threads[i]->~Thread();
		}
		for (int i = 0; i < Modules.size(); i++)
		{
			Modules[i]->~Module();
		}

		CloseHandle(Handle);
	}

	int Process::FindOwnedThreads(vector<Thread*>& threads)
	{
		for (int i = 0; i < threads.size(); i++)
			if (Id == threads[i]->OwnerProcessId)
				Threads.push_back(threads[i]);
		return 0;
	}

	int Process::LoadModules()
	{
		HANDLE snapshotHandle;
		MODULEENTRY32 moduleData;

		snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, Id);
		if (snapshotHandle == INVALID_HANDLE_VALUE)
		{
			return 5;
		}

		moduleData.dwSize = sizeof(MODULEENTRY32);

		if (!Module32First(snapshotHandle, &moduleData))
		{
			CloseHandle(snapshotHandle);
			return 6;
		}

		do
		{
			Module *module = new Module(&moduleData);
			Modules.push_back(module);
		} while (Module32Next(snapshotHandle, &moduleData));

		CloseHandle(snapshotHandle);
		return 0;
	}
}