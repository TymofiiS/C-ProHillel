#include "app/cli_handler.h"
#include "core/project.h"
#include <spdlog/spdlog.h>
#include <filesystem>

namespace app {
int CLIHandler::run(int argc, char** argv) {
    (void)argc; (void)argv;
    spdlog::info("Current directory: {}", std::filesystem::current_path().string());

    core::Project project("Demo");
    auto id = project.add_task("Try ProjectManager");
    spdlog::info("Created project '{}' with first task id {}", project.name(), id);
    return 0;
}
} // namespace app
