#include "Mem.h"

Mem::Mem(const wchar_t* processName)
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (snapshot == INVALID_HANDLE_VALUE)
		return;

	while (Process32Next(snapshot, &entry))
		if (!_wcsicmp(entry.szExeFile, processName))
		{
			this->processID = entry.th32ProcessID;
			this->processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
			break;
		}
	CloseHandle(snapshot);
}


uintptr_t Mem::GetModuleAddress(const wchar_t* moduleName)
{
	MODULEENTRY32 entry;
	entry.dwSize = sizeof(MODULEENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processID);
	
	if (snapshot == INVALID_HANDLE_VALUE)
		return 0;

	while (Module32Next(snapshot, &entry))
		if (!_wcsicmp(entry.szModule, moduleName))
		{
			CloseHandle(snapshot);
			return (uintptr_t)entry.modBaseAddr;
		}
	CloseHandle(snapshot);
	return 0;
}

Mem::~Mem()
{
	if (this->processHandle)
		CloseHandle(this->processHandle);
}

bool Mem::InForeground()
{
	HWND tmp_hwnd = GetForegroundWindow();
	if (tmp_hwnd == NULL)
		return false;

	DWORD foregroundPID;
	if (!GetWindowThreadProcessId(tmp_hwnd, &foregroundPID))
		return false;
	return foregroundPID == this->processID;
}

DWORD Mem::GetProcessID()
{
	return processID;
}

HANDLE Mem::GetProcessHandle()
{
	return processHandle;
}