#include <array>

//size of a  tile
#define TILESET_ELEMENT_SIZE 16

void move_object(const int id, const float dx, const float dy);

/// Holds the overall map data
//0th tile is top left
extern std::array<std::array<int, 16>, 16> world_data;