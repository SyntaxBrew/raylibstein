#include "raymath.h"

struct Player {
    Vector2 pos;
    Vector2 look_dir;
    Vector2 plane;
    Vector2 move_dir;
    float speed;
    float radius;
    float fov;

    Player(Vector2 pos, Vector2 look_dir, float speed, float radius, float fov);
};