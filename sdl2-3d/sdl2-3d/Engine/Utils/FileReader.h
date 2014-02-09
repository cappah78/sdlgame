#pragma once

#include <string>

class FileReader
{
public:
	static std::string readStringFromFile(const char* path);

private:
	FileReader();
	~FileReader();
};