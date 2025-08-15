find_package(GTest CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(CLI11 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

# Core tests
add_executable(core_tests test_core.cpp)
target_link_libraries(core_tests PRIVATE core GTest::gtest GTest::gtest_main)
target_compile_features(core_tests PRIVATE cxx_std_17)
add_test(NAME core_tests COMMAND core_tests)

# App CLI tests: compile with the CLI sources (exclude app main)
add_executable(app_tests test_app.cpp
    ${CMAKE_SOURCE_DIR}/src/app/src/cli_handler.cpp
)
target_include_directories(app_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src/app/include
    ${CMAKE_SOURCE_DIR}/src/core/include
)
target_link_libraries(app_tests PRIVATE
    core
    GTest::gtest GTest::gtest_main
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    CLI11::CLI11
    fmt::fmt
)
target_compile_features(app_tests PRIVATE cxx_std_17)
add_test(NAME app_cli_tests COMMAND app_tests)
