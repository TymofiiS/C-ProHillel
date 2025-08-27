
cmake_minimum_required(VERSION 3.16)
project(QtFilesTableMinV2_Stats_Actions_Progress_FolderQml LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.2 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2)

add_executable(QtFilesTableMinV2_Stats_Actions_Progress_FolderQml
    src/main.cpp
    src/FileTableModel.h
    src/FileTableModel.cpp
    src/FileController.h
    src/FileController.cpp
    qml/qml.qrc
)

target_link_libraries(QtFilesTableMinV2_Stats_Actions_Progress_FolderQml
    PRIVATE Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick Qt6::QuickControls2
)

if (WIN32)
  set_property(TARGET QtFilesTableMinV2_Stats_Actions_Progress_FolderQml PROPERTY WIN32_EXECUTABLE TRUE)
endif()
