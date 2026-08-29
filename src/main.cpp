#include <iostream>
#include <numbers>

#include "raylib.h"
#include "map.hpp"

int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;

int CELL_SIZE = 64;

float DegreesToRads(float degrees) {
    return degrees * (std::numbers::pi / 180.0f);
}

Vector2 AngleToVector2(float angle) {
    return {cosf(DegreesToRads(angle)), sinf(DegreesToRads(angle))};
}

void Render2DMap(Map& map) {
    for (int r = 0; r < map.rows; r++) {
        for (int c = 0; c < map.cols; c++) {
            DrawRectangle(c * CELL_SIZE, r * CELL_SIZE, CELL_SIZE, CELL_SIZE, map.IsSolid(r, c) ? WHITE : BLACK);
            DrawRectangle((c+1) * CELL_SIZE - 1, r * CELL_SIZE, 2, CELL_SIZE, GRAY);
            DrawRectangle(c * CELL_SIZE, (r+1) * CELL_SIZE - 1, CELL_SIZE, 2, GRAY);
        }
    }
}

int main() {
    Map map{8, 8};
    float angle = 30;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Liminalstein");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        BeginDrawing();
            ClearBackground(BLACK);
            Render2DMap(map);
            
            angle += 64.0f * dt;
            RaycastResult result = map.CastRay({1.5, 1.5}, AngleToVector2(angle));

            DrawLineV(
                Vector2Scale(result.start_pos, CELL_SIZE),
                Vector2Scale(result.end_pos, CELL_SIZE),
                RED
            );
        EndDrawing();
    }
    //std::cout << result.to_str() << "\n";
    return 0;
}