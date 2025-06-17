//Create a FileManager class that:
//Opens a file in its constructor (for example, using std::fopen).
//Closes a file in its destructor (for example, using std::fclose).
//Add a write(const std::string&) method to write text to a file.
//
//Handle exceptions cases:
//Throw an exception if the file cannot be opened.
//
//Ensure that the file is automatically closed even if an exception occurs.

#define _CRT_SECURE_NO_DEPRECATE
#include "FileManager.h"
#include <stdexcept>
#include <iostream>

FileManager::FileManager(const char* filename, const char* mode)
{
	file_ = std::fopen(filename, mode);
    if (!file_) {
        throw std::runtime_error("Failed to open file");
    }
    std::cout << "File opened successfully\n";
}

FileManager::~FileManager()
{
    if (file_) {
        std::fclose(file_);
        std::cout << "File closed automatically\n";
    }
}

void FileManager::write(const std::string& str)
{
    std::fprintf(file_, "%s\n", str.data());
}
