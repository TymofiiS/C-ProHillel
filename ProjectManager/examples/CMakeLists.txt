add_executable(basic_usage basic_usage.cpp)
target_link_libraries(basic_usage PRIVATE core)
target_include_directories(basic_usage PRIVATE ${CMAKE_SOURCE_DIR}/src/core/include)
target_compile_features(basic_usage PRIVATE cxx_std_17)
