#include <iostream>
#include <numbers>

#include "raylib.h"

#include "map.hpp"
#include "player.hpp"

struct InputState {
    int turn = 0;
    int forward = 0;
    int perp = 0;
};

class Game {
private:
    int window_width = 1280;
    int window_height = 720;
    int cell_2D_length = 64;

    Map map = Map(8, 8);
    Player player = Player({1.5, 1.5}, {0, 1}, 1, 0.1);
    InputState input_state;

public:
    Game();
    ~Game();

    void Run();
    void ProcessInputs(float dt);
    void Update(float dt);
    void Render();
};