add_library(book STATIC
    book.cpp
    book_spawner.cpp
)

target_include_directories(book PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(book PUBLIC assets raylib players)

if(MSVC)
    target_compile_options(book PRIVATE /W4)
else()
    target_compile_options(book PRIVATE -Wall -Wextra -Wpedantic)
endif()