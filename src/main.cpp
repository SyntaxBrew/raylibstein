#include <iostream>
#include "raylib.h"
#include "map.hpp"

int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;

int main() {
    //Map map{8, 8};
    //std::cout << map.in_bounds(1, 10) << "\n";

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Liminalstein");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();
    }

    std::cout << "Hello, Backrooms!\n";
    return 0;
}