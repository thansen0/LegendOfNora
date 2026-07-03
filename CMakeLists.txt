cmake_minimum_required(VERSION 3.20)

project(LegendOfNora VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(USE_SDL3 "Build using the SDL3 backend" OFF)
if(USE_SDL3)
    set(PLATFORM "SDL")
endif()

# raylib disables JPEG support by default; enable it for .jpg backgrounds
set(CUSTOMIZE_BUILD ON CACHE BOOL "" FORCE)
set(SUPPORT_FILEFORMAT_JPG ON CACHE BOOL "" FORCE)

add_subdirectory(extern/raylib)
add_subdirectory(src)
