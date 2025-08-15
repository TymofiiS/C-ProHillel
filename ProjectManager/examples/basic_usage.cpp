#include "core/project.h"
#include <iostream>

int main() {
    core::Project p{"Example"};
    auto id1 = p.add_task("Write docs");
    auto id2 = p.add_task("Ship 1.0");
    p.complete_task(id1);

    std::cout << "Project: " << p.name() << "\n";
    for (const auto& t : p.tasks()) {
        std::cout << "- [" << (t.done ? 'x' : ' ') << "] " << t.id << " " << t.title << "\n";
    }
    return 0;
}
