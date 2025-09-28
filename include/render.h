#ifndef RENDER_H
#define RENDER_H

#include <iostream>
#include <fstream>
#include "hittable.h"
#include "light_list.h"

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
    void render(const HittableList& world, const LightList& world_lights, const Camera& camera) const {
        // Center of first pixel (upper left) will be at the upperleft corner of viewport shifted halfway of a pixel delta
        const coord3 pixel_0_center = camera.viewport_upperleft_corner() + (0.5 * (camera.pixel_delta_u() + camera.pixel_delta_v()));
        std::vector<Color> pixel_colors;
        for (int y = 0; y < image_height_; y++) {
            std::clog << "\rRay Progress: " << (static_cast<double>(y) / image_height_) * 100 << "% " << std::flush;
            for (int x = 0; x < image_width_; x++) {
                // Use pixel's and camera's location to generate a ray
                coord3 pixel_center = pixel_0_center + camera.pixel_delta_u() * static_cast<float>(x) + camera.pixel_delta_v() * static_cast<float>(y);
                Ray ray{camera.position(), unit(pixel_center - camera.position())};
                pixel_colors.push_back(ray_color(ray, world, world_lights));
            }
        }
        // Done generating rays, write pixel colors to file
        std::clog << "\rRay Progress: 100.0%                         \n" << std::flush;
        write_to_file("image.ppm", pixel_colors);
        std::clog << "\rWrote to image.ppm\n" << std::flush;
    }

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
    static Color ray_color(const Ray& ray, const Hittable& world, const LightList& world_lights) {
        HitRecord hit_record;

        // Minimum of t = 0 so camera effectively looks forwards (not also backwards)
        if (!world.ray_hit(ray, Interval(0.f, std::numeric_limits<float>::max()), hit_record)) {
            // Background color light gray gradient dependent on y coord. -1 <= y <= 1, but 0 <= a <= 1 for color = (1 - a) * low_y_color + a * high_y_color
            const float a = 0.5f * (ray.direction().y() + 1);
            constexpr auto lightgray{Color{0.9f, 0.9f, 0.9f}};
            constexpr auto gray{Color{0.4f, 0.4f, 0.4f}};
            return (1 - a) * lightgray + a * gray;
        }
        // Color of hit object based on its stored materialistic properties and world lights
        const Material material{hit_record.material()};
        world_lights.light_intensity(hit_record);
        return material.color() * hit_record.light_intensity();
    }


    /**
     * @brief Outputs all image pixel data to a ppm file.
     * @param filename The name of the file containing the completed render.
     * @param pixels All the colored pixels to be written to the image file.
     */
    void write_to_file(const std::string& filename, const std::vector<Color>& pixels) const {
        std::ofstream out{filename, std::ios_base::binary};
        if (!out) {
            const std::error_code error{errno, std::generic_category()};
            throw std::runtime_error("Failed to open output file: " + filename + " (" + error.message() + ")");
        }
        out << "P6\n" << image_width_ << ' ' << image_height_ << "\n255\n";

        std::vector<uint8_t> outbuf;
        outbuf.reserve(pixels.size() * 3);
        for (const Color& pixel : pixels) {  // 1 byte per color channel
            const float r = pixel.x();
            const float g = pixel.y();
            const float b = pixel.z();
            outbuf.push_back(static_cast<uint8_t>(255.999f * r));
            outbuf.push_back(static_cast<uint8_t>(255.999f * g));
            outbuf.push_back(static_cast<uint8_t>(255.999f * b));
        }
        out.write(reinterpret_cast<const char*>(outbuf.data()), static_cast<long>(outbuf.size()));
    }
};

#endif