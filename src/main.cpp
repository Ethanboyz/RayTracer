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
    constexpr int num_samples{100};             // Increase for more samples = less noise but more compute
    constexpr float aspect_ratio{16.f/9.f};
    constexpr int image_height{1080};

    Camera camera{
        coord3{-2, 2, 1},
        coord3{0, 0, -2},
        uvec3{0, 1, 0},
        3.4,
        50,
        5,
        num_samples,
        aspect_ratio,
        image_height
    };

    // Setup the world and 3d objects
    HittableList world;
    constexpr float world_medium{1};    // Refraction index of the medium all objects are in (i.e. air ≈ 1)

    Material smooth_red {Material::create_reflective_material(Color{1.0, 0.0, 0.0}, Reflectance{0.6}, Shininess{0.0})};
    Material shiny      {Material::create_reflective_material(Color{0.8, 0.8, 0.4}, Reflectance{1.0}, Shininess{0.7})};
    Material flat_green {Material::create_reflective_material(Color{0.0, 1.0, 0.0}, Reflectance{0.5}, Shininess{0.2})};
    Material glass_blue {Material::create_refractive_material(Color{0.0, 0.0, 1.0}, Refraction{0.7}, RefractionIndex{1.5f / world_medium})};

    world.add(make_shared<Sphere>(coord3{0.0, 0.0, -2.0},   Radius{0.5}, smooth_red));
    world.add(make_shared<Sphere>(coord3{0.7, -0.2, -1.0},  Radius{0.5}, shiny));
    world.add(make_shared<Sphere>(coord3{-1.5, 0.5, -3.0},  Radius{0.5}, smooth_red));
    world.add(make_shared<Sphere>(coord3{0.0, -901, -1.0},  Radius{900}, flat_green));
    world.add(make_shared<Sphere>(coord3{-0.7, -0.2, -1.5}, Radius{0.5}, glass_blue));

    const Renderer renderer{camera};
    renderer.render(world);

    return 0;
}
