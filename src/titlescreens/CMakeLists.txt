add_library(titlescreens STATIC title_screen.cpp)

target_include_directories(titlescreens PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(titlescreens PUBLIC assets raylib)

if(MSVC)
    target_compile_options(titlescreens PRIVATE /W4)
else()
    target_compile_options(titlescreens PRIVATE -Wall -Wextra -Wpedantic)
endif()