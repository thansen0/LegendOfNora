add_subdirectory(levels)

add_executable(LegendOfNora main.cpp)

target_include_directories(LegendOfNora PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(LegendOfNora PRIVATE levels raylib)

add_custom_command(TARGET LegendOfNora POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
    COMMENT "Copying assets to build directory")

if(MSVC)
    target_compile_options(LegendOfNora PRIVATE /W4)
else()
    target_compile_options(LegendOfNora PRIVATE -Wall -Wextra -Wpedantic)
endif()