#pragma once
#include <string>
#include "project.h"

namespace core {
// Very simple text-file storage: one task per line: "<id>|<done>|<title>"
class FileStorage {
public:
    void save(const Project& project, const std::string& file_path) const;
    Project load(const std::string& file_path) const;
};
} // namespace core
