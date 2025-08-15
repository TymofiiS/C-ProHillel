add_library(core
    src/project.cpp
    src/task.cpp
    src/storage.cpp
)
target_include_directories(core PUBLIC include)
target_compile_features(core PUBLIC cxx_std_17)
