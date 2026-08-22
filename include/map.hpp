#include <vector>

class Map {
    int rows;
    int cols;
    std::vector<std::vector<int>> cells;

public:
    Map(int rows, int cols);
    bool in_bounds(int r, int c);
    bool is_solid(int r, int c);
    int get_cell(int r, int c);
};