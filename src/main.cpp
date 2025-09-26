#include <memory>
#include "camera.h"
#include "hittable_list.h"
#include "vec3.h"
#include "sphere.h"
#include "render.h"

// Later on, reorganize header and src files so different light classes can be all included once
#include "light.h"
#include "ambient_light.h"
#include "point_light.h"
#include "directional_light.h"
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
    LightList world_lights;

    Material plastic_red = {{1, 0, 0}, 0.3, 0.1};
    Material shiny_yellow = {{1, 1, 0}, 0.1, 0.9};
    Material dull_blue = {{0, 0, 1}, 0.7, 0.f};
    Material flat_green = {{0, 1, 0}, 0.5, 0.2};
    world.add(make_shared<Sphere>(coord3{0, 0, -2}, 0.5f, plastic_red));            // Sphere in front of camera
    world.add(make_shared<Sphere>(coord3{-1, 2, -5}, 0.5f, shiny_yellow));        // Sphere farther back
    world.add(make_shared<Sphere>(coord3{3, 0.5f, -3}, 0.5f, dull_blue));        // Sphere in front of camera
    world.add(make_shared<Sphere>(coord3{0, -5001, -1}, 5000.f, flat_green));    // Ground (giant sphere)

    //world_lights.add(make_shared<AmbientLight>(0.2f));
    world_lights.add(make_shared<PointLight>(0.7f, coord3{0, 2, 1}));

    const Renderer renderer{image_width, image_height};
    renderer.render(world, world_lights, camera);

    return 0;
}
