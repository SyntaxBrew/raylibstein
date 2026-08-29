#include <cmath>
#include <iostream>
#include <limits>
#include "map.hpp"

constexpr float inf = std::numeric_limits<float>::infinity();

Map::Map(int rows, int cols) : rows(rows), cols(cols) {
    this->cells = {
        {1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,1},
        {1,0,1,0,0,1,0,1},
        {1,0,0,0,1,0,0,1},
        {1,0,0,1,1,0,0,1},
        {1,0,1,0,0,1,0,1},
        {1,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1}
    };
}

RaycastResult Map::CastRay(const Vector2& start_pos, const Vector2& dir) {
    float side_dist_x = inf; // Running total to the next vertical line
    float side_dist_y = inf; // Running total to the next horizontal line

    // Totals initialized as distance to the first vertical/horizontal grid lines
    if (dir.x > 0) 
        side_dist_x = std::ceilf(start_pos.x) - start_pos.x;
    else if (dir.x < 0)
        side_dist_x = start_pos.x - std::floorf(start_pos.x); 

    if (dir.y > 0)
        side_dist_y = std::ceilf(start_pos.y) - start_pos.y;
    else if (dir.y < 0)
        side_dist_y = start_pos.y - std::floorf(start_pos.y); 

    float delta_dist_x = std::abs(1.0 / dir.x); // angled dist between each vertical line
    float delta_dist_y = std::abs(1.0 / dir.y); // angled dist between each horizontal line

    // we can increment x, y counters instead of re-calculating cell positions
    int row = (int) start_pos.x;
    int col = (int) start_pos.y;
    int step_x = dir.x == 0 ? 0 : (dir.x > 0 ? 1 : -1); 
    int step_y = dir.y == 0 ? 0 : (dir.y > 0 ? 1 : -1); 

    float dist_travelled = 0.0f; // running total to the current gridline
    bool hit = false;
    bool is_vertical = false;

    while (!hit && InBounds(row, col)) {
        // choose the closest grid line (what the ray hits first)

        // vertical hit
        if (side_dist_x < side_dist_y) { 
            dist_travelled = side_dist_x;
            row += step_x;
            side_dist_x += delta_dist_x;
            is_vertical = true;
        
        // horizontal hit
        } else {
            dist_travelled = side_dist_y;
            col += step_y;
            side_dist_y += delta_dist_y;
            is_vertical = false;
        }

        if (IsSolid(row, col)) {
            hit = true;
        }
    }

    return {
        dist_travelled,
        dir,
        hit,
        is_vertical
    };
}

bool Map::InBounds(int r, int c) {
    return (0 <= r && r < rows) && (0 <= c && c < cols);
}

bool Map::IsSolid(int r, int c) {
    if (!InBounds(r, c)) return false;
    return this->cells[r][c] != 0;
}

int Map::GetCell(int r, int c) {
    if (!InBounds(r, c)) return -1;
    return this->cells[r][c];
}