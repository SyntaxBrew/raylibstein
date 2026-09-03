#include "game.hpp"
#include <algorithm>

float DegreesToRads(float degrees) {
    return degrees * (std::numbers::pi / 180.0f);
}

Vector2 AngleToVector2(float angle) {
    return {cosf(DegreesToRads(angle)), sinf(DegreesToRads(angle))};
}

Game::Game() {
    InitWindow(this->window_width, this->window_height, "Liminalstein");
    SetTargetFPS(60);
    DisableCursor();

    this->cpu_image = GenImageColor(this->window_width, this->window_height, BLACK);
    this->gpu_texture = LoadTextureFromImage(this->cpu_image);
    this->map_textures = {
        {1, MapTexture("assets/textures/lobby_wall.png")},
        {2, MapTexture("assets/textures/manila_wall.png")},
    };

    this->focal_length = (this->window_width / 2.0f) / std::tanf(DegreesToRads(this->fov / 2));
}

Game::~Game() {
    UnloadImage(this->cpu_image);
    UnloadTexture(this->gpu_texture);

    for (const auto& [id, map_texture]: this->map_textures) {
        const auto& [image, pixels] = map_texture;
        UnloadImage(image);
        UnloadImageColors(pixels);
    }
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

    Vector2 mouse_delta = GetMouseDelta();
    this->input_state.turn += mouse_delta.x * 0.1;

    float mouse_wheel_delta = GetMouseWheelMove();
    this->fov = std::clamp(this->fov * (1 - (0.02f * mouse_wheel_delta)), 1.0f, 179.0f);
    this->focal_length = (this->window_width / 2.0f) / std::tanf(DegreesToRads(this->fov / 2));
    
    if (IsKeyPressed(KEY_GRAVE)) this->noclip = !this->noclip;

    if (IsKeyDown(KEY_LEFT)) this->input_state.turn--;
    if (IsKeyDown(KEY_RIGHT)) this->input_state.turn++;
    if (IsKeyDown(KEY_W)) this->input_state.forward++;
    if (IsKeyDown(KEY_S)) this->input_state.forward--;
    if (IsKeyDown(KEY_D)) this->input_state.perp++;
    if (IsKeyDown(KEY_A)) this->input_state.perp--;
}

void Game::Update(float dt) {
    float angle_step = dt * this->input_state.turn;
    float old_dir_x = player.look_dir.x; 
    float old_dir_y = player.look_dir.y;

    // Apply a rotation matrix 
    player.look_dir.x = old_dir_x * std::cosf(angle_step) - old_dir_y * std::sinf(angle_step);
    player.look_dir.y = old_dir_x * std::sinf(angle_step) + old_dir_y * std::cosf(angle_step);

    // Prevent floating-point rounding errors from accumulating
    player.look_dir = Vector2Normalize(player.look_dir); 

    //player.look_dir = Vector2Normalize(Vector2Subtract(this->input_state.mouse_pos, Vector2Scale(player.pos, this->cell_2D_length)));
    // std::cout << player.look_dir.x << ", " << player.look_dir.y << "\n";

    Vector2 foward_dir = Vector2Scale(player.look_dir, this->input_state.forward);
    Vector2 perp_dir = Vector2Scale({-player.look_dir.y, player.look_dir.x}, this->input_state.perp);
    // Combine forward and perpendiular directions to calculate where player is moving
    player.move_dir = Vector2Normalize(Vector2Add(foward_dir, perp_dir));

    Vector2 delta_pos = Vector2Scale(player.move_dir, player.speed * dt); // change in player position after dt seconds, scaled by player speed
    Vector2 proposed_pos = Vector2Add(player.pos, delta_pos); // candidate pos to test whether it's actually possible to move there first
    Map& map = this->map;

    if (!this->noclip) {
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
    }
    
    player.pos = Vector2Add(player.pos, delta_pos);

    this->raycasts.clear();
    for (int col = 0; col < this->window_width; col += ray_offset) {
        // Map each screen column to a bipolar X-coordinate [-1.0, 1.0] on the camera plane to give direction (left/right side of screen)
        float plane_x = ((2.0f * (float) col) / this->window_width) - 1; 
        float plane_length = std::tanf(DegreesToRads(this->fov / 2));

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

    // Update pixels
    for (int col = 0; col < this->window_width; col++) {
        // Automatically truncates down to the nearest raycast result for offsets > 1
        RaycastResult& result = this->raycasts[col / ray_offset];

        // Prevent wall warping (fish-eye correction) by using perpendicular distances instead of total distance
        // Diagonal rays travel farther at an angle, so always ignore the extra parallel distance added
        float dot_product = Vector2DotProduct(player.look_dir, result.dir); // x1 * x2 + y1 * y1
        float perp_dist = result.distance * (dot_product / (Vector2Length(player.look_dir) * Vector2Length(result.dir))); 

        // Multiply height of each strip by the ratio: half-width of actual window / half-width of virtual camera
        // This scales up the entire game (distances) to the proper window size
        float strip_height = (1.0f / perp_dist) * focal_length;

        // Snap height to pixelated steps, remove fractional portion, then multiply back 
        int px_strip_height = ((int) std::roundf(strip_height / this->ray_offset)) * this->ray_offset;

        int top_boundary = this->window_height / 2 - px_strip_height / 2;
        int bottom_boundary = this->window_height / 2 + px_strip_height / 2;
      
        MapTexture* map_texture = nullptr;
        if (this->map_textures.contains(result.cell_type)) {
            map_texture = &this->map_textures.at(result.cell_type);
        }
       
        for (int row = 0; row < this->window_height; row++) {
            // Convert 2D coordinates to a 1D index
            int index = (row * this->window_width) + col;

            // Roof
            if (row < top_boundary) {
                pixels[index] = {133, 128, 60, 255};
            }

            // Floor
            if (row >= bottom_boundary) {
                pixels[index] = {113, 98, 33, 255};
            }

            if (map_texture != nullptr) {
                // Update each corresponding pixel within the strip boundary
                if (top_boundary <= row && row < bottom_boundary) {
                    int texture_col = (result.is_vertical ? result.end_pos.y - std::floorf(result.end_pos.y) : result.end_pos.x - std::floorf(result.end_pos.x)) * 64;
                    int texture_row = ((float) (row - top_boundary) / px_strip_height) * 64;
                    int texture_index = texture_row * 64 + texture_col;
                    pixels[index] = result.is_vertical ? map_texture->pixels[texture_index] : ColorBrightness(map_texture->pixels[texture_index], -0.2f);
                }
            }
        }
    }

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

