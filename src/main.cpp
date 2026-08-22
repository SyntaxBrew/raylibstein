#include <iostream>
#include "raylib.h"

int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;

int main() {
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