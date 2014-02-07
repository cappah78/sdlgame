#pragma once

#include <string>

class FileReader
{
public:
	static std::string readStringFromFile(const char* const path);

private:
	FileReader();
	~FileReader();
};