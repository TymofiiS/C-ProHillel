#include "app/cli_handler.h"
#include "core/project.h"
#include "core/storage.h"

#include <spdlog/spdlog.h>
#include <CLI/CLI.hpp>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <system_error>

namespace {
constexpr const char* DEFAULT_FILE = "projects.json";
}

namespace app {

int CLIHandler::run(int argc, char** argv) {
    CLI::App app{"ProjectManager CLI"};
    app.require_subcommand(1);

    std::string file = DEFAULT_FILE;

    // helper: add --file to each subcommand so users can place it after the subcommand
    auto add_file_opt = [&](CLI::App* sub) {
        sub->add_option("-f,--file", file, "Storage JSON file (default: projects.json)");
    };

    int rc = 0; // collect callback results and return them

    // --- create ---
    std::string name, description;
    auto* create = app.add_subcommand("create", "Create a new project");
    create->add_option("--name", name, "Project name")->required();
    create->add_option("--description", description, "Project description");
    add_file_opt(create);
    create->callback([&]{ rc = this->cmd_create(name, description, file); });

    // --- list ---
    auto* list = app.add_subcommand("list", "List projects");
    add_file_opt(list);
    list->callback([&]{ rc = this->cmd_list(file); });

    // --- add-task ---
    std::string title, priority = "normal";
    auto* add_task = app.add_subcommand("add-task", "Add a task");
    add_task->add_option("--title", title, "Task title")->required();
    auto* opt_priority = add_task->add_option("--priority", priority, "Priority (low|normal|high)");
    add_file_opt(add_task);
    add_task->callback([&]{
        const bool priority_was_provided = opt_priority->count() > 0;
        rc = this->cmd_add_task(file, title, priority, priority_was_provided);
    });

    // --- list-tasks ---
    auto* list_tasks = app.add_subcommand("list-tasks", "List tasks");
    add_file_opt(list_tasks);
    list_tasks->callback([&]{ rc = this->cmd_list_tasks(file); });

    // --- update-task ---
    int task_id = 0; std::string status;
    auto* update_task = app.add_subcommand("update-task", "Update task status");
    update_task->add_option("--task-id", task_id, "Task id")->required();
    update_task->add_option("--status", status, "Status (completed|open)")->required();
    add_file_opt(update_task);
    update_task->callback([&]{ rc = this->cmd_update_task(file, task_id, status); });

    // --- export ---
    std::string out_file;
    auto* export_cmd = app.add_subcommand("export", "Export project to JSON");
    export_cmd->add_option("--out", out_file, "Output JSON file")->required();
    add_file_opt(export_cmd);
    export_cmd->callback([&]{ rc = this->cmd_export_json(file, out_file); });

    // --- import ---
    std::string in_file;
    auto* import_cmd = app.add_subcommand("import", "Import project from JSON");
    import_cmd->add_option("--in", in_file, "Input JSON file")->required();
    add_file_opt(import_cmd);
    import_cmd->callback([&]{ rc = this->cmd_import_json(file, in_file); });

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e); // CLI parse errors set non-zero automatically
    }
    return rc; // propagate our command result
}

// ================== private commands ==================

int CLIHandler::cmd_create(const std::string& name,
                           const std::string& description,
                           const std::string& file) {
    (void)description;
    core::Project p{name};
    core::FileStorage storage;

    std::filesystem::path fp{file};
    if (!fp.parent_path().empty())
        std::filesystem::create_directories(fp.parent_path());

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

int CLIHandler::cmd_add_task(const std::string& file,
                             const std::string& title,
                             const std::string& priority,
                             bool use_priority_suffix) {
    core::FileStorage storage;
    try {
        auto p = storage.load(file);

        std::string stored_title = title;
        if (use_priority_suffix) {
            stored_title += " [" + priority + "]";
        }

        auto id = p.add_task(stored_title);
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

int CLIHandler::cmd_update_task(const std::string& file,
                                int task_id,
                                const std::string& status) {
    core::FileStorage storage;
    try {
        auto p = storage.load(file);
        if (status == "completed") {
            if (!p.complete_task(task_id)) {
                spdlog::warn("Task {} not found", task_id);
            }
        } else if (status == "open") {
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

int CLIHandler::cmd_export_json(const std::string& file,
                                const std::string& out_file) {
    std::error_code ec;
    std::filesystem::create_directories(std::filesystem::path(out_file).parent_path(), ec);
    std::filesystem::copy_file(file, out_file,
        std::filesystem::copy_options::overwrite_existing, ec);
    if (ec) {
        spdlog::error("Export failed: {}", ec.message());
        return 1;
    }
    spdlog::info("Exported '{}' -> '{}'", file, out_file);
    return 0;
}

int CLIHandler::cmd_import_json(const std::string& file,
                                const std::string& in_file) {
    std::error_code ec;
    std::filesystem::create_directories(std::filesystem::path(file).parent_path(), ec);
    std::filesystem::copy_file(in_file, file,
        std::filesystem::copy_options::overwrite_existing, ec);
    if (ec) {
        spdlog::error("Import failed: {}", ec.message());
        return 1;
    }
    spdlog::info("Imported '{}' -> '{}'", in_file, file);
    return 0;
}

} // namespace app
