#include <stdio.h>
#include <Windows.h> 
#include <assert.h>
#include <Psapi.h>
#include <psapi.h>
#include <tchar.h>
#include <malloc.h>
#include <iostream>
#include <vector>
#include <set>
#pragma comment(lib, "psapi.lib")

#define DIV 1024 // use to convert bytes to KB




void GetLocalProcessInfo(HANDLE hProcess, DWORD processID)
{

	PROCESS_MEMORY_COUNTERS_EX process_memory;

	printf("Informacao sobre o processo numero %d \n", processID);

	GetProcessMemoryInfo(hProcess, reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&process_memory), sizeof(process_memory));

}

void GetProcessName(HANDLE hProcess)
{

	if (hProcess)
	{
		wchar_t buffer[MAX_PATH];
		if (GetModuleFileNameEx(hProcess, 0, buffer, sizeof(buffer) - 1))
		{
			//std::cout << buffer << "\n";
			printf("Name process: %ls \n\n", buffer);
			
			char path_buffer[_MAX_PATH];
			char drive[_MAX_DRIVE];
			char dir[_MAX_DIR];
			char fname[_MAX_FNAME];
			char ext[_MAX_EXT];
			wcstombs(path_buffer, buffer, sizeof(buffer));
			_splitpath(path_buffer, drive, dir, fname, ext);
			printf("Path extracted with _splitpath:\n");
			printf("  Drive: %s\n", drive);
			printf("  Dir: %s\n", dir);
			printf("  Filename: %s\n", fname);
			printf("  Ext: %s\n", ext);
		}
		else
		{
			GetLastError();
		}
		//CloseHandle(hProcess);
	}
}
int _tmain(int argc, TCHAR* argv[]) {

	int pid;
	HANDLE process;
	pid = (argc < 2) ? GetCurrentProcessId() : _ttoi(argv[1]);

	
	process = OpenProcess(
		PROCESS_VM_READ | PROCESS_QUERY_INFORMATION,
		FALSE,
		pid);
	GetLocalProcessInfo(process, pid);
	GetProcessName(process);
	printf("Usage: %d process ID \n\n", pid);
	CloseHandle(process);

	return 0;
}