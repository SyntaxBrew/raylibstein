cmake_minimum_required(VERSION 3.20)
project(Liminalstein CXX) # CXX = C++

set(CMAKE_CXX_STANDARD 20) # C++ 20
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Enforces C++ standard

# Built-in tool for directly downloading external libraries directly inside the build folder at run-time
include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib
    GIT_TAG 6.0
)

# Raylib will install itself inside the build directory
FetchContent_MakeAvailable(raylib) 

add_executable(Liminalstein
    src/main.cpp
    src/game.cpp
    src/map.cpp
)

# PRIVATE = only exposes stuff to your files
target_include_directories(Liminalstein PRIVATE include) 
target_link_libraries(Liminalstein PRIVATE raylib)

if (EMSCRIPTEN)
    set_target_properties(Liminalstein PROPERTIES SUFFIX ".html")
    target_link_options(Liminalstein PRIVATE
        "SHELL:-s USE_GLFW=3"
        "SHELL:-s GL_ENABLE_GET_PROC_ADDRESS=1"
        "SHELL:-s ALLOW_MEMORY_GROWTH=1"
        --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/minshell.html
        --embed-file ${CMAKE_CURRENT_SOURCE_DIR}/assets@assets
    )
endif()