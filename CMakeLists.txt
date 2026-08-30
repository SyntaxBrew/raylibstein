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
    src/player.cpp
)

# PRIVATE = only exposes stuff to your files
target_include_directories(Liminalstein PRIVATE include) 
target_link_libraries(Liminalstein PRIVATE raylib)