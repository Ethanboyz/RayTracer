#include <iostream>
#include <fstream>
#include <memory>
#include "camera.h"
#include "hittable_list.h"
#include "color.h"
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
using std::make_shared;
using std::shared_ptr;
using std::uint8_t;

// Calculate color that should be written (depends on if/what the ray hits)
inline Color ray_color(const Ray& r, const Hittable& world) {
    HitRecord hit_record;
    if (!world.ray_hit(r, Interval(0.f, std::numeric_limits<float>::max()), hit_record)) {   // min_t = 0 so camera effectively looks forwards (not also backwards)
        // Background color light gray gradient dependent on y coord. -1 <= y <= 1, but 0 <= a <= 1 for color = (1 - a) * low_y_color + a * high_y_color
        const float a = 0.5f * (r.direction().y() + 1);
        constexpr auto lightgray{Color{0.9f, 0.9f, 0.9f}};
        constexpr auto gray{Color{0.4f, 0.4f, 0.4f}};
        return (1 - a) * lightgray + a * gray;
    } else {
        // Colored sphere
        return 0.5 * (nounit(hit_record.normal()) + Color{1, 1, 1}); // Prevent negative color components
    }
}

// Output all image pixel data to a ppm file
inline void write_to_file(const std::string& filename, const int image_width, const int image_height, const std::vector<Color>& pixels) {
    std::ofstream out{filename, std::ios_base::binary};
    if (!out) {
        const std::error_code error{errno, std::generic_category()};
        throw std::runtime_error("Failed to open output file: " + filename + " (" + error.message() + ")");
    }
    out << "P6\n" << image_width << ' ' << image_height << "\n255\n";

    std::vector<uint8_t> outbuf;
    outbuf.reserve(static_cast<size_t>(pixels.size() * 3));
    for (size_t i = 0; i < pixels.size(); i++) {  // 1 byte per color channel
        const float r = pixels[i].x();
        const float g = pixels[i].y();
        const float b = pixels[i].z();
        outbuf.push_back(static_cast<uint8_t>(255.f * r + 0.5f));
        outbuf.push_back(static_cast<uint8_t>(255.f * g + 0.5f));
        outbuf.push_back(static_cast<uint8_t>(255.f * b + 0.5f));
    }
    out.write(reinterpret_cast<const char*>(outbuf.data()), outbuf.size());
}

int main() {
    constexpr float fov = 1.f;
    constexpr float aspect_ratio = 16.f/9.f;
    constexpr int image_width = 1000;
    constexpr int image_height = static_cast<int>(static_cast<float>(image_width) / aspect_ratio);
    constexpr float viewport_height = 2.f;

    const auto cam = Camera{vec3{0, 0, 0}, fov, aspect_ratio, image_height, viewport_height};

    // Center of first pixel (upper left) will be at the upperleft corner of viewport shifted halfway a pixel delta
    const coord3 pixel_0_center = cam.viewport_upperleft_corner() + (0.5 * (cam.pixel_delta_u() + cam.pixel_delta_v()));

    // Setup the world and 3d objects
    HittableList world;
    world.add(make_shared<Sphere>(coord3{0, 0, -2}, 0.5f));         // Sphere in front of camera
    world.add(make_shared<Sphere>(coord3{-1, 2, -5}, 0.5f));        // Sphere farther back
    world.add(make_shared<Sphere>(coord3{3, 0.5f, -3}, 0.5f));      // Sphere in front of camera
    world.add(make_shared<Sphere>(coord3{0, -5001, -1}, 5000.f));   // Ground (giant sphere)

    // Sequential ray generation
    std::vector<Color> pixel_colors;
    for (int y = 0; y < image_height; y++) {
        std::clog << "\rRay Progress: " << (static_cast<double>(y) / image_height) * 100 << "% " << std::flush;
        for (int x = 0; x < image_width; x++) {
            // Use pixel's and camera's location to generate a ray
            coord3 pixel_center = pixel_0_center + cam.pixel_delta_u() * static_cast<float>(x) + cam.pixel_delta_v() * static_cast<float>(y);
            Ray ray{cam.position(), unit(pixel_center - cam.position())};
            pixel_colors.push_back(ray_color(ray, world));
        }
    }
    // Done generating rays, write pixel colors to file
    std::clog << "\rRay Progress: 100.0%                         \n" << std::flush;
    write_to_file("image.ppm", image_width, image_height, pixel_colors);
    std::clog << "\rWrote to image.ppm\n" << std::flush;

    return 0;
}
