#include "rt/render/render.hpp"
#include "rt/utilities.hpp"

void Renderer::render(const HittableList& world) const {
    std::vector<Color> pixel_colors;
    for (int y{}; y < image_height_; y++) {
        std::clog << "\rRay Progress: " << static_cast<double>(y) / image_height_ * 100 << "% " << std::flush;
        for (int x{}; x < image_width_; x++) {
            // Use pixel's and camera's location to generate a bunch of rays in the pixel, then write the average of the ray colors
            Color pixel_color{0, 0, 0};
            for (int sample = 0; sample < camera_.num_samples(); sample++) {
                Ray ray{generate_ray(x, y)};
                pixel_color += ray_color(ray, 10, world);
            }
            pixel_color /= static_cast<float>(camera_.num_samples());
            pixel_colors.push_back(pixel_color);
        }
    }
    // Done generating rays, write pixel colors to file
    std::clog << "\rRay Progress: 100.0%                         \n" << std::flush;
    write_to_file("image.ppm", pixel_colors);
    std::clog << "\rWrote to image.ppm\n" << std::flush;
}

Color Renderer::ray_color(const Ray& ray, const int depth, const Hittable& world) {
    HitRecord hit_record;

    if (depth <= 0) {
        return {0, 0, 0};
    }
    // Minimum of t = 0 so camera effectively looks forwards (not also backwards)
    if (!world.ray_hit(ray, Interval{0.001f, std::numeric_limits<float>::max()}, hit_record)) {
        // Background color light gray gradient dependent on y coord. -1 <= y <= 1, but 0 <= a <= 1 for color = (1 - a) * low_y_color + a * high_y_color
        const float a{0.5f * (ray.direction().y() + 1)};
        constexpr auto white{Color{1.f, 1.f, 1.f}};
        constexpr auto light_red{Color{1.f, 0.6f, 0.6f}};
        return (1 - a) * white + a * light_red;
    }

    // Ray-object intersection, generate new child rays in random directions outwards from the surface
    vec3 child_direction{{0.f, 0.1f}};
    if (dot(child_direction, hit_record.normal()) <= 0) {
        child_direction = -child_direction;
    }
    child_direction += hit_record.point();
    return 0.5 * ray_color(Ray{hit_record.point(), unit(child_direction)}, depth - 1, world);
}
Ray Renderer::generate_ray(const int x, const int y) const {
    // Get a vector to a random point inside the pixel square centered at (i, j)
    const float random_float{Utilities::random_float(Interval{0.f, 1.f})};
    const vec3 offset{random_float - 0.5f, random_float - 0.5f, 0};

    const coord3 horizontal_offset{(static_cast<float>(x) + offset.x()) * camera_.pixel_delta_u()};
    const coord3 vertical_offset{(static_cast<float>(y) + offset.y()) * camera_.pixel_delta_v()};
    const coord3 pixel_sample{pixel_0_center_ + horizontal_offset + vertical_offset};

    return {camera_.position(), unit(pixel_sample)};
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
        float r{pixel.x()};
        float g{pixel.y()};
        float b{pixel.z()};

        // Gamma color correction (linear to gamma conversion)
        constexpr float gamma{2.2f};
        r = std::pow(std::abs(r), 1 / gamma);
        g = std::pow(std::abs(g), 1 / gamma);
        b = std::pow(std::abs(b), 1 / gamma);
        constexpr Interval color_intensity{0.f, 0.999f};
        outbuf.push_back(static_cast<uint8_t>(256 * color_intensity.clamp(r)));
        outbuf.push_back(static_cast<uint8_t>(256 * color_intensity.clamp(g)));
        outbuf.push_back(static_cast<uint8_t>(256 * color_intensity.clamp(b)));
    }
    out.write(reinterpret_cast<const char*>(outbuf.data()), static_cast<long>(outbuf.size()));
}