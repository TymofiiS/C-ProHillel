#include <gtest/gtest.h>
#include "core/project.h"

TEST(ProjectBasics, AddAndComplete) {
    core::Project p{"T"};
    auto id = p.add_task("X");
    ASSERT_EQ(p.tasks().size(), 1u);
    EXPECT_FALSE(p.tasks().front().done);
    EXPECT_TRUE(p.complete_task(id));
    EXPECT_TRUE(p.tasks().front().done);
}
