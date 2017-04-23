// mapfile.cpp : Defines the entry point for the console application.
//
// Exemplo de utilização da API de mapeamento de ficheiros em memória
//
// 

#include "stdafx.h"
#include "MapFile.h"

// auxiliary method to convert the file access mode to file mapping access
static DWORD fileToMapAccess(int fa) {
	int ma = ((fa & GENERIC_READ) != 0) ? PAGE_READONLY : 0;
	if ((fa & GENERIC_WRITE) != 0) ma = PAGE_READWRITE;
	return ma;
}

// auxiliary method to convert the file access mode to view  access
static DWORD mapToViewAccess(int fa) {
	int va = ((fa & GENERIC_READ) != 0) ? FILE_MAP_READ : 0;
	if ((fa & GENERIC_WRITE) != 0) va |= FILE_MAP_WRITE;
	return va;
}

BOOL mapFile(LPCTSTR fileName, int access, int mode, LPCTSTR name, SIZE_T size, PFILEMAP fm) {
	HANDLE fh = INVALID_HANDLE_VALUE;
	HANDLE mh = NULL;
	LPVOID mapAddress = NULL;
 
	if (fileName != NULL) {
		if ((fh = CreateFile(fileName, access, 0, NULL, mode, 0, NULL)) == INVALID_HANDLE_VALUE) goto error;
		if (size == 0) {
			LARGE_INTEGER fileSize;
			if (!GetFileSizeEx(fh, &fileSize)) goto error;
			size = (SIZE_T)fileSize.QuadPart;
		}
	}
	LARGE_INTEGER aux;
	aux.QuadPart = size;
	mh = CreateFileMapping(fh, NULL, fileToMapAccess(access), aux.HighPart, aux.LowPart, name);
	if (mh == NULL) goto error;

	
	mapAddress = MapViewOfFile(mh, mapToViewAccess(access), 0, 0, 0);
	if (mapAddress == NULL) goto error;
	fm->baseAddress = (BYTE*)mapAddress;
	fm->mapHandle = mh;
	fm->error = 0;
	fm->size = size;
	CloseHandle(fh);
	return TRUE;

error:
	fm->error = GetLastError();
	if (mapAddress != NULL) UnmapViewOfFile(mapAddress);
	if (mh != NULL) CloseHandle(mh);
	if (fh != INVALID_HANDLE_VALUE) CloseHandle(fh);
	return FALSE;
}

VOID unmapFile(PFILEMAP fm) {
	UnmapViewOfFile(fm->baseAddress);
	CloseHandle(fm->mapHandle);
}


// Test program:
int _tmain(int argc, _TCHAR* argv[])
{
	int res;
	FILEMAP mapDesc;

	if (argc != 2) {
		_tprintf(_T("Usage: mapFile <fileName>\n"));
		return 1;
	}

	if ((res = mapFile(argv[1], GENERIC_READ, OPEN_EXISTING, NULL, 0, &mapDesc)) < 0)
		_tprintf(_T("Error %d mapping file\n"), res);
	else {
		int nints = mapDesc.size / sizeof(int);
		int *curr = (int *)mapDesc.baseAddress;

		for (int i = 0; i < nints; ++i) {
			_tprintf(_T("%d\n"), curr[i]);
		}
	}
	
	unmapFile(&mapDesc);
	return 0;
}

