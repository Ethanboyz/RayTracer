#include <chrono>
#include <memory>
#include <thread>

#include "rt/geom/bvh.hpp"
#include "rt/render/camera.hpp"
#include "rt/geom/hittable_list.hpp"
#include "rt/math/vec3.hpp"
#include "rt/geom/sphere.hpp"
#include "rt/geom/triangle.hpp"
#include "rt/render/render.hpp"
#include "rt/geom/heightmap.hpp"

using std::make_shared;
using std::shared_ptr;
using std::uint8_t;
using namespace std::chrono_literals;

int main() {
    auto start{std::chrono::steady_clock::now()};
    constexpr int num_samples{10000};             // Increase for more samples = less noise but more compute
    constexpr float aspect_ratio{16.f/9.f};
    constexpr int image_height{1080};

    Camera camera{
        coord3{10, 5, 0},
        coord3{0, 0, 0},
        uvec3{0, 1, 0},
        2.1,
        90,
        0,
        num_samples,
        aspect_ratio,
        image_height
    };

    // Setup the world and 3d objects
    HittableList world;
    constexpr float world_medium{1};    // Refraction index of the medium all objects are in (i.e. air ≈ 1)

    Material light      {Material::create_light(Color{1.0, 1.0, 1.0}, Emittance{10.0})};
    Material glass_blue {Material::create_refractive_material(Color{0.0, 0.0, 1.0}, Refraction{0.7}, RefractionIndex{1.5f / world_medium})};
    Material green      {Material::create_reflective_material(Color{0.0, 1.0, 0.0}, Reflectance{1.0}, Shininess{0.0})};

    world.add(make_shared<Sphere>(coord3{-6, 0.5, -6},  Radius{0.5}, light));
    world.add(make_shared<Sphere>(coord3{6, 0.5, -6},   Radius{0.5}, light));
    world.add(make_shared<Sphere>(coord3{6, 0.5, 6},    Radius{0.5}, light));
    world.add(make_shared<Sphere>(coord3{-6, 0.5, 6},   Radius{0.5}, light));
    world.add(make_shared<Sphere>(coord3{0, 1, 0},  Radius{1}, glass_blue));

    // Ground
    Heightmap map{{-10, -1, -10}, 20, 20};
    for (std::vector ground_triangles{map.construct_map(green)}; shared_ptr triangle : ground_triangles) {
        world.add(triangle);
    }

    const Renderer renderer{camera};
    world = HittableList(make_shared<Bvh>(world));          // Put objects into the BVH
    auto checkpoint{std::chrono::steady_clock::now()};
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(checkpoint - start);
    std::cout << "Setup time: " << duration.count() << " ms" << std::endl;

    renderer.render(world);

    auto end{std::chrono::steady_clock::now()};
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - checkpoint);
    std::cout << "Render time: " << duration.count() << " ms" << std::endl;
    return 0;
}
