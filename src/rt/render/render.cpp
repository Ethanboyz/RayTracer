#include "rt/render/render.hpp"

void Renderer::render(const HittableList& world, const LightList& world_lights, const Camera& camera) const {
    // Center of first pixel (upper left) will be at the upperleft corner of viewport shifted halfway of a pixel delta
    const coord3 pixel_0_center{camera.viewport_upperleft_corner() + 0.5 * (camera.pixel_delta_u() + camera.pixel_delta_v())};
    std::vector<Color> pixel_colors;
    for (int y{}; y < image_height_; y++) {
        std::clog << "\rRay Progress: " << (static_cast<double>(y) / image_height_) * 100 << "% " << std::flush;
        for (int x{}; x < image_width_; x++) {
            // Use pixel's and camera's location to generate a ray
            coord3 pixel_center{pixel_0_center + camera.pixel_delta_u() * static_cast<float>(x) + camera.pixel_delta_v() * static_cast<float>(y)};
            Ray ray{camera.position(), unit(pixel_center - camera.position())};
            pixel_colors.push_back(ray_color(ray, world, world_lights));
        }
    }
    // Done generating rays, write pixel colors to file
    std::clog << "\rRay Progress: 100.0%                         \n" << std::flush;
    write_to_file("image.ppm", pixel_colors);
    std::clog << "\rWrote to image.ppm\n" << std::flush;
}

Color Renderer::ray_color(const Ray& ray, const Hittable& world, const LightList& world_lights) {
    HitRecord hit_record;

    // Minimum of t = 0 so camera effectively looks forwards (not also backwards)
    if (!world.ray_hit(ray, Interval(0.f, std::numeric_limits<float>::max()), hit_record)) {
        // Background color light gray gradient dependent on y coord. -1 <= y <= 1, but 0 <= a <= 1 for color = (1 - a) * low_y_color + a * high_y_color
        const float a{0.5f * (ray.direction().y() + 1)};
        constexpr auto lightgray{Color{0.9f, 0.9f, 0.9f}};
        constexpr auto gray{Color{0.4f, 0.4f, 0.4f}};
        return (1 - a) * lightgray + a * gray;
    }
    // Color of hit object based on its stored materialistic properties and world lights
    const Material material{hit_record.material()};
    world_lights.light_intensity(hit_record);
    return material.color() * hit_record.light_intensity();
}

void Renderer::write_to_file(const std::string& filename, const std::vector<Color>& pixels) const {
    std::ofstream out{filename, std::ios_base::binary};
    if (!out) {
        const std::error_code error{errno, std::generic_category()};
        throw std::runtime_error("Failed to open output file: " + filename + " (" + error.message() + ")");
    }
    out << "P6\n" << image_width_ << ' ' << image_height_ << "\n255\n";

    std::vector<uint8_t> outbuf;
    outbuf.reserve(pixels.size() * 3);
    for (const Color& pixel : pixels) {  // 1 byte per color channel
        const float r{pixel.x()};
        const float g{pixel.y()};
        const float b{pixel.z()};
        outbuf.push_back(static_cast<uint8_t>(255.999f * r));
        outbuf.push_back(static_cast<uint8_t>(255.999f * g));
        outbuf.push_back(static_cast<uint8_t>(255.999f * b));
    }
    out.write(reinterpret_cast<const char*>(outbuf.data()), static_cast<long>(outbuf.size()));
}