cmake_minimum_required(VERSION 2.8.11)
project(tif_viewer)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp image/parser/parser.cpp gui.cpp gui.hpp renderer/renderer.cpp renderer/renderer.hpp image/image.cpp gui_unit.cpp gui_unit.hpp)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets OpenGL)
include_directories(${Qt5_INCLUDES})
add_definitions(${Qt5_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${Qt5_EXECUTABLE_COMPILE_FLAGS}")

INCLUDE_DIRECTORIES(D:/libs/gdalwin32-1.4.1/include/)
link_directories(D:/libs/gdal-220/lib)

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

#qt5_use_modules(${PROJECT_NAME} Widgets Core OpenGL)

target_link_libraries(${PROJECT_NAME} gdal_i Qt5::Core Qt5::Widgets Qt5::OpenGL)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
