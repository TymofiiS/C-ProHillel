#pragma once
#include <string>

namespace app {
class CLIHandler {
public:
    int run(int argc, char** argv);
private:
    int cmd_create(const std::string& name, const std::string& description, const std::string& file);
    int cmd_list(const std::string& file);
    int cmd_add_task(const std::string& file, const std::string& title, const std::string& priority);
    int cmd_list_tasks(const std::string& file);
    int cmd_update_task(const std::string& file, int task_id, const std::string& status);
    int cmd_export_json(const std::string& file, const std::string& out_file);
    int cmd_import_json(const std::string& file, const std::string& in_file);
};
} // namespace app
