#ifndef RENDER_H
#define RENDER_H

#include <iostream>
#include <fstream>
#include "rt/geom/hittable.hpp"
#include "rt/geom/hittable_list.hpp"
#include "rt/lighting/light_list.hpp"
#include "rt/render/camera.hpp"

/**
 * @class Renderer
 * @brief Encapsulates the overarching functions needed to produce a completed render file.
 */
class Renderer {
public:
    Renderer() : image_width_{400}, image_height_{225} {}
    Renderer(const int image_width, const int image_height) : image_width_{image_width}, image_height_{image_height} {}

    /**
     * @brief Render visual output to a .ppm image file.
     *
     * Sequentially generates a backward-tracing ray for each viewport pixel and coloring the results
     * to a .ppm in P6 (binary) format.
     * @param world All the Hittable objects to include in the render.
     * @param world_lights All the Light sources to include in the render.
     * @param camera Renders will be created in the perspective of the camera.
     */
    void render(const HittableList& world, const LightList& world_lights, const Camera& camera) const;

private:
    int image_width_;
    int image_height_;

    /**
     * @brief For a specific ray, calculates the color that should be written to the corresponding viewport pixel.
     * @param ray The ray in which coloring calculations will take place.
     * @param world All the Hittable objects to include in the render.
     * @param world_lights All the Light sources to include in the render.
     * @return The color that should be written to the ray's corresponding viewport pixel.
     */
    static Color ray_color(const Ray& ray, const Hittable& world, const LightList& world_lights);

    /**
     * @brief Outputs all image pixel data to a ppm file.
     * @param filename The name of the file containing the completed render.
     * @param pixels All the colored pixels to be written to the image file.
     */
    void write_to_file(const std::string& filename, const std::vector<Color>& pixels) const;
};

#endif