#include "game.hpp"

float DegreesToRads(float degrees) {
    return degrees * (std::numbers::pi / 180.0f);
}

Vector2 AngleToVector2(float angle) {
    return {cosf(DegreesToRads(angle)), sinf(DegreesToRads(angle))};
}

Game::Game() {
   
}

Game::~Game() {
   
}

void Game::Run() {
    InitWindow(this->window_width, this->window_height, "Liminalstein");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        ProcessInputs();
        Update(dt);
        Render();
    }

    CloseWindow();
}

void Game::ProcessInputs() {
    
}

void Game::Update(float dt) {

}

void Game::Render() {
    BeginDrawing();

    ClearBackground(BLACK);
    int CELL_SIZE = this->cell_2D_length;

    for (int r = 0; r < this->map.rows; r++) {
        for (int c = 0; c < this->map.cols; c++) {
            DrawRectangle(c * CELL_SIZE, r * CELL_SIZE, CELL_SIZE, CELL_SIZE, map.IsSolid(r, c) ? WHITE : BLACK);
            DrawRectangle((c+1) * CELL_SIZE - 1, r * CELL_SIZE, 2, CELL_SIZE, GRAY);
            DrawRectangle(c * CELL_SIZE, (r+1) * CELL_SIZE - 1, CELL_SIZE, 2, GRAY);
        }
    }

    EndDrawing();
}

