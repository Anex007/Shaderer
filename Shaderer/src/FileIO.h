#pragma once

#include <fstream>
#include <sstream>


#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#elif PLATFORM_LINUX
#error Platform Unimplemented
#else
#error Platform Not supported
#endif // PLATFORM_WINDOWS

#include <iostream>

/* REMOVE THIS AFTER TESTING!!! */
static void DebugFileWriteCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
	return;
}

class IOManager
{
public:
	IOManager()
	{
#ifdef PLATFORM_WINDOWS
		;
#elif PLATFORM_LINUX
		;
#endif // PLATFORM_WINDOWS
	}
	~IOManager()
	{
#ifdef PLATFORM_WINDOWS

#elif PLATFORM_LINUX

#endif // PLATFORM_WINDOWS
	}

	bool WriteToFile(const char* fileName, const void* buffer, size_t bytesToWrite)
	{
		HANDLE hFile = CreateFileA(fileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return false;

		OVERLAPPED oFile;
		oFile.Offset = 0xffffffff;
		oFile.OffsetHigh = 0xffffffff;

		if (!WriteFileEx(hFile, buffer, bytesToWrite, &oFile, DebugFileWriteCompletion))
			return false;

		CloseHandle(hFile);
		return true;
	}

	bool ReadFromFile(const char* path, std::string& src)
	{
		std::ifstream fs(path);

		if (fs.fail()) {
			return false;
		}

		std::stringstream ss;
		ss << fs.rdbuf();
		src = ss.str();
		fs.close();

		return true;
	}

	static IOManager* GetManager()
	{
		if (s_Instance == nullptr)
			s_Instance = new IOManager();
		return s_Instance;
	}

private:
	static IOManager* s_Instance;
};
