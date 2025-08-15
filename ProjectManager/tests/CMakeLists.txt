find_package(GTest CONFIG REQUIRED)

add_executable(core_tests test_core.cpp)
target_link_libraries(core_tests PRIVATE core GTest::gtest GTest::gtest_main)
target_compile_features(core_tests PRIVATE cxx_std_17)
add_test(NAME core_tests COMMAND core_tests)
