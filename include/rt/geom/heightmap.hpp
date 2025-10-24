#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <memory>
#include "hittable.hpp"

class Triangle;

using std::shared_ptr;
using std::function;

/**
 * @class Heightmap
 * @brief Stores vertex heights in a grid arrangement that can be used to construct a procedural terrain of Triangles.
 */
class Heightmap {
public:
    /**
     * @brief Constructs a new Heightmap represented by a flat 2D grid of vertex heights.
     * @param noise Noise function that will determine the procedural pattern of heights in the Heightmap.
     * @param corner Corner coordinates, where the first vertex will be.
     * @param grid_square_length Length of each grid square in the Heightmap.
     * @param length Length of the Heightmap grid, in number of grid squares.
     * @param width Width of the Heightmap grid, in number of grid squares.
     */
    constexpr Heightmap(
        const function<double(double, double)>& noise,
        const coord3& corner,
        const float grid_square_length,
        const int length,
        const int width
        ) :
        corner_{coord3{corner.x(), corner.y(), corner.z()}},
        grid_square_len_{grid_square_length},
        length_{std::max(1, length)},
        width_{std::max(1, width)} {
        vertices_heights_.resize(length_ * width_);
        for (int z = 0; z < length_; z++) {
            for (int x = 0; x < width_; x++) {
                vertices_heights_[z * width_ + x] = corner_.y() + noise(x, z);
            }
        }
    }

    /**
     * @brief Constructs triangles arranged in grid arrangement, where each grid square are of unit dimensions and composed
     * of two Triangles.
     * @return Vector of Triangles to be passed into the BVH.
     */
    [[nodiscard]] std::vector<shared_ptr<Triangle>> construct_map() const;
private:
    coord3 corner_;                         // Location of first grid square
    float grid_square_len_;                 // Length of each grid square
    int length_, width_;                    // Num of grid squares per length/width
    std::vector<float> vertices_heights_;   // Vertices stored effectively in "heightmap-space" (relative to heightmap grid)
};

#endif