#include <iostream>
#include <fstream>
#include "camera.h"
#include "color.h"
#include "vec3.h"
#include "ray.h"

int main() {
    constexpr float fov = 1.f;
    constexpr float aspect_ratio = 16.f/9.f;
    constexpr int image_width = 400;
    constexpr int image_height = static_cast<int>(static_cast<float>(image_width) / aspect_ratio);
    constexpr float viewport_height = 2.f;

    auto cam = Camera{Vec3{0, 0, 0}, fov, aspect_ratio, image_height, viewport_height};

    // Redirect output to new .ppm img file
    std::ofstream file("image.ppm");
    std::streambuf* cout_buff = std::cout.rdbuf();
    std::cout.rdbuf(file.rdbuf());

    // Center of first pixel (upper left) will be at the upperleft corner of viewport shifted halfway a pixel delta
    Coord3 pixel_0_center = cam.viewport_upperleft_corner() + (0.5 * (cam.pixel_delta_u() + cam.pixel_delta_v()));

    // Sequential ray generation
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int y = 0; y < image_height; y++) {
        std::clog << "\rProgress: " << (static_cast<double>(y) / image_height) * 100 << "% " << std::flush;
        for (int x = 0; x < image_width; x++) {
            // Use pixel's and camera's location to generate a ray
            Coord3 pixel_center = pixel_0_center + cam.pixel_delta_u() * static_cast<float>(x) + cam.pixel_delta_v() * static_cast<float>(y);
            Vec3 ray_dir = pixel_center - cam.position();

            [[maybe_unused]]
            Ray ray{cam.position(), ray_dir};   // Doesn't do anything yet

            // Finally magic
            write_color(std::cout, {0, 0, 0});
        }
    }

    // Done rendering
    std::clog << std::endl;
    std::cout.rdbuf(cout_buff);
    file.close();

    return 0;
}
