#include "FileUtils.h"
#include <windows.h>
#include <commdlg.h>
#include <fstream>


// read the contents of a file and return it as a string
std::string FileUtils::readFile(const std::string& filePath)
{
	std::ifstream infile{ filePath };
	std::string file_contents{ std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>() };
    return file_contents;
}

std::wstring FileUtils::openFileDialog(const wchar_t* filter) {
    OPENFILENAME ofn;       // Common dialog box structure
    wchar_t szFile[260] = { 0 }; // Buffer for file name
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr; // Or your window handle
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    //wchar_t* ptr = const_cast<wchar_t*>(filter);
    ofn.lpstrFilter = filter;//filter;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE) {
        return ofn.lpstrFile;
    }
    return {};
}