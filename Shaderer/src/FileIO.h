#pragma once

#include <fstream>
#include <sstream>

#include <iostream>


class IOManager
{
public:
	IOManager()
	{
	}

	~IOManager()
	{
	}

    bool WriteToFile(const char* fileName, const char* buffer)
    {
        std::ofstream fs(fileName);
        if (!fs.is_open()) {
            return false;
        }
        fs << buffer;
        fs.close();
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

#ifdef PLATFORM_WINDOWS

#else
#ifdef PLATFORM_LINUX

#endif
#endif



	static IOManager* GetManager()
	{
		if (s_Instance == nullptr)
			s_Instance = new IOManager();
		return s_Instance;
	}

private:
	static IOManager* s_Instance;
};
