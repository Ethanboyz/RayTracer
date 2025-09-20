#include <iostream>
#include <fstream>
#include "camera.h"
#include "color.h"
#include "vec3.h"
#include "ray.h"

int main() {
    float aspect_ratio = 16.f/9.f;
    int image_width = 400;
    int image_height = static_cast<int>(static_cast<float>(image_width) / aspect_ratio);
    float viewport_height = 2.f;
    float viewport_width = viewport_height * static_cast<float>(static_cast<float>(image_width) / static_cast<float>(image_height));

    std::ofstream file("image.ppm");
    std::streambuf* cout_buff = std::cout.rdbuf();
    std::cout.rdbuf(file.rdbuf());

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = 0; j < image_height; j++) {
        std::clog << "\rProgress: " << (static_cast<double>(j) / image_height) * 100 << "% " << std::flush;
        for (int i = 0; i < image_width; i++) {

        }
    }

    std::clog << std::endl;
    std::cout.rdbuf(cout_buff);
    file.close();

    return 0;
}
