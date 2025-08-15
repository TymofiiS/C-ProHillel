#include "app/cli_handler.h"
#include "core/project.h"
#include "core/storage.h"
#include <spdlog/spdlog.h>
#include <CLI/CLI.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>

using nlohmann::json;

namespace {
constexpr const char* DEFAULT_FILE = "projects.json";
}

namespace app {

int CLIHandler::run(int argc, char** argv) {
    CLI::App app{"ProjectManager CLI"};
    app.require_subcommand(1);

    std::string file = DEFAULT_FILE;

    // Global option for storage file
    app.add_option("-f,--file", file, "Storage JSON file (default: projects.json)");

    // create
    std::string name, description;
    auto* create = app.add_subcommand("create", "Create a new project");
    create->add_option("--name", name, "Project name")->required();
    create->add_option("--description", description, "Project description");
    create->callback([&]{ this->cmd_create(name, description, file); });

    // list (projects) - single-project demo
    auto* list = app.add_subcommand("list", "List projects (demo: shows the single stored project name)");
    list->callback([&]{ this->cmd_list(file); });

    // add-task
    std::string title, priority="normal";
    auto* add_task = app.add_subcommand("add-task", "Add a task to the stored project");
    add_task->add_option("--title", title, "Task title")->required();
    add_task->add_option("--priority", priority, "Priority (low|normal|high)");
    add_task->callback([&]{ this->cmd_add_task(file, title, priority); });

    // list-tasks
    auto* list_tasks = app.add_subcommand("list-tasks", "List tasks of the stored project");
    list_tasks->callback([&]{ this->cmd_list_tasks(file); });

    // update-task
    int task_id = 0; std::string status;
    auto* update_task = app.add_subcommand("update-task", "Update task status");
    update_task->add_option("--task-id", task_id, "Task id")->required();
    update_task->add_option("--status", status, "Status (completed|open)")->required();
    update_task->callback([&]{ this->cmd_update_task(file, task_id, status); });

    // export (json)
    std::string out_file;
    auto* export_cmd = app.add_subcommand("export", "Export project to JSON");
    export_cmd->add_option("--out", out_file, "Output JSON file")->required();
    export_cmd->callback([&]{ this->cmd_export_json(file, out_file); });

    // import (json)
    std::string in_file;
    auto* import_cmd = app.add_subcommand("import", "Import project from JSON");
    import_cmd->add_option("--in", in_file, "Input JSON file")->required();
    import_cmd->callback([&]{ this->cmd_import_json(file, in_file); });

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }
    return 0;
}

int CLIHandler::cmd_create(const std::string& name, const std::string& description, const std::string& file) {
    core::Project p{name};
    (void)description; // reserved for future metadata
    core::FileStorage storage;
    storage.save(p, file);
    spdlog::info("Created project '{}' -> {}", name, file);
    return 0;
}

int CLIHandler::cmd_list(const std::string& file) {
    core::FileStorage storage;
    try {
        auto p = storage.load(file);
        spdlog::info("Project: {}", p.name());
    } catch (const std::exception& e) {
        spdlog::error("Cannot read '{}': {}", file, e.what());
        return 1;
    }
    return 0;
}

int CLIHandler::cmd_add_task(const std::string& file, const std::string& title, const std::string& priority) {
    core::FileStorage storage;
    try {
        auto p = storage.load(file);
        auto id = p.add_task(title + " [" + priority + "]");
        storage.save(p, file);
        spdlog::info("Added task {} -> {}", id, file);
    } catch (const std::exception& e) {
        spdlog::error("Error adding task: {}", e.what());
        return 1;
    }
    return 0;
}

int CLIHandler::cmd_list_tasks(const std::string& file) {
    core::FileStorage storage;
    try {
        auto p = storage.load(file);
        for (const auto& t : p.tasks()) {
            spdlog::info("- [{}] {} {}", (t.done ? 'x' : ' '), t.id, t.title);
        }
    } catch (const std::exception& e) {
        spdlog::error("Error listing tasks: {}", e.what());
        return 1;
    }
    return 0;
}

int CLIHandler::cmd_update_task(const std::string& file, int task_id, const std::string& status) {
    core::FileStorage storage;
    try {
        auto p = storage.load(file);
        if (status == "completed") {
            if (!p.complete_task(task_id)) {
                spdlog::warn("Task {} not found", task_id);
            }
        } else if (status == "open") {
            // No 'reopen' in core API; skipping for brevity
            spdlog::warn("Reopening is not implemented. Skipping.");
        } else {
            spdlog::warn("Unknown status '{}'", status);
        }
        storage.save(p, file);
        spdlog::info("Updated tasks -> {}", file);
    } catch (const std::exception& e) {
        spdlog::error("Error updating task: {}", e.what());
        return 1;
    }
    return 0;
}

int CLIHandler::cmd_export_json(const std::string& file, const std::string& out_file) {
    std::error_code ec;
    std::filesystem::copy_file(file, out_file, std::filesystem::copy_options::overwrite_existing, ec);
    if (ec) {
        spdlog::error("Export failed: {}", ec.message());
        return 1;
    }
    spdlog::info("Exported '{}' -> '{}'", file, out_file);
    return 0;
}

int CLIHandler::cmd_import_json(const std::string& file, const std::string& in_file) {
    std::error_code ec;
    std::filesystem::copy_file(in_file, file, std::filesystem::copy_options::overwrite_existing, ec);
    if (ec) {
        spdlog::error("Import failed: {}", ec.message());
        return 1;
    }
    spdlog::info("Imported '{}' -> '{}'", in_file, file);
    return 0;
}

} // namespace app
