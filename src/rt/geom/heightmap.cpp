#include "rt/geom/heightmap.hpp"

#include "rt/utilities.hpp"
#include "rt/geom/triangle.hpp"

using std::make_shared;

// For each quad (square of vertices), construct two triangles
std::vector<shared_ptr<Triangle>> Heightmap::construct_map() const {
    std::vector<shared_ptr<Triangle>> triangles;
    int vertex = 0;
    // Iterate through each vertex one width at a time (each iterated vertex is the upper left corner of a quad)
    for (int z = 0; z < length_ - 1; z++) {
        for (int x = 0; x < width_ - 1; x++) {
            // Construct two triangles that make up the grid square
            const float left_x{grid_square_len_ * static_cast<float>(x) + corner_.x()};
            const float right_x{grid_square_len_ * static_cast<float>(x + 1) + corner_.x()};
            const float up_z{grid_square_len_ * static_cast<float>(z) + corner_.z()};
            const float low_z{grid_square_len_ * static_cast<float>(z + 1) + corner_.z()};

            const coord3 up_left{left_x, vertices_heights_[vertex], up_z};
            const coord3 up_right{right_x, vertices_heights_[vertex + 1], up_z};
            const coord3 low_left{left_x, vertices_heights_[vertex + width_], low_z};
            const coord3 low_right{right_x, vertices_heights_[vertex + width_ + 1], low_z};

            const float a{up_left.y()};
            const Color color{(1.f - a) * Color{0.0, 1.0, 0.0} + a * Color{0.859, 0.580, 0.271}};
            const Color color1{color * Utilities::random_float(Interval{0.7f, 1.f})};
            const Material material1{
                Material::create_reflective_material(
                    color1,
                    Reflectance{1.0},
                    Shininess{0.0}
                    )};
            const Color color2{color * Utilities::random_float(Interval{0.7f, 1.f})};
            const Material material2{
                Material::create_reflective_material(
                    color2,
                    Reflectance{1.0},
                    Shininess{0.0}
                )};
            Triangle upper_left{up_left, up_right, low_left, material1};
            Triangle lower_right{up_right, low_left, low_right, material2};
            triangles.push_back(make_shared<Triangle>(upper_left));
            triangles.push_back(make_shared<Triangle>(lower_right));
            vertex++;
        }
        vertex++;
    }
    return triangles;
}