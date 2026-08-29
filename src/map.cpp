#include <cmath>
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

RaycastResult Map::CastRay(Vector2 start_pos, Vector2 dir) {
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