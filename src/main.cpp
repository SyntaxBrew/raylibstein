#include <iostream>
#include <numbers>

#include "raylib.h"
#include "map.hpp"

int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;

float DegreesToRads(float degrees) {
    return degrees * (std::numbers::pi / 180.0f);
}

Vector2 AngleToVector2(float angle) {
    return {cosf(DegreesToRads(angle)), sinf(DegreesToRads(angle))};
}

int main() {
    Map map{8, 8};
 
    //std::cout << map.in_bounds(1, 10) << "\n";

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Liminalstein");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();
    }

    RaycastResult result = map.CastRay({1.5, 1.5}, AngleToVector2(60));
    std::cout << result.distance << "\n";
    std::cout << result.direction.x << "," << result.direction.y << "\n";

    std::cout << "Hello, Backrooms!\n";
    return 0;
}