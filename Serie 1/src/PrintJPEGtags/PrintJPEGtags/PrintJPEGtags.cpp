// PrintJpegInfo.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "windows.h"
#include "stdio.h"
#include "string"
#include "MapFile.h"

typedef struct exif
{
	std::string imageDescription;
	std::string dateTime;
	unsigned short iso;
	unsigned imageWidth;
	unsigned imageHeight;
	double exposureValue;
	double apertureValue;
	double latitude;
	double longitude;
	double altitude;
}JPEGDATA;

JPEGDATA loadExif(FILEMAP jpegFile) {
	JPEGDATA tags;
	BYTE* bAddress = jpegFile.baseAddress;
	BYTE* idf0 = bAddress + ;
	tags.imageDescription = &(bAddress+);
}

int printJpegInfo(LPCWSTR file) {
	HANDLE fileH = CreateFile(file, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	DWORD size = GetFileSize(fileH, NULL);
	FILEMAP mappedFile;
	bool suceeded = mapFile(argv[0], GENERIC_READ, OPEN_EXISTING, NULL, (size_t)size, &mappedFile);
	return 0;
}

int _tmain() {
	return 0;
}