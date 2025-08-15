#pragma once
#include <string>
#include "project.h"

namespace core {
// JSON file storage using nlohmann_json.
class FileStorage {
public:
    void save(const Project& project, const std::string& file_path) const;
    Project load(const std::string& file_path) const;
};
} // namespace core
