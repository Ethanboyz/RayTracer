#include <chrono>
#include <memory>
#include <thread>
#include <random>
#include "rt/geom/bvh.hpp"
#include "rt/render/camera.hpp"
#include "rt/geom/hittable_list.hpp"
#include "rt/math/vec3.hpp"
#include "rt/geom/sphere.hpp"
#include "rt/geom/triangle.hpp"
#include "rt/render/render.hpp"
#include "rt/geom/heightmap.hpp"
#include "terrain/noise/opensimplex2s.hpp"

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
        coord3{0, 1.1, 10},
        coord3{0, 0, 0},
        uvec3{0, 1, 0},
        2.1,
        90,
        0,
        num_samples,
        aspect_ratio,
        image_height
    };
    std::random_device rd;
    uint64_t seed{rd()};
    Utilities::seed_random_generator(seed);
    std::cout << "Seed: " << seed << std::endl;

    // Setup the world and 3d objects
    const Renderer renderer{camera};
    HittableList world;

    Material light {Material::create_light(Color{1.0, 1.0, 1.0}, Emittance{10.0})};
    world.add(make_shared<Sphere>(coord3{0, 1.5, 0},   Radius{0.5}, light));
    /*constexpr float world_medium{1};    // Refraction index of the medium all objects are in (i.e. air ≈ 1)
    Material glass_blue {Material::create_refractive_material(Color{0.0, 0.0, 1.0}, Refraction{0.7}, RefractionIndex{1.5f / world_medium})};*/

    // Noise generation for terrain
    const OpenSimplex2S simplex{seed};
    #ifndef NDEBUG
    renderer.render(simplex, 5);
    #endif

    // Ground (center visible ground around camera)
    constexpr float grid_square_length{0.2};
    constexpr int length{100};
    constexpr int width{200};
    constexpr int norm{std::min(length, width)};
    Heightmap map{
        [&](const double x, const double y){ return simplex.noise2(x * 6 / norm, y * 6 / norm); },
        {-width * grid_square_length / 2, 0, -10},
        grid_square_length,
        length,
        width
    };
    // Construct triangle mesh out of Heightmap
    for (std::vector ground_triangles{map.construct_map()}; const shared_ptr<Triangle>& triangle : ground_triangles) {
        world.add(triangle);
    }

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
