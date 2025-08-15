#pragma once
#include <string>
#include <vector>
#include <optional>
#include "task.h"

namespace core {
class Project {
public:
    explicit Project(std::string name = "Untitled");
    const std::string& name() const noexcept;
    void set_name(std::string n);

    int add_task(std::string title);
    bool complete_task(int id);
    std::optional<Task> get_task(int id) const;
    const std::vector<Task>& tasks() const noexcept;

    // For persistence layers to reconstruct from storage (use with care).
    void add_task_raw(int id, std::string title, bool done);

private:
    std::string m_name;
    std::vector<Task> m_tasks;
    int m_next_id{1};
};
} // namespace core
