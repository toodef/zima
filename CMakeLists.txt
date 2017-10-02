cmake_minimum_required(VERSION 2.8.11)
project(zima)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp image/parser/parser.cpp renderer/renderer.cpp renderer/renderer.hpp image/image.cpp)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

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

if (DEFINED ENV{GLEW_INCLUDE_DIR})
    include_directories($ENV{GLEW_INCLUDE_DIR})
else()
    message(ERROR " Set GLEW_INCLUDE_DIR environment variable")
endif()

if (DEFINED ENV{GLEW_LIBRARY_DIR})
    link_directories($ENV{GLEW_LIBRARY_DIR})
else()
    message(ERROR " Set GLEW_LIBRARY_DIR environment variable")
endif()

if (WIN32)
    if (DEFINED ENV{OPENEXR_ROOT})
        set(OPENEXR_INCLUDE_DIRS $ENV{OPENEXR_ROOT}/include/OpenEXR)
        set(OPENEXR_LIBRARY_DIRS $ENV{OPENEXR_ROOT}/lib)
    else()
        message(ERROR " Please set OPENEXR_ROOT environment variable")
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
        message(ERROR " Please set ZLIB_ROOT environment variable")
    endif()

    include_directories(${ZLIB_INCLUDE_DIRS})
    link_directories(${ZLIB_LIBRARY_DIRS})
elseif(UNIX)
    include_directories(/usr/include/OpenEXR)
endif()

add_subdirectory(gui)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
if (WIN32)
    target_link_libraries(${PROJECT_NAME} zlibstatic gui ${OPENGL_LIBRARIES} glew32 gdal_i Half IlmThread Iex IlmImf)
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} gdal gui ${OPENGL_LIBRARIES} ${GLEW_LIBRARY_DIR} Half IlmThread Iex IlmImf)
endif()
