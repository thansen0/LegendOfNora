add_library(scenes STATIC scene.cpp)

target_include_directories(scenes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(scenes PUBLIC assets raylib)

if(MSVC)
    target_compile_options(scenes PRIVATE /W4)
else()
    target_compile_options(scenes PRIVATE -Wall -Wextra -Wpedantic)
endif()