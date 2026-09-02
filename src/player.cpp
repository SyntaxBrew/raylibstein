#include "raymath.h"
#include "player.hpp"

Player::Player(Vector2 pos, Vector2 look_dir, float speed, float radius, float fov) :
    pos(pos), look_dir(look_dir), speed(speed), radius(radius), fov(fov) {}