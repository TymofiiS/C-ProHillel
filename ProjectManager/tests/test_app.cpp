
#include <gtest/gtest.h>
#include "app/cli_handler.h"
#include "core/storage.h"
#include <filesystem>
#include <fstream>
#include <random>
#include <memory>
#include <cstring>

namespace fs = std::filesystem;

static fs::path temp_dir(const char* hint) {
    auto base = fs::temp_directory_path() / ("pm_app_tests_" + std::string(hint) + "_" + std::to_string(::testing::UnitTest::GetInstance()->random_seed()));
    fs::create_directories(base);
    return base;
}

static int run_cli(const std::vector<std::string>& args) {
    // Build argv
    std::vector<std::unique_ptr<char[]>> storage;
    std::vector<char*> argv;
    storage.reserve(args.size()+1);
    argv.reserve(args.size()+1);

    for (const auto& s : args) {
        auto buf = std::make_unique<char[]>(s.size()+1);
        std::memcpy(buf.get(), s.c_str(), s.size()+1);
        argv.push_back(buf.get());
        storage.push_back(std::move(buf));
    }
    argv.push_back(nullptr); // safety

    app::CLIHandler cli;
    return cli.run(static_cast<int>(args.size()), argv.data());
}

static core::Project load_proj(const fs::path& file) {
    core::FileStorage s;
    return s.load(file.string());
}

TEST(AppCLI, CreateProjectAndList) {
    auto dir = temp_dir("create");
    auto file = dir / "projects.json";
    int rc = run_cli({"project_manager", "create", "--name", "My Project", "--description", "Desc", "--file", file.string()});
    EXPECT_EQ(rc, 0);
    ASSERT_TRUE(fs::exists(file));

    // list should succeed
    rc = run_cli({"project_manager", "list", "--file", file.string()});
    EXPECT_EQ(rc, 0);

    auto p = load_proj(file);
    EXPECT_EQ(p.name(), "My Project");
    EXPECT_TRUE(p.tasks().empty());
}

TEST(AppCLI, ListMissingFileFails) {
    auto dir = temp_dir("list_missing");
    auto file = dir / "nope.json";
    int rc = run_cli({"project_manager", "list", "--file", file.string()});
    EXPECT_NE(rc, 0);
}

TEST(AppCLI, AddTaskAndListTasks) {
    auto dir = temp_dir("add_task");
    auto file = dir / "projects.json";

    // create
    EXPECT_EQ(run_cli({"project_manager", "create", "--name", "Demo", "--file", file.string()}), 0);
    // add
    EXPECT_EQ(run_cli({"project_manager", "add-task", "--title", "Task 1", "--priority", "high", "--file", file.string()}), 0);
    // list-tasks
    EXPECT_EQ(run_cli({"project_manager", "list-tasks", "--file", file.string()}), 0);

    auto p = load_proj(file);
    ASSERT_EQ(p.tasks().size(), 1u);
    EXPECT_EQ(p.tasks()[0].id, 1);
    EXPECT_NE(p.tasks()[0].title.find("Task 1"), std::string::npos);
    EXPECT_NE(p.tasks()[0].title.find("[high]"), std::string::npos);
    EXPECT_FALSE(p.tasks()[0].done);
}

TEST(AppCLI, UpdateTaskCompleted) {
    auto dir = temp_dir("update_task");
    auto file = dir / "projects.json";
    ASSERT_EQ(run_cli({"project_manager", "create", "--name", "U", "--file", file.string()}), 0);
    ASSERT_EQ(run_cli({"project_manager", "add-task", "--title", "T", "--file", file.string()}), 0);

    int rc = run_cli({"project_manager", "update-task", "--task-id", "1", "--status", "completed", "--file", file.string()});
    EXPECT_EQ(rc, 0);

    auto p = load_proj(file);
    ASSERT_EQ(p.tasks().size(), 1u);
    EXPECT_TRUE(p.tasks()[0].done);
}

TEST(AppCLI, UpdateTaskUnknownStatusWarnsButReturns0) {
    auto dir = temp_dir("update_unknown");
    auto file = dir / "projects.json";
    ASSERT_EQ(run_cli({"project_manager", "create", "--name", "U", "--file", file.string()}), 0);
    ASSERT_EQ(run_cli({"project_manager", "add-task", "--title", "T", "--file", file.string()}), 0);

    int rc = run_cli({"project_manager", "update-task", "--task-id", "1", "--status", "weird", "--file", file.string()});
    EXPECT_EQ(rc, 0); // current implementation warns only

    auto p = load_proj(file);
    ASSERT_EQ(p.tasks().size(), 1u);
    EXPECT_FALSE(p.tasks()[0].done); // unchanged
}

TEST(AppCLI, ExportAndImport) {
    auto dir = temp_dir("export_import");
    auto file = dir / "projects.json";
    auto out  = dir / "export.json";
    ASSERT_EQ(run_cli({"project_manager", "create", "--name", "E", "--file", file.string()}), 0);
    ASSERT_EQ(run_cli({"project_manager", "add-task", "--title", "T1", "--file", file.string()}), 0);

    EXPECT_EQ(run_cli({"project_manager", "export", "--out", out.string(), "--file", file.string()}), 0);
    ASSERT_TRUE(fs::exists(out));

    // Overwrite main file with different content, then import back
    std::ofstream ofs(file);
    ofs << R"({"name":"Other","tasks":[{"id":1,"title":"X","done":true}]})";
    ofs.close();

    EXPECT_EQ(run_cli({"project_manager", "import", "--in", out.string(), "--file", file.string()}), 0);
    auto p = load_proj(file);
    EXPECT_EQ(p.name(), "E");
    ASSERT_EQ(p.tasks().size(), 1u);
    EXPECT_EQ(p.tasks()[0].title, "T1");
}

TEST(AppCLI, MissingRequiredOptionCausesNonZeroExit) {
    // "create" without --name should fail parse
    int rc = run_cli({"project_manager", "create"});
    EXPECT_NE(rc, 0);
}

TEST(AppCLI, AddTaskOnMissingFileFails) {
    auto dir = temp_dir("add_missing");
    auto file = dir / "projects.json";
    int rc = run_cli({"project_manager", "add-task", "--title", "X", "--file", file.string()});
    EXPECT_NE(rc, 0);
}
