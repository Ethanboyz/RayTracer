#include <thread>
#include "rt/render/render.hpp"
#include "rt/utilities.hpp"
#include "rt/math/ray.hpp"
#include "rt/geom/hittable_list.hpp"
#include "rt/math/vec3.hpp"

void Renderer::render(const HittableList& world) const {
    const unsigned ray_threads{std::max(1u, std::thread::hardware_concurrency() - 1)};  // Reserve 1 thread for logging
    std::clog << "This system can support " << ray_threads + 1 << " threads.\n";
    std::vector<std::jthread> threads;
    threads.reserve(ray_threads);

    std::atomic<size_t> next{};     // Batched pixels needing work
    std::atomic<size_t> done{};     // Completed pixels

    const size_t num_pixels = image_width_ * image_height_;
    std::vector<Color> pixel_colors(num_pixels);

    // Thread generation scope
    {
        // Separate thread for logging progress
        std::jthread log_progress([&] {
            using namespace std::chrono_literals;
            while (true) {
                const size_t completed{done.load(std::memory_order_relaxed)};
                if (completed >= num_pixels) {
                    break;
                }
                const double progress{num_pixels ? static_cast<double>(completed) * 100.0 / static_cast<double>(num_pixels) : 100.0};
                std::clog << "\rRay Progress: " << progress << "% " << std::flush;
                std::this_thread::sleep_for(200ms);
            }
            std::clog << "\rRay Progress: 100.0%            \n" << std::flush;
        });

        // Assign all available threads pixels to color
        for (unsigned thread = 0; thread < ray_threads; thread++) {
            threads.emplace_back([&] {
                while (true) {
                    // Each thread should be assigned 32 pixels at a time
                    const size_t start{next.fetch_add(32, std::memory_order_relaxed)};
                    if (start >= num_pixels) {
                        break;
                    }
                    const size_t end{std::min(start + 32, num_pixels)};

                    // Pixels are written from left to right, one row at a time
                    for (size_t i = start; i < end; i++) {
                        const int x{static_cast<int>(i) % image_width_};
                        const int y{static_cast<int>(i) / image_width_};
                        pixel_colors[i] = pixel_color(x, y, world);
                    }

                    // Keep track of the pixels finished
                    done.fetch_add(end - start, std::memory_order_relaxed);
                }
            });
        }
    }
    // Done generating rays, write pixel colors to file
    write_to_file("image.ppm", pixel_colors);
    std::clog << "\rWrote to image.ppm\n" << std::flush;
}

Color Renderer::pixel_color(const int x, const int y, const HittableList& world) const {
    Color pixel_color{0, 0, 0};
    for (int sample = 0; sample < camera_.num_samples(); sample++) {
        Ray ray{generate_ray(x, y)};
        pixel_color += ray_color(ray, 16, world);
    }
    pixel_color /= static_cast<float>(camera_.num_samples());
    return pixel_color;
}

Color Renderer::ray_color(const Ray& ray, const int depth, const Hittable& world) const {
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
    Color attenuation;
    Ray scattered;
    if (hit_record.material().scatter(ray, hit_record, attenuation, scattered)) {
        return attenuation * ray_color(scattered, depth - 1, world);
    }
    return {0, 0, 0};
}
Ray Renderer::generate_ray(const int x, const int y) const {
    // Get a vector to a random point inside the pixel square centered at (i, j)
    const float random_float{Utilities::random_float()};
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
        r = std::pow(std::fabs(r), 1 / gamma);
        g = std::pow(std::fabs(g), 1 / gamma);
        b = std::pow(std::fabs(b), 1 / gamma);
        constexpr Interval color_intensity{0.f, 0.999f};
        outbuf.push_back(static_cast<uint8_t>(256 * color_intensity.clamp(r)));
        outbuf.push_back(static_cast<uint8_t>(256 * color_intensity.clamp(g)));
        outbuf.push_back(static_cast<uint8_t>(256 * color_intensity.clamp(b)));
    }
    out.write(reinterpret_cast<const char*>(outbuf.data()), static_cast<long>(outbuf.size()));
}