#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"

class Camera {

public:
    Camera(
        const coord3& position,
        const float fov,
        const float aspect_ratio,
        const int image_height,
        const float viewport_height
    ) :
    pos{position},
    fov{fov},
    image_h{image_height},
    viewport_h{viewport_height} {
        image_w = static_cast<int>(static_cast<float>(image_h) * aspect_ratio);
        viewport_w = viewport_h * (static_cast<float>(image_w) / static_cast<float>(image_h));
        viewport_u = vec3{viewport_w, 0.f, 0.f};
        viewport_v = vec3{0.f, -viewport_h, 0.f};
    }

    constexpr coord3 position() const noexcept { return pos; }
    constexpr float focal_len() const noexcept { return fov; }

    vec3 pixel_delta_u() const {
        return viewport_u / static_cast<float>(image_w);
    }
    vec3 pixel_delta_v() const {
        return viewport_v / static_cast<float>(image_h);
    }
    // Position of upperleft corner of viewport (hardcoded straight down the Z axis)
    coord3 viewport_upperleft_corner() const {
        return pos + vec3{0.f, 0.f, -fov} - viewport_u / 2 - viewport_v / 2;
    }

private:
    coord3 pos;             // Camera position
    float fov;              // Focus length, controls fov
    int image_w;            // Img width
    int image_h;            // Img height
    float viewport_w;       // Viewport width
    float viewport_h;       // Viewport height
    vec3 viewport_u;        // Viewport width vec representation
    vec3 viewport_v;        // Viewport height vec representation
};

#endif