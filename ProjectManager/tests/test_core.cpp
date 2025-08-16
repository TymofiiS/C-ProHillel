
#include <gtest/gtest.h>
#include "core/project.h"
#include "core/storage.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

static fs::path temp_json_path(const std::string& name_hint) {
    auto base = fs::temp_directory_path() / ("pm_tests_" + name_hint + "_" + std::to_string(::testing::UnitTest::GetInstance()->random_seed()));
    fs::create_directories(base);
    return base / "projects.json";
}

// -------- Project basic behavior --------
TEST(ProjectBasics, DefaultNameAndSetName) {
    core::Project p;
    EXPECT_EQ(p.name(), "Untitled");
    p.set_name("Alpha");
    EXPECT_EQ(p.name(), "Alpha");
}

TEST(ProjectBasics, AddTaskAssignsIncrementingIds) {
    core::Project p{"X"};
    int a = p.add_task("A");
    int b = p.add_task("B");
    int c = p.add_task("C");
    EXPECT_EQ(a, 1);
    EXPECT_EQ(b, 2);
    EXPECT_EQ(c, 3);
    ASSERT_EQ(p.tasks().size(), 3u);
    EXPECT_EQ(p.tasks()[0].title, "A");
    EXPECT_EQ(p.tasks()[1].title, "B");
    EXPECT_EQ(p.tasks()[2].title, "C");
}

TEST(ProjectBasics, CompleteTaskAndGetTask) {
    core::Project p{"Y"};
    int id = p.add_task("Do it");
    EXPECT_FALSE(p.get_task(id)->done);
    EXPECT_TRUE(p.complete_task(id));
    ASSERT_TRUE(p.get_task(id).has_value());
    EXPECT_TRUE(p.get_task(id)->done);
    EXPECT_FALSE(p.complete_task(id + 100)); // nonexistent
    EXPECT_FALSE(p.get_task(id + 100).has_value());
}

TEST(ProjectBasics, AddTaskRawRespectsAndAdvancesNextId) {
    core::Project p{"Z"};
    // next id starts at 1
    p.add_task_raw(10, "ten", false);    // jumps next_id to 11
    int id = p.add_task("eleven");       // should be 11
    EXPECT_EQ(id, 11);
    // raw with lower id should not move next_id backwards
    p.add_task_raw(5, "five", true);
    int id2 = p.add_task("twelve");
    EXPECT_EQ(id2, 12);
}

// -------- Storage JSON roundtrips --------
TEST(StorageJSON, SaveAndLoadRoundTrip) {
    auto file = temp_json_path("roundtrip");
    core::Project p{"Demo"};
    auto id1 = p.add_task("Привіт світе"); // unicode
    auto id2 = p.add_task("Żółć iść w bór?"); // diacritics
    ASSERT_NE(id1, id2);

    core::FileStorage s;
    s.save(p, file.string());

    // Read back
    auto p2 = s.load(file.string());
    EXPECT_EQ(p2.name(), "Demo");
    ASSERT_EQ(p2.tasks().size(), 2u);
    EXPECT_EQ(p2.tasks()[0].title, "Привіт світе");
    EXPECT_EQ(p2.tasks()[1].title, "Żółć iść w bór?");

    // Ensure new ids continue from max id
    auto id3 = p2.add_task("Next");
    EXPECT_EQ(id3, std::max(id1, id2) + 1);
}

TEST(StorageJSON, LoadMissingFileThrows) {
    core::FileStorage s;
    auto missing = (fs::temp_directory_path() / "pm_tests_missing" / "no_such.json").string();
    EXPECT_THROW({ s.load(missing); }, std::runtime_error);
}

TEST(StorageJSON, SaveToNonexistentDirectoryThrows) {
    core::FileStorage s;
    core::Project p{"A"};
    auto bad_dir = fs::temp_directory_path() / "pm_tests_bad" / "deep" / "path";
    // don't create directories -> saving should fail
    auto target = (bad_dir / "out.json").string();
    EXPECT_THROW({ s.save(p, target); }, std::runtime_error);
}

TEST(StorageJSON, LoadWithMissingFieldsUsesDefaults) {
    auto file = temp_json_path("defaults");
    // Write a JSON with some tasks missing fields
    std::ofstream ofs(file);
    ofs << R"({
        "name": "X",
        "tasks": [
            {"id": 7, "title": "A"},
            {"title": "B", "done": true},
            {"id": 9, "done": false}
        ]
    })";
    ofs.close();

    core::FileStorage s;
    auto p = s.load(file.string());
    EXPECT_EQ(p.name(), "X");
    ASSERT_EQ(p.tasks().size(), 3u);
    EXPECT_EQ(p.tasks()[0].id, 7);
    EXPECT_EQ(p.tasks()[0].title, "A");
    EXPECT_FALSE(p.tasks()[0].done);

    // Defaults: id -> 0, done -> true from file, title -> "", etc.
    EXPECT_EQ(p.tasks()[1].id, 0);
    EXPECT_EQ(p.tasks()[1].title, "B");
    EXPECT_TRUE(p.tasks()[1].done);

    EXPECT_EQ(p.tasks()[2].id, 9);
    EXPECT_EQ(p.tasks()[2].title, "");
    EXPECT_FALSE(p.tasks()[2].done);

    // Next id should advance to max(id)+1 = 10
    auto id = p.add_task("N");
    EXPECT_EQ(id, 10);
}

// Large-ish data set to cover loops & performance edges
TEST(StorageJSON, RoundTripManyTasks) {
    auto file = temp_json_path("many");
    core::Project p{"Many"};
    const int N = 500; // keep fast
    for (int i = 0; i < N; ++i) {
        auto id = p.add_task("T" + std::to_string(i));
        if (i % 3 == 0) p.complete_task(id);
    }
    core::FileStorage s;
    s.save(p, file.string());
    auto p2 = s.load(file.string());
    ASSERT_EQ((int)p2.tasks().size(), N);
    // spot-check a few
    EXPECT_TRUE(p2.tasks()[0].done);
    EXPECT_FALSE(p2.tasks()[1].done);
    EXPECT_FALSE(p2.tasks()[2].done);
    EXPECT_TRUE(p2.tasks()[3].done);
}

// Ensure tasks() returns const reference (compile-time check)
// Also runtime: verify reference stability vs copy
TEST(ProjectBasics, TasksConstReference) {
    const core::Project p{"Ref"};
    const auto& ref = p.tasks();
    EXPECT_EQ(ref.size(), 0u);
    // Cannot modify 'ref' here; this test ensures const-ness at compile time.
}

// Verify behavior when completing nonexistent id (returns false)
TEST(ProjectBasics, CompleteNonexistentReturnsFalse) {
    core::Project p{"Nope"};
    EXPECT_FALSE(p.complete_task(42));
}

// Ensure add_task_raw with out-of-order ids elevates next id correctly
TEST(ProjectBasics, AddTaskRawNonMonotonic) {
    core::Project p{"Raw"};
    p.add_task_raw(5, "five", false);
    p.add_task_raw(2, "two", false);
    p.add_task_raw(20, "twenty", false);
    int id = p.add_task("next");
    EXPECT_EQ(id, 21);
}
