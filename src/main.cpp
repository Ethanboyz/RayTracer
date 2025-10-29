#include <chrono>
#include "args.hpp"
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

int main(int argc, char* argv[]) {
    auto start{std::chrono::steady_clock::now()};

    int num_samples{};             // Increase for more samples = less noise but more compute
    constexpr float aspect_ratio{16.f/9.f};
    constexpr int image_height{1080};

    const run_arguments args{arg_parseopt(argc, argv)};
    uint64_t seed{args.seed};
    num_samples = args.spp;

    Camera camera{
        coord3{0, 1, 19},
        coord3{0, 0, 0},
        uvec3{0, 1, 0},
        2.1,
        90,
        0,
        num_samples,
        aspect_ratio,
        image_height
    };
    Utilities::seed_random_generator(seed);
    std::cout << "Seed: " << seed << std::endl;

    // Setup the world and 3d objects
    const Renderer renderer{camera};
    HittableList world;

    Material light {Material::create_light(Color{1.0, 0.6, 0.5}, Emittance{100.0})};
    world.add(make_shared<Sphere>(coord3{0, 1.1, -10}, Radius{1.5}, light));

    // Noise generation for terrain
    const OpenSimplex2S simplex{seed};
    #ifndef NDEBUG
    constexpr int noise_img_freq{5};
    renderer.render(simplex, noise_img_freq);
    #endif

    // Ground (center visible ground around camera)
    const float grid_square_length{args.triangle_length};       // <= 1, lower -> more triangles
    constexpr int coord_length{20};
    constexpr int coord_width{40};
    constexpr int freq{6};
    const int length{static_cast<int>(coord_length / grid_square_length)};
    const int width{static_cast<int>(coord_width / grid_square_length)};
    constexpr coord3 corner{static_cast<float>(-coord_length), 0, 0};
    const int norm{std::min(length, width)};
    Heightmap map{
        [&](const double x, const double y){ return simplex.noise2(x * freq / norm, y * freq / norm); },
        corner,
        grid_square_length,
        length,
        width
    };
    // Construct triangle mesh out of Heightmap
    for (std::vector ground_triangles{map.construct_map()}; const shared_ptr<Triangle>& triangle : ground_triangles) {
        world.add(triangle);
    }

    // Water at low elevations
    constexpr float world_medium{1};    // Refraction index of the medium all objects are in (i.e. air ≈ 1)
    Material water {Material::create_refractive_material(Color{0.0, 0.0, 1.0}, Refraction{0.4}, RefractionIndex{1.3325f / world_medium})};
    //Material water {Material::create_reflective_material(Color{0.0, 0.0, 1.0}, Reflectance{0.9}, Shininess{0.9})};

    constexpr float sea_level{0};       // -1 for dry, 1 for completely submerged
    constexpr coord3 a{-coord_length, sea_level, corner.z()};
    constexpr coord3 b{coord_length, sea_level, corner.z()};
    constexpr coord3 c{coord_length, sea_level, coord_width};
    constexpr coord3 d{-coord_length, sea_level, coord_width};
    const auto water1{make_shared<Triangle>(Triangle{a, b, c, water})};
    const auto water2{make_shared<Triangle>(Triangle{a, c, d, water})};
    world.add(water1);
    world.add(water2);

    world = HittableList(make_shared<Bvh>(world));          // Put objects into the BVH
    auto checkpoint{std::chrono::steady_clock::now()};
    [[maybe_unused]] auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(checkpoint - start);
    #ifndef NDEBUG
    std::cout << "Setup time: " << duration.count() << " ms" << std::endl;
    #endif

    renderer.render(world);

    auto end{std::chrono::steady_clock::now()};
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - checkpoint);
    std::cout << "Render time: " << duration.count() << " ms" << std::endl;
    return 0;
}
