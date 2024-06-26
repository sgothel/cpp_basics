cmake_minimum_required (VERSION 3.13.0 FATAL_ERROR)
include(JaulibSetup.cmake)
JaulibPreset()

project (cpp_basics LANGUAGES C CXX)

if(${CMAKE_VERSION} VERSION_LESS "3.18.0")
    message(STATUS "CMake ${CMAKE_VERSION}, please consider to switch to CMake 3.18.0 or later")
else()
    message(STATUS "CMake ${CMAKE_VERSION} >= 3.18.0, good.")
endif()

set (THREADS_PREFER_PTHREAD_FLAG ON)
find_package (Threads REQUIRED)

# set(CMAKE_MESSAGE_LOG_LEVEL DEBUG)

if(NOT DEFINED DONT_USE_RTTI)
    set(DONT_USE_RTTI ON)
    message(STATUS "${PROJECT_NAME} DONT_USE_RTTI ${DONT_USE_RTTI} (cpp_basics default)")
else()
    message(STATUS "${PROJECT_NAME} DONT_USE_RTTI ${DONT_USE_RTTI} (user)")
endif()

JaulibSetup()

find_path (SYSTEM_USR_DIR "stdlib.h")
include_directories (${SYSTEM_USR_DIR})

add_subdirectory (src)
