#include "FileUtils.h"
#include <fstream>

// read the contents of a file and return it as a string
std::string FileUtils::readFile(const std::string& filePath)
{
	std::ifstream infile{ filePath };
	std::string file_contents{ std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>() };
    return file_contents;
}
