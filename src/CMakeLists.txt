add_subdirectory(assets)
add_subdirectory(player)
add_subdirectory(book)
add_subdirectory(entities)
add_subdirectory(levels)
add_subdirectory(scenes)

add_executable(LegendOfNora main.cpp)

set_target_properties(LegendOfNora PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

target_include_directories(LegendOfNora PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(LegendOfNora PRIVATE levels scenes players raylib)

add_custom_command(TARGET LegendOfNora POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
    COMMENT "Copying assets to build directory")

if(MSVC)
    target_compile_options(LegendOfNora PRIVATE /W4)
else()
    target_compile_options(LegendOfNora PRIVATE -Wall -Wextra -Wpedantic)
endif()
