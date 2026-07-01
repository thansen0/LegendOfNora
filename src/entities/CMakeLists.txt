add_library(entities STATIC
    angry_chimp.cpp
    angry_chimp_spawner.cpp
    gorilla.cpp
    sliding_chimp.cpp
    sliding_chimp_spawner.cpp
)

target_include_directories(entities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(entities PUBLIC assets raylib)

if(MSVC)
    target_compile_options(entities PRIVATE /W4)
else()
    target_compile_options(entities PRIVATE -Wall -Wextra -Wpedantic)
endif()