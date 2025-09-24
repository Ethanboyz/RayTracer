#include <memory>
#include "camera.h"
#include "hittable_list.h"
#include "vec3.h"
#include "sphere.h"
#include "render.h"
using std::make_shared;
using std::shared_ptr;
using std::uint8_t;

int main() {
    constexpr float fov = 1.f;
    constexpr float aspect_ratio = 16.f/9.f;
    constexpr int image_width = 1000;
    constexpr int image_height = static_cast<int>(static_cast<float>(image_width) / aspect_ratio);
    constexpr float viewport_height = 2.f;

    const auto camera = Camera{vec3{0, 0, 0}, fov, aspect_ratio, image_height, viewport_height};

    // Setup the world and 3d objects
    HittableList world;
    Color red = {1, 0, 0};
    Color yellow = {1, 1, 0};
    Color blue = {0, 0, 1};
    Color green = {0, 1, 0};
    world.add(make_shared<Sphere>(coord3{0, 0, -2}, 0.5f, red));            // Sphere in front of camera
    world.add(make_shared<Sphere>(coord3{-1, 2, -5}, 0.5f, yellow));        // Sphere farther back
    world.add(make_shared<Sphere>(coord3{3, 0.5f, -3}, 0.5f, blue));        // Sphere in front of camera
    world.add(make_shared<Sphere>(coord3{0, -5001, -1}, 5000.f, green));    // Ground (giant sphere)

    const Renderer renderer{image_width, image_height};
    renderer.render(world, camera);

    return 0;
}
