add_library(players STATIC nora.cpp)

target_include_directories(players PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(players PUBLIC assets raylib)

if(MSVC)
    target_compile_options(players PRIVATE /W4)
else()
    target_compile_options(players PRIVATE -Wall -Wextra -Wpedantic)
endif()