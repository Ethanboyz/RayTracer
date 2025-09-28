#include <memory>
#include "rt/render/camera.hpp"
#include "rt/geom/hittable_list.hpp"
#include "rt/math/vec3.hpp"
#include "rt/geom/sphere.hpp"
#include "rt/render/render.hpp"

#include "rt/lighting/ambient_light.hpp"
#include "rt/lighting/point_light.hpp"
#include "rt/lighting/directional_light.hpp"
using std::make_shared;
using std::shared_ptr;
using std::uint8_t;

int main() {
    constexpr float fov = 1.f;
    constexpr float aspect_ratio = 16.f/9.f;
    constexpr int image_width = 1000;
    constexpr int image_height = static_cast<int>(static_cast<float>(image_width) / aspect_ratio);
    constexpr float viewport_height = 2.f;

    constexpr auto camera = Camera{vec3{0, 0, 0}, fov, aspect_ratio, image_height, viewport_height};

    // Setup the world and 3d objects
    HittableList world;
    LightList world_lights;

    Material plastic_red = {{1, 0, 0}, 0.3, 0.1};
    Material shiny_yellow = {{1, 1, 0}, 0.1, 0.9};
    Material dull_blue = {{0, 0, 1}, 0.7, 0.f};
    Material flat_green = {{0, 1, 0}, 0.5, 0.2};
    world.add(make_shared<Sphere>(coord3{0, 0, -2}, 0.5f, plastic_red));            // Sphere in front of camera
    world.add(make_shared<Sphere>(coord3{-1, 2, -5}, 0.5f, shiny_yellow));          // Sphere farther back
    world.add(make_shared<Sphere>(coord3{3, 0.5f, -3}, 0.5f, dull_blue));           // Sphere in front of camera
    world.add(make_shared<Sphere>(coord3{0, -5001, -1}, 5000.f, flat_green));       // Ground (giant sphere)

    world_lights.add(make_shared<AmbientLight>(0.2f));
    world_lights.add(make_shared<PointLight>(4, coord3{-1.5, 3, -4}));
    world_lights.add(make_shared<DirectionalLight>(1, uvec3{0, 2, 4}));

    const Renderer renderer{image_width, image_height};
    renderer.render(world, world_lights, camera);

    return 0;
}
