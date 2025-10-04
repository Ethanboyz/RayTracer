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
    constexpr int num_samples = 7000;             // Increase for more samples = less noise but more compute
    constexpr float aspect_ratio = 16.f/9.f;
    constexpr int image_width = 1000;
    constexpr int image_height = static_cast<int>(static_cast<float>(image_width) / aspect_ratio);
    constexpr float viewport_height = 2.f;

    constexpr auto camera = Camera{vec3{0, 0, 0}, fov, num_samples, aspect_ratio, image_height, viewport_height};

    // Setup the world and 3d objects
    HittableList world;

    constexpr float world_medium{1};    // Refraction index of the medium all objects are in (i.e. air ≈ 1)
    Material plastic_red = {{1, 0, 0}, 0, 0.6, 0, 0, 0};
    Material shiny = {{0.8, 0.8, 0.4}, 0, 1, 1, 0, 0};
    Material glass_blue = {{0, 0, 1}, 0, 0, 0, 1, 1.5f / world_medium};
    Material flat_green = {{0, 1, 0}, 0, 0.5, 0.2, 0, 0};
    world.add(make_shared<Sphere>(coord3{0, 0, -2}, 0.5, plastic_red));
    world.add(make_shared<Sphere>(coord3{0.7, -0.2, -1.5}, 0.5, shiny));
    world.add(make_shared<Sphere>(coord3{-0.7, -0.2, -1.5}, 0.5, glass_blue));
    world.add(make_shared<Sphere>(coord3{-1.5, 0.5, -3}, 0.5, plastic_red));
    world.add(make_shared<Sphere>(coord3{0, -1001, -1}, 1000, flat_green));

    const Renderer renderer{camera};
    renderer.render(world);

    return 0;
}
