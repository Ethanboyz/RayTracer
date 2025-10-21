#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <memory>
#include "hittable.hpp"

class Triangle;

/**
 * @class Heightmap
 * @brief Stores vertex heights in a grid arrangement that can be used to construct a procedural terrain of Triangles.
 */
class Heightmap : public Hittable {
public:
    /**
     * @brief Constructs a new Heightmap represented by a flat 2D grid of vertex heights.
     * @param corner Corner coordinates, where the first vertex will be. Coordinates should consist of whole numbers.
     * @param length Length of the Heightmap grid.
     * @param width Width of the Heightmap grid.
     */
    constexpr Heightmap(const coord3& corner, const int length, const int width) :
        corner_{coord3{truncf(corner.x()), truncf(corner.y()), truncf(corner.z())}},
        length_{length},
        width_{width} {
        for (int y = corner.y(); y < corner.y() + length; y++) {
            for (int x = corner.x(); x < corner.x() + width; x++) {
                vertices_heights_[y * width + x] = corner.z() + noise(x, y);
            }
        }
    }

    /**
     * @brief Constructs triangles arranged in grid arrangement, where each grid square are of unit dimensions and composed
     * of two Triangles.
     * @return Vector of Triangles to be passed into the BVH.
     */
    std::vector<std::shared_ptr<Triangle>> construct_map() const;
private:
    coord3 corner_;
    int length_, width_;
    std::vector<float> vertices_heights_;   // Vertices stored effectively in "heightmap-space" (independent of world coords)

    /** @return Height value of a specified x and y-coordinate pair. */
    static float noise(const int x, const int y) {
        return 0.f * x * y;
    }
};

#endif