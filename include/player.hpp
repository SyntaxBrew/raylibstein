#include "raymath.h"

struct Player {
    Vector2 position;
    Vector2 direction;
    Vector2 plane;
    float speed;
    float radius;
    
    Player(Vector2 pos, Vector2 dir, float speed, float radius);
};