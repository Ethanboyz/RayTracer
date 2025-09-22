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

// Calculate color that should be written (depends on if/what the ray hits)
inline Color ray_color(const Ray& r, const Hittable& world) {
    HitRecord hit_record;
    if (!world.ray_hit(r, 0, std::numeric_limits<float>::max(), hit_record)) {   // min_t = 0 so camera effectively looks forwards (not also backwards)
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

int main() {
    constexpr float fov = 1.f;
    constexpr float aspect_ratio = 16.f/9.f;
    constexpr int image_width = 1000;
    constexpr int image_height = static_cast<int>(static_cast<float>(image_width) / aspect_ratio);
    constexpr float viewport_height = 2.f;

    auto cam = Camera{vec3{0, 0, 0}, fov, aspect_ratio, image_height, viewport_height};

    // Redirect output to new .ppm img file
    std::ofstream file("image.ppm");
    std::streambuf* cout_buff = std::cout.rdbuf();
    std::cout.rdbuf(file.rdbuf());

    // Center of first pixel (upper left) will be at the upperleft corner of viewport shifted halfway a pixel delta
    coord3 pixel_0_center = cam.viewport_upperleft_corner() + (0.5 * (cam.pixel_delta_u() + cam.pixel_delta_v()));

    // Setup the world and 3d objects
    HittableList world;
    world.add(make_shared<Sphere>(coord3{0, 0, -2}, 0.5f));    // Sphere in front of camera
    world.add(make_shared<Sphere>(coord3{-1, 2, -5}, 0.5f));    // Sphere farther back
    world.add(make_shared<Sphere>(coord3{3, 0.5f, -3}, 0.5f));    // Sphere in front of camera
    world.add(make_shared<Sphere>(coord3{0, -100.5, -1}, 100.f)); // Ground (giant sphere)

    // Sequential ray generation
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int y = 0; y < image_height; y++) {
        std::clog << "\rProgress: " << (static_cast<double>(y) / image_height) * 100 << "% " << std::flush;
        for (int x = 0; x < image_width; x++) {
            // Use pixel's and camera's location to generate a ray
            coord3 pixel_center = pixel_0_center + cam.pixel_delta_u() * static_cast<float>(x) + cam.pixel_delta_v() * static_cast<float>(y);
            Ray ray{cam.position(), unit(pixel_center - cam.position())};
            write_color(std::cout, ray_color(ray, world));
        }
    }

    // Done rendering
    std::clog << "\rProgress: 100.0%                         " << std::flush;
    std::clog << std::endl;
    std::cout.rdbuf(cout_buff);
    file.close();

    return 0;
}
