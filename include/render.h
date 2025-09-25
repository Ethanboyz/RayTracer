#ifndef RENDER_H
#define RENDER_H

#include <iostream>
#include <fstream>
#include "hittable.h"

class Renderer {
public:
    Renderer() : image_w{400}, image_h{225} {}
    Renderer(const int image_width, const int image_height) : image_w{image_width}, image_h{image_height} {}

    // Sequential ray generation and write to .ppm in P6 format
    void render(const HittableList& world, const Camera& camera) const {
        // Center of first pixel (upper left) will be at the upperleft corner of viewport shifted halfway of a pixel delta
        const coord3 pixel_0_center = camera.viewport_upperleft_corner() + (0.5 * (camera.pixel_delta_u() + camera.pixel_delta_v()));
        std::vector<Color> pixel_colors;
        for (int y = 0; y < image_h; y++) {
            std::clog << "\rRay Progress: " << (static_cast<double>(y) / image_h) * 100 << "% " << std::flush;
            for (int x = 0; x < image_w; x++) {
                // Use pixel's and camera's location to generate a ray
                coord3 pixel_center = pixel_0_center + camera.pixel_delta_u() * static_cast<float>(x) + camera.pixel_delta_v() * static_cast<float>(y);
                Ray ray{camera.position(), unit(pixel_center - camera.position())};
                pixel_colors.push_back(ray_color(ray, world));
            }
        }
        // Done generating rays, write pixel colors to file
        std::clog << "\rRay Progress: 100.0%                         \n" << std::flush;
        write_to_file("image.ppm", image_w, image_h, pixel_colors);
        std::clog << "\rWrote to image.ppm\n" << std::flush;
    }

private:
    int image_w;
    int image_h;

    // Calculate color that should be written (depends on if/what the ray hits)
    static Color ray_color(const Ray& r, const Hittable& world) {
        HitRecord hit_record;
        if (!world.ray_hit(r, Interval(0.f, std::numeric_limits<float>::max()), hit_record)) {   // min_t = 0 so camera effectively looks forwards (not also backwards)
            // Background color light gray gradient dependent on y coord. -1 <= y <= 1, but 0 <= a <= 1 for color = (1 - a) * low_y_color + a * high_y_color
            const float a = 0.5f * (r.direction().y() + 1);
            constexpr auto lightgray{Color{0.9f, 0.9f, 0.9f}};
            constexpr auto gray{Color{0.4f, 0.4f, 0.4f}};
            return (1 - a) * lightgray + a * gray;
        }
        // Color hit object based on its stored materialistic properties
        Material material{hit_record.material()};
        return material.color();
    }

    // Output all image pixel data to a ppm file
    static void write_to_file(const std::string& filename, const int image_width, const int image_height, const std::vector<Color>& pixels) {
        std::ofstream out{filename, std::ios_base::binary};
        if (!out) {
            const std::error_code error{errno, std::generic_category()};
            throw std::runtime_error("Failed to open output file: " + filename + " (" + error.message() + ")");
        }
        out << "P6\n" << image_width << ' ' << image_height << "\n255\n";

        std::vector<uint8_t> outbuf;
        outbuf.reserve(pixels.size() * 3);
        for (const Color& pixel : pixels) {  // 1 byte per color channel
            const float r = pixel.x();
            const float g = pixel.y();
            const float b = pixel.z();
            outbuf.push_back(static_cast<uint8_t>(255.f * r + 0.5f));
            outbuf.push_back(static_cast<uint8_t>(255.f * g + 0.5f));
            outbuf.push_back(static_cast<uint8_t>(255.f * b + 0.5f));
        }
        out.write(reinterpret_cast<const char*>(outbuf.data()), outbuf.size());
    }
};

#endif