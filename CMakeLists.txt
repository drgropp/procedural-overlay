cmake_minimum_required(VERSION 3.16)

project(procedural_overlay LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(RAYLIB_ROOT "C:/Users/michael flynn/Documents/raylib/raylib" CACHE PATH "Path to the raylib source tree")
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
add_subdirectory("${RAYLIB_ROOT}" "${CMAKE_BINARY_DIR}/raylib" EXCLUDE_FROM_ALL)

add_executable(procedural_overlay WIN32
    src/main.c
    src/effects.c
)

target_link_libraries(procedural_overlay PRIVATE raylib)

if(MINGW)
    target_compile_options(procedural_overlay PRIVATE -Wall -Wextra -Wpedantic)
endif()

include(CTest)
if(BUILD_TESTING)
    add_executable(palette_tests
        tests/palette_tests.c
        src/effects.c
    )
    target_include_directories(palette_tests PRIVATE src)
    if(NOT MSVC)
        target_link_libraries(palette_tests PRIVATE m)
    endif()
    if(MINGW)
        target_compile_options(palette_tests PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    add_test(NAME palette_commands_and_colors COMMAND palette_tests)
endif()
