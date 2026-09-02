#include <iostream>
#include <numbers>
#include <vector>

#include "raylib.h"

#include "map.hpp"
#include "player.hpp"


struct InputState {
    float turn = 0;
    int forward = 0;
    int perp = 0;
    Vector2 mouse_pos;
};

class Game {
private:
    int window_width = 1280;
    int window_height = 720;
    int cell_2D_length = 16;
    int ray_offset = 8;
    float fov = 90;
    float focal_length;

    Map map = Map(8, 8);
    Player player = {
        .pos = {1.5, 1.5},
        .look_dir = {0, 1},
        .speed = 1,
        .radius = 0.1
    };
    InputState input_state;
    std::vector<RaycastResult> raycasts{};

    Image cpu_image;       // image is a cpu-side structure
    Texture2D gpu_texture;   // texture is a gpu-side structure

public:
    Game();
    ~Game();

    void Run();
    void ProcessInputs(float dt);
    void Update(float dt);
    void Render();
};