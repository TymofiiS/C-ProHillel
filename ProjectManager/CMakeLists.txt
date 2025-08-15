cmake_minimum_required(VERSION 3.15)
project(ProjectManager LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Subprojects
add_subdirectory(src/core)
add_subdirectory(src/app)
add_subdirectory(examples)
