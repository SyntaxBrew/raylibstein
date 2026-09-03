#include <vector>
#include <string>
#include "raymath.h"

struct RaycastResult {
    float distance;
    Vector2 dir;
    Vector2 start_pos;
    Vector2 end_pos;
    bool hit;
    bool is_vertical;
    int cell_type;

    std::string to_str() {
        return std::string("RaycastResult {\n")
        + "\tdistance: " + std::to_string(this->distance) + "\n"
        + "\tdir: " + "(" + std::to_string(this->dir.x) + ", " + std::to_string(this->dir.y) + ")" + "\n"
        + "\tstart_pos" + "(" + std::to_string(this->start_pos.x) + ", " + std::to_string(this->start_pos.y) + ")" + "\n"
        + "\tend_pos" + "(" + std::to_string(this->end_pos.x) + ", " + std::to_string(this->end_pos.y) + ")" + "\n"
        + "\thit: " + (this->hit ? "true" : "false") + "\n"
        + "\tis_vertical: " + (this->is_vertical ? "true" : "false") + "\n"
        + "\tcell_type: " + std::to_string(this->cell_type) + "\n}";
    }
};

struct Map {
    int rows;
    int cols;
    std::vector<std::vector<int>> cells;

public:
    Map(int rows, int cols);

    RaycastResult CastRay(const Vector2& start_pos, const Vector2& dir);
    bool InBounds(int r, int c);
    bool IsSolid(int r, int c);
    int GetCell(int r, int c);
};