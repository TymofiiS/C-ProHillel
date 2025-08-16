#pragma once
#include <string>
#include <string_view>

namespace core {
struct Task {
    int id{};
    std::string title;
    bool done{false};
};
} // namespace core
