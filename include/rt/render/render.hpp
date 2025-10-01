#ifndef RENDER_H
#define RENDER_H

#include <fstream>
#include "rt/geom/hittable.hpp"
#include "rt/geom/hittable_list.hpp"
#include "rt/render/camera.hpp"

/**
 * @class Renderer
 * @brief Encapsulates the overarching functions needed to produce a completed render file.
 */
class Renderer {
public:
    /**
     * @brief Constructs a Renderer which can generate a completed image of the world into an output .ppm file.
     * @param camera
     */
    explicit Renderer(const Camera& camera)
      : image_width_{camera.image_width()},
        image_height_{camera.image_height()},

        // Center of first pixel (upper left) will be at the upperleft corner of viewport shifted halfway of a pixel delta
        pixel_0_center_{camera.viewport_upperleft_corner() + 0.5 * (camera.pixel_delta_u() + camera.pixel_delta_v())},

        camera_(camera) {}

    /**
     * @brief Render visual output to a .ppm image file.
     *
     * Sequentially generates a backward-tracing ray for each viewport pixel and coloring the results
     * to a .ppm in P6 (binary) format.
     * @param world All the Hittable objects to include in the render.
     */
    void render(const HittableList& world) const;

private:
    int image_width_;           // Number of rays to generate per row
    int image_height_;          // Number of ray to generate per column
    coord3 pixel_0_center_;     // Location of the first pixel (upperleft corner)
    Camera camera_;             // Renders will be created in the perspective of the camera

    /**
     * @brief For a specific ray, calculates the color that should be written to the corresponding viewport pixel.
     * @param ray The ray in which coloring calculations will take place.
     * @param depth Number of ray reflections until no more child rays can be generated.
     * @param world All the Hittable objects to include in the render.
     * @return The color that should be written to the ray's corresponding viewport pixel.
     */
    static Color ray_color(const Ray& ray, int depth, const Hittable& world);

    /**
     * @brief Creates a ray directed at a random point centered around a specified pixel.
     * @param x Horizontal position of the pixel.
     * @param y Vertical position of the pixel.
     * @return Return a generated ray randomly sampled around the pixel location (i, j).
     */
    [[nodiscard]] Ray generate_ray(int x, int y) const;

    /**
     * @brief Outputs all image pixel data to a ppm file.
     * @param filename The name of the file containing the completed render.
     * @param pixels All the colored pixels to be written to the image file.
     */
    void write_to_file(const std::string& filename, const std::vector<Color>& pixels) const;
};

#endif