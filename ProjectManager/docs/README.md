# ProjectManager — Multi‑module Project Management System (CMake + Conan 2)

**ProjectManager** is a small, didactic codebase that demonstrates how to build a multi‑module C++ project with
**modern CMake (≥ 3.20)** and **Conan 2**. It focuses on dependency management, testing, logging, command‑line UX,
and (optionally) packaging. The system consists of:

- **core** — a static library that models projects, tasks, and storage.
- **app** — a CLI executable that integrates with `core` and provides a user‑friendly command line interface.

> Default storage is a simple JSON file; an optional SQLite backend can be added later.

---

## Features

- Clean, layered layout (`core` library + `app` executable + `examples`).
- Modern CMake targets and Conan‑managed dependencies.
- JSON import/export and simple persistence (default JSON; optional SQLite).
- Logging via **spdlog**.
- Command‑line parsing via **CLI11**.
- Unit tests via **GoogleTest**.
- Ready for packaging/exporting CMake targets (install/export stubs provided below).

---

## Project Layout

```
ProjectManager/
├── conanfile.txt
├── CMakeLists.txt
├── src/
│   ├── core/                    # Core library
│   │   ├── CMakeLists.txt
│   │   ├── include/
│   │   │   └── core/
│   │   │       ├── project.h
│   │   │       ├── task.h
│   │   │       └── storage.h
│   │   └── src/
│   │       ├── project.cpp
│   │       ├── task.cpp
│   │       └── storage.cpp
│   └── app/                     # CLI app
│       ├── CMakeLists.txt
│       ├── include/
│       │   └── app/
│       │       └── cli_handler.h
│       └── src/
│           ├── cli_handler.cpp
│           └── main.cpp
├── examples/
│   ├── CMakeLists.txt
│   └── basic_usage.cpp
└── docs/
    └── README.md
```

---

## Dependencies (via Conan 2)

**Required**
- `nlohmann_json` — JSON serialization.
- `spdlog` — logging.
- `cli11` — command‑line parsing.
- `gtest` — unit testing.
- `fmt` — string formatting (also used by spdlog).

**Optional**
- `sqlite3` — persistent storage backend instead of files.
- `boost` — extra utilities if you need them.

### Example `conanfile.txt`

```ini
[requires]
nlohmann_json/3.11.3
spdlog/1.14.1
cli11/2.4.2
gtest/1.14.0
fmt/11.0.2

[generators]
CMakeDeps
CMakeToolchain

[tool_requires]
cmake/3.29.3

[options]
spdlog:header_only=False
spdlog:fmt_external=True
fmt:header_only=False
```

> Add `sqlite3/…` and/or `boost/…` to `[requires]` if you plan to use them.

---

## Build Instructions

### Prerequisites
- **Conan 2.x**
- **CMake ≥ 3.20**
- A C++17 (or newer) compiler (MSVC, Clang, GCC)

### Configure & Build (recommended sequence)
From the project root:
```bash
conan install . -of build -s build_type=Release --build=missing
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Run:
```bash
# Windows
.build\Release\project_manager.exe

# Linux/macOS
./build/project_manager
```

### Using CTest
If tests are added under `tests/` and registered with CTest:
```bash
ctest -C Release --output-on-failure
```

---

## Usage (CLI)

The CLI is designed around a few clear subcommands. Examples below illustrate the expected UX:

```bash
# Create a project
./project_manager create --name "My Project" --description "Test project"

# List projects
./project_manager list

# Add a task
./project_manager add-task --project-id 1 --title "Task 1" --priority high

# List tasks of a project
./project_manager list-tasks --project-id 1

# Update a task
./project_manager update-task --task-id 1 --status completed

# Export data (JSON)
./project_manager export --project-id 1 --format json

# Import data (JSON)
./project_manager import --file projects.json
```

The default storage is JSON files (using `nlohmann_json`). If compiled with the SQLite option, the same
commands operate on a database file instead.

---

## Architecture & Implementation Notes

### `core` (static library)
- **Classes:** `Project`, `Task`, `Storage` (e.g., `FileStorage` for JSON; optional `SqliteStorage`)
- **Responsibilities:**
  - Project & task lifecycle (create/list/update).
  - JSON serialization/deserialization.
  - Logging (via `spdlog`) for major operations and errors.

**Public headers:** `src/core/include/core/*.h`  
**Sources:** `src/core/src/*.cpp`

### `app` (CLI executable)
- Parses command‑line arguments with **CLI11**.
- Calls into `core` to perform actions.
- Validates inputs and reports errors gracefully.
- Logs helpful context for each operation.

### Examples
- `examples/basic_usage.cpp` shows direct library usage without the CLI.

---

## CMake: Targets & Linking

The project uses modern target‑based linking. Conan’s **CMakeDeps** generator provides package configs that you
can consume with `find_package(NAME CONFIG REQUIRED)` then link the imported targets, e.g.:

```cmake
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(CLI11 CONFIG REQUIRED)
# find_package(unofficial-sqlite3 CONFIG REQUIRED)  # if you add sqlite3

target_link_libraries(project_manager
    PRIVATE
        core
        spdlog::spdlog
        fmt::fmt
        nlohmann_json::nlohmann_json
        CLI11::CLI11
)
```

> Conan’s toolchain file sets up search paths; always configure CMake with
> `-DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake` after `conan install`.

---

## Logging

`spdlog` is used system‑wide. By default, logs go to the console. You can create a combined console+file logger at
program start:

```cpp
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
auto file    = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/app.log", true);
auto logger  = std::make_shared<spdlog::logger>("pm", spdlog::sinks_init_list{console, file});
spdlog::set_default_logger(logger);
spdlog::set_level(spdlog::level::info);
```

---

## Testing

Unit tests use **GoogleTest**. A minimal test layout:

```
tests/
├── CMakeLists.txt
└── test_core.cpp
```

Example CMake glue:

```cmake
enable_testing()
add_executable(tests tests/test_core.cpp)
target_link_libraries(tests PRIVATE core GTest::gtest GTest::gtest_main)
add_test(NAME core_tests COMMAND tests)
```

Run:
```bash
ctest -C Release --output-on-failure
```

---

## Packaging & Exported Targets (optional)

To make the `core` library consumable by other projects, add install and export rules:

```cmake
# In src/core/CMakeLists.txt
add_library(core ...)
target_include_directories(core PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                                   $<INSTALL_INTERFACE:include>)

install(TARGETS core
    EXPORT ProjectManagerTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY include/ DESTINATION include)

# At top-level CMakeLists.txt
install(EXPORT ProjectManagerTargets
    FILE ProjectManagerTargets.cmake
    NAMESPACE ProjectManager::
    DESTINATION lib/cmake/ProjectManager
)
```

This produces exported, namespaced targets like `ProjectManager::core` for downstream CMake consumers.

---

## License

MIT.
