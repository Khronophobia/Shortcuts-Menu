cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()

set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(ShortcutsMenu VERSION 1.0.0)
option(SHORTCUTSMENU_DEBUG "Debug. Disable unlock checks and maybe other stuff" OFF)

add_library(${PROJECT_NAME} SHARED
    src/ShortcutsPopup.cpp
    src/Hooks.cpp
    src/LayerHooks.cpp
)

if (SHORTCUTSMENU_DEBUG)
    add_compile_definitions(SHORTCUTSMENU_DEBUG)
endif()

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

setup_geode_mod(${PROJECT_NAME})
