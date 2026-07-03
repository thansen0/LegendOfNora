cmake_minimum_required(VERSION 3.20)

project(LegendOfNora VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(PLATFORM "SDL" CACHE STRING "" FORCE)

# raylib disables JPEG support by default; enable it for .jpg backgrounds
set(CUSTOMIZE_BUILD ON CACHE BOOL "" FORCE)
set(SUPPORT_FILEFORMAT_JPG ON CACHE BOOL "" FORCE)

add_subdirectory(extern/raylib)
add_subdirectory(src)
