#include <memory>
#include "rt/render/camera.hpp"
#include "rt/geom/hittable_list.hpp"
#include "rt/math/vec3.hpp"
#include "rt/geom/sphere.hpp"
#include "rt/render/render.hpp"

using std::make_shared;
using std::shared_ptr;
using std::uint8_t;

int main() {
    constexpr float fov = 1.f;
    constexpr int num_samples = 1000;             // Increase for more samples = less noise but more compute
    constexpr float aspect_ratio = 16.f/9.f;
    constexpr int image_width = 1000;
    constexpr int image_height = static_cast<int>(static_cast<float>(image_width) / aspect_ratio);
    constexpr float viewport_height = 2.f;

    constexpr auto camera = Camera{vec3{0, 0, 0}, fov, num_samples, aspect_ratio, image_height, viewport_height};

    // Setup the world and 3d objects
    HittableList world;

    Material plastic_red = {{1, 0, 0}, 0.f, 0.6f, 0.1f};
    Material shiny_yellow = {{1, 1, 0}, 0.f, 1.f, 1.f};
    Material dull_blue = {{0, 0, 1}, 0.f, 0.4, 0.f};
    Material flat_green = {{0, 1, 0}, 0.f, 0.5, 0.2};
    world.add(make_shared<Sphere>(coord3{0, 0, -2}, 0.5f, plastic_red));
    world.add(make_shared<Sphere>(coord3{1, 0, -2}, 0.5f, shiny_yellow));
    world.add(make_shared<Sphere>(coord3{3, 0.5f, -3}, 0.5f, dull_blue));
    world.add(make_shared<Sphere>(coord3{0, -1001, -1}, 1000.f, flat_green));

    const Renderer renderer{camera};
    renderer.render(world);

    return 0;
}
