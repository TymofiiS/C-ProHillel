// examples/basic_usage.cpp
// A user-facing example that shows how to use the core library
// and how to initialize/use the logger (spdlog).
// It also prints a CLI "cheat sheet" demonstrating the expected commands.

#include "core/project.h"
#include "core/storage.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

// Set up a default logger that logs to both console and a file.
static void setup_logger() {
    auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file    = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/example.log", true);
    auto logger  = std::make_shared<spdlog::logger>("pm", spdlog::sinks_init_list{console, file});
    spdlog::set_default_logger(logger);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
    spdlog::set_level(spdlog::level::info);
}

static void print_cli_cheatsheet() {
    std::cout << "\n=== CLI quick start ===\n"
              << "./project_manager create --name \"My Project\" --description \"Test project\"\n"
              << "./project_manager list\n"
              << "./project_manager add-task --project-id 1 --title \"Task 1\" --priority high\n"
              << "./project_manager list-tasks --project-id 1\n"
              << "./project_manager update-task --task-id 1 --status completed\n"
              << "./project_manager export --project-id 1 --format json\n"
              << "./project_manager import --file projects.json\n"
              << "=======================\n\n";
}

int main() {
    setup_logger();
    spdlog::info("=== ProjectManager basic usage ===");

    // Show the exact commands the user expects to see.
    print_cli_cheatsheet();

    // Choose a storage file (in the working directory).
    const fs::path storage_file = "example_projects.json";

    try {
        // 1) Create a project and add tasks
        core::Project project{"Sample Project"};
        int id1 = project.add_task("Explore the CLI app");
        int id2 = project.add_task("Write documentation");
        spdlog::info("Created project '{}' with tasks: {}, {}", project.name(), id1, id2);

        // 2) Persist to JSON via FileStorage
        core::FileStorage storage;
        storage.save(project, storage_file.string());
        spdlog::info("Saved project to '{}'", storage_file.string());

        // 3) Load back and print
        auto loaded = storage.load(storage_file.string());
        spdlog::info("Loaded project: '{}'", loaded.name());
        for (const auto& t : loaded.tasks()) {
            spdlog::info("- [{}] {} {}", (t.done ? 'x' : ' '), t.id, t.title);
        }

        // 4) Update a task, save again
        loaded.complete_task(id1);
        storage.save(loaded, storage_file.string());
        spdlog::info("Marked task {} as completed and saved again.", id1);

        std::cout << "Example finished. See '" << storage_file.string()
                  << "' and 'logs/example.log' for output.\n";
        return 0;
    } catch (const std::exception& e) {
        spdlog::error("Example failed: {}", e.what());
        return 1;
    }
}
