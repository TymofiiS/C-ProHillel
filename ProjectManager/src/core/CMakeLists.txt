find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_library(core
    src/project.cpp
    src/task.cpp
    src/storage.cpp
)
target_include_directories(core PUBLIC include)
target_compile_features(core PUBLIC cxx_std_17)
target_link_libraries(core
    PRIVATE
        nlohmann_json::nlohmann_json
        spdlog::spdlog
)
