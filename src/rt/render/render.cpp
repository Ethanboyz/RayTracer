#include <thread>
#include <format>
#include "rt/render/render.hpp"
#include "rt/utilities.hpp"
#include "rt/math/ray.hpp"
#include "rt/geom/hittable_list.hpp"
#include "rt/math/vec3.hpp"

static constexpr int ASSIGN_PIXELS{32};                       // Work (number of pixels) to assign at a time to a ray/worker thread
static constexpr int RAY_DEPTH{16};                           // Max number of ray bounces per ray
static constexpr Color BACKGROUND_COLOR{0.01, 0.01, 0.01};       // Effective ambient color

// Multithreaded pixel handling
void Renderer::render(const HittableList& world) const {
    const unsigned ray_threads{std::max(1u, std::thread::hardware_concurrency() - 1)};  // Reserve 1 thread for logging
    std::cout << "This system can support " << ray_threads + 1 << " threads." << std::endl;

    std::atomic<size_t> next{};     // Batched pixels needing work
    std::atomic<size_t> done{};     // Completed pixels

    const size_t num_pixels = image_width_ * image_height_;
    std::vector<Color> pixel_colors(num_pixels);

    // Thread generation scope
    {
        std::vector<std::jthread> threads;
        threads.reserve(ray_threads);

        // Separate thread for logging progress
        std::jthread log_progress([&]() {
            using namespace std::chrono_literals;

            constexpr int bar_width = 80;
            while (true) {
                const size_t completed = done.load(std::memory_order_relaxed);
                const double progress = (num_pixels == 0) ? 1.0 : std::min(static_cast<double>(completed) / static_cast<double>(num_pixels), 1.0);

                if (progress >= 1.0) {
                    break;
                }
                const int filled_bar = static_cast<int>(std::clamp(progress, 0.0, 0.999) * (bar_width + 1));

                std::string bar(bar_width, '.');
                std::fill_n(bar.begin(), std::min<std::size_t>(filled_bar, bar.size()), '#');
                const std::string line = std::format("[{}] {:6.2f}%", bar, progress * 100.0);
                std::cout << "\x1b[2K\r" << line << std::flush;

                std::this_thread::sleep_for(100ms);
            }
            // Progress completed
            std::cout << "\x1b[2K\r" << std::format("[{}] {:6.2f}%", std::string(bar_width, '#'), 100.0) << std::endl;
        });

        // Assign all available threads pixels to color
        for (unsigned thread = 0; thread < ray_threads; thread++) {
            threads.emplace_back([&] {
                while (true) {
                    // Each thread should be assigned 32 pixels at a time
                    const size_t start{next.fetch_add(ASSIGN_PIXELS, std::memory_order_relaxed)};
                    if (start >= num_pixels) {
                        break;
                    }
                    const size_t end{std::min(start + ASSIGN_PIXELS, num_pixels)};

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
    std::cout << "\rWrote to image.ppm" << std::endl;
}

Color Renderer::pixel_color(const int x, const int y, const HittableList& world) const {
    Color pixel_color{0, 0, 0};
    for (int sample = 0; sample < camera_.num_samples(); sample++) {
        Ray ray{generate_ray(x, y)};
        pixel_color += ray_color(ray, RAY_DEPTH, world);
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
        return BACKGROUND_COLOR;
    }

    // Ray-object intersection, generate new child rays in random directions outwards from the surface
    const Color color_from_emission{hit_record.emitted()};
    Color attenuation{};
    Ray next;
    if (!hit_record.bounce(ray, attenuation, next)) {
        return color_from_emission;
    }

    return attenuation * ray_color(next, depth - 1, world) + color_from_emission;
}

Ray Renderer::generate_ray(const int x, const int y) const {
    // Get a vector to a random point inside the pixel square centered at (i, j)
    const float random_x{Utilities::random_float()};
    const float random_y{Utilities::random_float()};
    const vec3 offset{random_x - 0.5f, random_y - 0.5f, 0};

    const coord3 horizontal_offset{(static_cast<float>(x) + offset.x()) * camera_.pixel_delta_u()};
    const coord3 vertical_offset{(static_cast<float>(y) + offset.y()) * camera_.pixel_delta_v()};
    const coord3 pixel_sample{pixel_0_center_ + horizontal_offset + vertical_offset};

    const coord3 ray_origin{camera_.defocus_angle() <= 0 ? camera_.position() : camera_.defocus_disk_sample()};
    return {ray_origin, unit(pixel_sample - ray_origin)};
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