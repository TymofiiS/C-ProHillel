#include "core/storage.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>

using nlohmann::json;

namespace core {

void FileStorage::save(const Project& project, const std::string& file_path) const {
    json j;
    j["name"] = project.name();
    j["tasks"] = json::array();
    for (const auto& t : project.tasks()) {
        j["tasks"].push_back({{"id", t.id}, {"title", t.title}, {"done", t.done}});
    }
    std::ofstream ofs(file_path, std::ios::trunc);
    if (!ofs) throw std::runtime_error("Failed to open file for writing: " + file_path);
    ofs << j.dump(2);
}

Project FileStorage::load(const std::string& file_path) const {
    std::ifstream ifs(file_path);
    if (!ifs) throw std::runtime_error("Failed to open file for reading: " + file_path);
    json j; ifs >> j;
    Project p{ j.value("name", std::string{"Untitled"}) };
    for (const auto& jt : j.value("tasks", json::array())) {
        p.add_task_raw(jt.value("id", 0), jt.value("title", std::string{}), jt.value("done", false));
    }
    return p;
}

} // namespace core
