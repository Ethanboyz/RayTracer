#ifndef NOISE_H
#define NOISE_H

/**
 * @class Noise
 * @brief Describes a noise function that provides a height value for each given Heightmap (x, z) coordinate.
 */
class Noise {
public:
    virtual ~Noise() = default;

    /**
     * @return Height value (-1.0 - 1.0) of a specified x and z-coordinate pair in heightmap-space.
     * @note Return result should be used to construct a Heightmap.
     */
    virtual float eval(int x, int z) const = 0;
};

#endif