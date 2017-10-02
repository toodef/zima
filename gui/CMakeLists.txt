cmake_minimum_required(VERSION 2.8.11)
project(gui)

set(CMAKE_CXX_STANDARD 14)

option(QT_GUI_BACKEND "Qt gui framework" ON)

set(SOURCE_FILES src/gui_backend.hpp src/qt_backend/qt_backend.hpp src/qt_backend/qt_backend.cpp include/gui.hpp src/gui.cpp)
set(HEADRE_FILES src/gui_backend.hpp)

if (QT_GUI_BACKEND)
    set(CMAKE_AUTOMOC ON)

    find_package(Qt5 REQUIRED COMPONENTS Core Widgets OpenGL)
    include_directories(${Qt5_INCLUDES})
    add_definitions(${Qt5_DEFINITIONS})
    set(CMAKE_CXX_FLAGS "${Qt5_EXECUTABLE_COMPILE_FLAGS}")

    qt5_add_resources(RESOURCES src/qt_backend/resource.qrc)
endif()

add_library(${PROJECT_NAME} ${RESOURCES} ${SOURCE_FILES})

if (QT_GUI_BACKEND)
    target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets Qt5::OpenGL)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC include)
