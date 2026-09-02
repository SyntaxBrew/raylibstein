#include "game.hpp"

float DegreesToRads(float degrees) {
    return degrees * (std::numbers::pi / 180.0f);
}

Vector2 AngleToVector2(float angle) {
    return {cosf(DegreesToRads(angle)), sinf(DegreesToRads(angle))};
}

Game::Game() {
    InitWindow(this->window_width, this->window_height, "Liminalstein");
    SetTargetFPS(60);

    this->cpu_image = GenImageColor(this->window_width, this->window_height, BLACK);
    this->gpu_texture = LoadTextureFromImage(this->cpu_image);
}

Game::~Game() {
   UnloadImage(this->cpu_image);
   UnloadTexture(this->gpu_texture);
}

void Game::Run() {
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
    this->input_state.mouse_pos = GetMousePosition();
    
    if (IsKeyDown(KEY_LEFT)) this->input_state.turn--;
    if (IsKeyDown(KEY_RIGHT)) this->input_state.turn++;
    if (IsKeyDown(KEY_W)) this->input_state.forward++;
    if (IsKeyDown(KEY_S)) this->input_state.forward--;
    if (IsKeyDown(KEY_D)) this->input_state.perp++;
    if (IsKeyDown(KEY_A)) this->input_state.perp--;
}

void Game::Update(float dt) {
    /*
    float angle_step = 2.0f * dt * this->input_state.turn;
    float old_dir_x = player.look_dir.x; 
    float old_dir_y = player.look_dir.y;

    // Apply a rotation matrix 
    player.look_dir.x = old_dir_x * std::cosf(angle_step) - old_dir_y * std::sinf(angle_step);
    player.look_dir.y = old_dir_x * std::sinf(angle_step) + old_dir_y * std::cosf(angle_step);

    // Prevent floating-point rounding errors from accumulating
    player.look_dir = Vector2Normalize(player.look_dir); 
    */

    player.look_dir = Vector2Normalize(Vector2Subtract(this->input_state.mouse_pos, Vector2Scale(player.pos, this->cell_2D_length)));

    // std::cout << player.look_dir.x << ", " << player.look_dir.y << "\n";

    Vector2 foward_dir = Vector2Scale(player.look_dir, this->input_state.forward);
    Vector2 perp_dir = Vector2Scale({-player.look_dir.y, player.look_dir.x}, this->input_state.perp);
    // Combine forward and perpendiular directions to calculate where player is moving
    player.move_dir = Vector2Normalize(Vector2Add(foward_dir, perp_dir));

    Vector2 delta_pos = Vector2Scale(player.move_dir, player.speed * dt); // change in player position after dt seconds, scaled by player speed
    Vector2 proposed_pos = Vector2Add(player.pos, delta_pos); // candidate pos to test whether it's actually possible to move there first
    Map& map = this->map;

    // Handle horizontal movement, check top and bottom boundaries
    // If its currently halfway under/above a corner, no horizontal movement occurs
    for (int y_dir : {-1, 1}) {
        for (int x_dir: {-1, 1}) {
            if (map.IsSolid(player.pos.y + player.radius * y_dir, proposed_pos.x + player.radius * x_dir)) {
                delta_pos.x = 0;
            }
        }
    }

    // Handles vertical movement, check left and right boundaries
    // If its currently halfway to the left/right of a corner, no vertical movement occurs
    // Prevent corner-clipping to the left/right sides of a wall 
    for (int y_dir : {-1, 1}) {
        for (int x_dir : {-1, 1}) {
            if (map.IsSolid(proposed_pos.y + player.radius * y_dir, player.pos.x + player.radius * x_dir)) {
                delta_pos.y = 0;
            }
        }
    }
  
    player.pos = Vector2Add(player.pos, delta_pos);

    this->raycasts.clear();
    for (int col = 0; col < this->window_width; col++) {
        // Map each screen column to a bipolar X-coordinate [-1.0, 1.0] on the camera plane to give direction (left/right side of screen)
        float plane_x = ((2.0f * (float) col) / this->window_width) - 1; 
        float plane_length = std::tanf(DegreesToRads(player.fov / 2));

        // Generate a perpendicular plane vector scaled by FOV and screen position
        Vector2 plane = Vector2Scale({-player.look_dir.y, player.look_dir.x}, plane_length * plane_x);

        // The direction of each ray is calculated from the evenly spaced plane X-offsets
        // This ensures that rays are evenly spaced across the flat screen projection
        // Fixes distortion near the edges of the screen
        Vector2 ray_dir = Vector2Normalize(Vector2Add(player.look_dir, plane));

        RaycastResult result = this->map.CastRay(player.pos, ray_dir);
        this->raycasts.push_back(result);
    }
}

void Game::Render() {
    // Extract color pixel data from CPU buffer
    Color* pixels = (Color*) this->cpu_image.data;
    std::fill(pixels, pixels + this->window_width * this->window_height, BLACK); // Clear previous CPU pixel buffer

    // Update GPU buffer with color pixel data from CPU
    UpdateTexture(this->gpu_texture, pixels);

    BeginDrawing();
    ClearBackground(BLACK); // Clear previous GPU canvas 
    DrawTexture(this->gpu_texture, 0, 0, WHITE);

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
    
    for (RaycastResult& result: this->raycasts) {
        DrawLineV(
            Vector2Scale(result.start_pos, CELL_SIZE),
            Vector2Scale(result.end_pos, CELL_SIZE),
            RED
        );
    }

    EndDrawing();
}

