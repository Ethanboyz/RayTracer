#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <memory>
#include "hittable.hpp"
#include "rt/utilities.hpp"

class Triangle;

using std::shared_ptr;

/**
 * @class Heightmap
 * @brief Stores vertex heights in a grid arrangement that can be used to construct a procedural terrain of Triangles.
 */
class Heightmap {
public:
    /**
     * @brief Constructs a new Heightmap represented by a flat 2D grid of vertex heights.
     * @param corner Corner coordinates, where the first vertex will be. Coordinates should consist of whole numbers.
     * @param length Length of the Heightmap grid.
     * @param width Width of the Heightmap grid.
     */
    constexpr Heightmap(const coord3& corner, const int length, const int width) :
        corner_{coord3{truncf(corner.x()), truncf(corner.y()), truncf(corner.z())}},
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
     * @param material Material of all the constructed Triangles.
     * @return Vector of Triangles to be passed into the BVH.
     */
    [[nodiscard]] std::vector<shared_ptr<Triangle>> construct_map(const Material& material) const;
private:
    coord3 corner_;
    int length_, width_;
    std::vector<float> vertices_heights_;   // Vertices stored effectively in "heightmap-space" (relative to heightmap grid)

    /** @return Height value (0.0-1.0) of a specified x and y-coordinate pair in heightmap-space. */
    static float noise([[maybe_unused]] const int x, [[maybe_unused]] const int z) {
        return Utilities::random_float();
    }
};

#endif