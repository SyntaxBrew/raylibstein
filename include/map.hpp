#include <vector>
#include "raymath.h"

struct RaycastResult {
    float distance;
    Vector2 direction;
    bool hit;
    bool is_vertical;
};

class Map {
    int rows;
    int cols;
    std::vector<std::vector<int>> cells;

public:
    Map(int rows, int cols);

    RaycastResult CastRay(Vector2 start_pos, Vector2 dir);
    bool InBounds(int r, int c);
    bool IsSolid(int r, int c);
    int GetCell(int r, int c);
};