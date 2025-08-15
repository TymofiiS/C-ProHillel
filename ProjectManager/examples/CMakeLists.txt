# examples/CMakeLists.txt
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(basic_usage basic_usage.cpp)
target_link_libraries(basic_usage PRIVATE
    core
    spdlog::spdlog
    nlohmann_json::nlohmann_json
)
target_compile_features(basic_usage PRIVATE cxx_std_17)
