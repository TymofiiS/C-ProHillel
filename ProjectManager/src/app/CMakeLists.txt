find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(CLI11 CONFIG REQUIRED)

add_executable(project_manager
    src/cli_handler.cpp
    src/main.cpp
)

target_include_directories(project_manager PRIVATE include)
target_link_libraries(project_manager PRIVATE
    core
    fmt::fmt
    spdlog::spdlog
    nlohmann_json::nlohmann_json
    CLI11::CLI11
)
target_compile_features(project_manager PRIVATE cxx_std_17)
