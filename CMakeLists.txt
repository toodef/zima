cmake_minimum_required(VERSION 2.8.11)
project(zima)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp image/parser/parser.cpp gui.cpp gui.hpp renderer/renderer.cpp renderer/renderer.hpp image/image.cpp gui_unit.cpp gui_unit.hpp)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets OpenGL)
include_directories(${Qt5_INCLUDES})
add_definitions(${Qt5_DEFINITIONS})
qt5_add_resources(RESOURCES resource.qrc)

set(CMAKE_CXX_FLAGS "${Qt5_EXECUTABLE_COMPILE_FLAGS}")

if (WIN32)
    INCLUDE_DIRECTORIES($ENV{GDAL_DIR}/include)
    link_directories($ENV{GDAL_DIR}/lib)
endif(WIN32)

if (UNIX)
    INCLUDE_DIRECTORIES(/usr/include/gdal)
    link_directories(/usr/lib)
endif(UNIX)

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

if (WIN32)
    if (DEFINED ENV{OPENEXR_ROOT})
        set(OPENEXR_INCLUDE_DIRS $ENV{OPENEXR_ROOT}/include/OpenEXR)
        set(OPENEXR_LIBRARY_DIRS $ENV{OPENEXR_ROOT}/lib)
    else()
        message(ERROR "Please set OPENEXR_ROOT environment variable")
    endif()

    include_directories(${OPENEXR_INCLUDE_DIRS})
    if (CMAKE_BUILD_TYPE STREQUAL Release)
        link_directories(${OPENEXR_LIBRARY_DIRS}/Release)
    elseif(CMAKE_BUILD_TYPE STREQUAL Debug)
        link_directories(${OPENEXR_LIBRARY_DIRS}/Debug)
    endif()

    if (DEFINED ENV{ZLIB_ROOT})
        set(ZLIB_INCLUDE_DIRS $ENV{ZLIB_ROOT}/include)
        set(ZLIB_LIBRARY_DIRS $ENV{ZLIB_ROOT}/lib)
    else()
        message(ERROR "Please set ZLIB_ROOT environment variable")
    endif()

    include_directories(${ZLIB_INCLUDE_DIRS})
    link_directories(${ZLIB_LIBRARY_DIRS})
elseif(UNIX)
    include_directories(/usr/include/OpenEXR)
endif()

add_executable(${PROJECT_NAME} WIN32 ${RESOURCES} ${SOURCE_FILES})
if (WIN32)
    target_link_libraries(${PROJECT_NAME} zlibstatic ${OPENGL_LIBRARIES} Qt5::Core Qt5::Widgets Qt5::OpenGL gdal_i Half IlmThread Iex IlmImf)
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} gdal ${OPENGL_LIBRARIES} Qt5::Core Qt5::Widgets Qt5::OpenGL Half IlmThread Iex IlmImf)
endif()
