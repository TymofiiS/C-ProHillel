#pragma once
#include <cstdio>
#include <string>
class FileManager {

private:
    std::FILE* file_;

public:
    explicit FileManager(const char* filename, const char* mode);

    ~FileManager();

    void write(const std::string&);
};

