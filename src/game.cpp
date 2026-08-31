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
        ProcessInputs(dt);
        Update(dt);
        Render();
    }

    CloseWindow();
}

// TODO: MOVE DIRECT LOOK_DIR, MOVE_DIR UPDATES TO UPDATE METHOD, USE INPUT STATE STRUCT
void Game::ProcessInputs(float dt) {
    this->input_state.turn = 0; // cw/ccw rotation
    this->input_state.forward = 0; // forward motion
    this->input_state.perp = 0; // sideways motion
    
    if (IsKeyDown(KEY_LEFT)) this->input_state.turn--;
    if (IsKeyDown(KEY_RIGHT)) this->input_state.turn++;
    if (IsKeyDown(KEY_W)) this->input_state.forward++;
    if (IsKeyDown(KEY_S)) this->input_state.forward--;
    if (IsKeyDown(KEY_D)) this->input_state.perp++;
    if (IsKeyDown(KEY_A)) this->input_state.perp--;
}

void Game::Update(float dt) {
    float angle_step = 2.0f * dt * this->input_state.turn;
    float old_dir_x = player.look_dir.x; 
    float old_dir_y = player.look_dir.y;

    // Apply a rotation matrix 
    player.look_dir.x = old_dir_x * std::cosf(angle_step) - old_dir_y * std::sinf(angle_step);
    player.look_dir.y = old_dir_x * std::sinf(angle_step) + old_dir_y * std::cosf(angle_step);

    // Prevent floating-point rounding errors from accumulating
    player.look_dir = Vector2Normalize(player.look_dir); 

    std::cout << player.look_dir.x << ", " << player.look_dir.y << "\n";

    Vector2 foward_dir = Vector2Scale(player.look_dir, this->input_state.forward);
    Vector2 perp_dir = Vector2Scale({-player.look_dir.y, player.look_dir.x}, this->input_state.perp);
    // Combine forward and perpendiular directions to calculate where player is moving
    player.move_dir = Vector2Normalize(Vector2Add(foward_dir, perp_dir));

    Vector2 delta_pos = Vector2Scale(player.move_dir, player.speed * dt); // change in player position after dt seconds, scaled by player speed
    player.pos = Vector2Add(player.pos, delta_pos);
}

void Game::Render() {
    BeginDrawing();

    ClearBackground(BLACK);
    int CELL_SIZE = this->cell_2D_length;

    for (int r = 0; r < this->map.rows; r++) {
        for (int c = 0; c < this->map.cols; c++) {
            DrawRectangle(c * CELL_SIZE, r * CELL_SIZE, CELL_SIZE, CELL_SIZE, map.IsSolid(r, c) ? WHITE : BLACK);
            
            // Draw cell outlines
            DrawRectangle((c+1) * CELL_SIZE - 1, r * CELL_SIZE, 2, CELL_SIZE, GRAY);
            DrawRectangle(c * CELL_SIZE, (r+1) * CELL_SIZE - 1, CELL_SIZE, 2, GRAY);
        }
    }

    Vector2 player_render_pos = Vector2Scale(player.pos, CELL_SIZE);
    float player_render_radius = player.radius * CELL_SIZE;

    DrawCircleV(player_render_pos, player_render_radius, GREEN);
    DrawLineV(player_render_pos, Vector2Add(player_render_pos, Vector2Scale(player.look_dir, CELL_SIZE)), BLUE);

    EndDrawing();
}

