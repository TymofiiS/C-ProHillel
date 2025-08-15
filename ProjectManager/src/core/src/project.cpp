#include "core/project.h"
#include <algorithm>
#include <utility>

namespace core {

Project::Project(std::string name) : m_name(std::move(name)) {}

const std::string& Project::name() const noexcept { return m_name; }
void Project::set_name(std::string n) { m_name = std::move(n); }

int Project::add_task(std::string title) {
    Task t;
    t.id = m_next_id++;
    t.title = std::move(title);
    m_tasks.push_back(t);
    return t.id;
}

void Project::add_task_raw(int id, std::string title, bool done) {
    Task t;
    t.id = id;
    t.title = std::move(title);
    t.done = done;
    m_tasks.push_back(std::move(t));
    if (id >= m_next_id) m_next_id = id + 1;
}

bool Project::complete_task(int id) {
    for (auto& t : m_tasks) {
        if (t.id == id) { t.done = true; return true; }
    }
    return false;
}

std::optional<Task> Project::get_task(int id) const {
    for (const auto& t : m_tasks) {
        if (t.id == id) return t;
    }
    return std::nullopt;
}

const std::vector<Task>& Project::tasks() const noexcept { return m_tasks; }

} // namespace core
