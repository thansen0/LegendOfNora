add_library(assets STATIC
    asset_paths.cpp
    spritesheet.cpp
    ground.cpp
)

target_include_directories(assets PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(assets PUBLIC raylib)

if(MSVC)
    target_compile_options(assets PRIVATE /W4)
else()
    target_compile_options(assets PRIVATE -Wall -Wextra -Wpedantic)
endif()