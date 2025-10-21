#include "rt/geom/heightmap.hpp"

// For each quad (square of vertices), construct two triangles
std::vector<std::shared_ptr<Triangle>> Heightmap::construct_map() const {
    int vertex = 0;

    // Iterate through each vertex one width at a time
    while (vertex / width_ < length_ - 1) {
        while (vertex % width_ < ) {

        }
    }
}