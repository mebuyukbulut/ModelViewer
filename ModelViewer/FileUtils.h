#pragma once

#include <string>

class FileUtils
{
public:
	static std::string readFile(const std::string& filePath);
	static std::wstring openFileDialog(const wchar_t* filter = L"All Files\0*.*\0");
};


