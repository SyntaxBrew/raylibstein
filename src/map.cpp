#include <cmath>
#include <iostream>
#include <limits>
#include "map.hpp"

constexpr float inf = std::numeric_limits<float>::infinity();

Map::Map(int rows, int cols) : rows(rows), cols(cols) {
    this->cells = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1},
        {1,0,1,1,0,1,0,1,1,1,0,1,0,1,0,1},
        {1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1},
        {1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
        {1,1,1,1,1,0,1,1,1,1,0,1,1,1,0,1},
        {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
        {1,0,1,1,1,0,1,0,1,1,1,1,1,1,0,1},
        {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2},
        {1,0,0,0,0,0,0,0,0,0,0,0,2,0,0,2},
        {1,0,1,0,0,1,0,0,1,0,2,0,2,0,0,2},
        {1,0,0,0,0,0,0,0,0,0,2,0,0,0,0,2},
        {1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2}
    };
}

RaycastResult Map::CastRay(const Vector2& start_pos, const Vector2& dir) {
    float initial_dist_x = inf; // Horizontal dist to the first vertical line
    float initial_dist_y = inf; // Vertical dist to the first horizontal line

    if (dir.x > 0) 
        initial_dist_x = (int) (start_pos.x + 1) - start_pos.x;
    else if (dir.x < 0)
        initial_dist_x = start_pos.x - (int) (start_pos.x); 

    if (dir.y > 0)
        initial_dist_y = (int) (start_pos.y + 1) - start_pos.y;
    else if (dir.y < 0)
        initial_dist_y = start_pos.y - (int) (start_pos.y); 

    // Running totals initialized as angled dists to the first vertical/horizontal grid lines
    float side_dist_x = std::fabs(initial_dist_x / dir.x);
    float side_dist_y = std::fabs(initial_dist_y / dir.y);

    float delta_dist_x = std::fabs(1.0 / dir.x); // angled dist between each vertical line
    float delta_dist_y = std::fabs(1.0 / dir.y); // angled dist between each horizontal line

    // we can increment x, y counters instead of re-calculating cell positions
    int col = (int) std::floorf(start_pos.x);
    int row = (int) std::floorf(start_pos.y);
    int step_x = dir.x == 0 ? 0 : (dir.x > 0 ? 1 : -1); 
    int step_y = dir.y == 0 ? 0 : (dir.y > 0 ? 1 : -1); 

    float dist_travelled = 0.0f; // running total to the current gridline
    bool hit = false;
    bool is_vertical = false;

    while (!hit && InBounds(row, col)) {
        // choose the closest grid line (what the ray hits first)
        // vertical line
        if (side_dist_x < side_dist_y) { 
            dist_travelled = side_dist_x;
            col += step_x;
            side_dist_x += delta_dist_x;
            is_vertical = true;
        
        // horizontal line
        } else {
            dist_travelled = side_dist_y;
            row += step_y;
            side_dist_y += delta_dist_y;
            is_vertical = false;
        }

        //std::cout << dist_travelled << "\n";
        //std::cout << row << ", " << col << "\n";
        
        if (IsSolid(row, col)) {
            hit = true;
        }
    }

    return {
        hit ? dist_travelled : inf,
        dir,
        start_pos,
        {start_pos.x + dist_travelled * dir.x, start_pos.y + dist_travelled * dir.y},
        hit,
        is_vertical,
        this->GetCell(row, col)
    };
}

bool Map::InBounds(int r, int c) {
    return (0 <= r && r < rows) && (0 <= c && c < cols);
}

bool Map::IsSolid(int r, int c) {
    if (!InBounds(r, c)) return false;
    return this->cells[r][c] > 0;
}

int Map::GetCell(int r, int c) {
    if (!InBounds(r, c)) return -1;
    return this->cells[r][c];
}