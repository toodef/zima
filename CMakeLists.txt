cmake_minimum_required(VERSION 2.8.11)
project(tif_viewer)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp image/parser/parser.cpp gui.cpp gui.hpp)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

include_directories(${Qt5_INCLUDES})
add_definitions(${Qt5_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${Qt5_EXECUTABLE_COMPILE_FLAGS}")

INCLUDE_DIRECTORIES(D:/libs/Qt/5.8/mingw53_32/include/)

INCLUDE_DIRECTORIES(D:/libs/gdalwin32-1.4.1/include/)
link_directories(D:/libs/gdalwin32-1.4.1/lib)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} gdal_i Qt5::Widgets)
