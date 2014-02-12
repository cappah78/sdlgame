#include "FileReader.h"

#include <sstream>
#include <fstream>

std::string FileReader::readStringFromFile(const char* path)
{
	if (!path)
		return 0;

	std::ifstream file(path);
	if (!file.is_open())
		printf("Could not find or open the file %s", path);

	std::stringstream data;
	data << file.rdbuf();
	file.close();

	return data.str();
}