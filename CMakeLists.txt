cmake_minimum_required(VERSION 3.20)

project(LegendOfNora VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# enable SDL3
set(PLATFORM "SDL" CACHE STRING "" FORCE)

add_subdirectory(extern/raylib)

add_executable(LegendOfNora
    src/main.cpp
)

target_link_libraries(LegendOfNora PRIVATE raylib)

if(MSVC)
    target_compile_options(LegendOfNora PRIVATE /W4)
else()
    target_compile_options(LegendOfNora PRIVATE -Wall -Wextra -Wpedantic)
endif()

