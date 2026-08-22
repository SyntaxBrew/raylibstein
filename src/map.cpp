#include "map.hpp"

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

bool Map::in_bounds(int r, int c) {
    return (0 <= r && r < rows) && (0 <= c && c < cols);
}

bool Map::is_solid(int r, int c) {
    if (!in_bounds(r, c)) return false;
    return this->cells[r][c] != 0;
}

int Map::get_cell(int r, int c) {
    if (!in_bounds(r, c)) return -1;
    return this->cells[r][c];
}