add_library(levels STATIC firstlevel.cpp)

target_include_directories(levels PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(levels PUBLIC assets book players raylib)

if(MSVC)
    target_compile_options(levels PRIVATE /W4)
else()
    target_compile_options(levels PRIVATE -Wall -Wextra -Wpedantic)
endif()