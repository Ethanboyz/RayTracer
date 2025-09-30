#ifndef CAMERA_H
#define CAMERA_H

#include "rt/math/vec3.hpp"

/**
 * @class Camera
 * @brief Defines the perspective in which the world is viewed.
 */
class Camera {
public:
    /**
     * @brief Constructs a camera that determines the viewable perspective based on the defined parameters.
     * @param position Coordinate position of the camera.
     * @param focal_len Acts as the effective FOV.
     * @param num_samples Number of ray samples per pixel (for antialiasing).
     * @param aspect_ratio Width to height ratio of the produced image.
     * @param image_height Height of the image, controls the number of generated rays.
     * @param viewport_height Height of the viewport, controls the geometric size of the completed render.
     */
    constexpr Camera(
        const coord3& position,
        const float focal_len,
        const int num_samples,
        const float aspect_ratio,
        const int image_height,
        const float viewport_height
    ) :
    position_{position},
    focal_len_{focal_len},
    num_samples_{num_samples},
    image_height_{image_height},
    viewport_height_{viewport_height} {
        image_width_ = static_cast<int>(static_cast<float>(image_height_) * aspect_ratio);
        viewport_width_ = viewport_height_ * (static_cast<float>(image_width_) / static_cast<float>(image_height_));
        viewport_u_ = vec3{viewport_width_, 0.f, 0.f};
        viewport_v_ = vec3{0.f, -viewport_height_, 0.f};
    }

    // Accessors
    /** @return Coordinate position of the camera. */
    [[nodiscard]] constexpr coord3 position() const noexcept { return position_; }
    /** @return Focal length (FOV) of the camera. */
    [[nodiscard]] constexpr float focal_len() const noexcept { return focal_len_; }
    /** @return Number of ray samples taken per pixel. */
    [[nodiscard]] constexpr int num_samples() const noexcept { return num_samples_; }
    /** @return Width of the image. */
    [[nodiscard]] constexpr int image_width() const noexcept { return image_width_; }
    /** @return Height of the image. */
    [[nodiscard]] constexpr int image_height() const noexcept { return image_height_; }

private:
    coord3 position_;           // Camera position
    float focal_len_;           // Controls fov
    int num_samples_;           // Samples per pixel for antialiasing
    int image_width_;
    int image_height_;
    float viewport_width_;
    float viewport_height_;
    vec3 viewport_u_;           // Viewport width vec3 representation
    vec3 viewport_v_;           // Viewport height vec3 representation

    friend class Renderer;  // Only the renderer will need the pixel delta functions

    /** @return Horizontal distance between each viewport pixel. */
    [[nodiscard]] constexpr vec3 pixel_delta_u() const {
        return viewport_u_ / static_cast<float>(image_width_);
    }

    /** @return Vertical distance between each viewport pixel. */
    [[nodiscard]] constexpr vec3 pixel_delta_v() const {
        return viewport_v_ / static_cast<float>(image_height_);
    }

    /**
     * @brief Calculates the location of the upperleft corner of the viewport.
     *
     * Use in conjunction with pixel_delta_u() and pixel_delta_v() to find the position of the other viewport pixels.
     * @return Position of upperleft corner of viewport (hardcoded straight down the Z axis).
     */
    [[nodiscard]] constexpr coord3 viewport_upperleft_corner() const noexcept {
        return position_ + vec3{0.f, 0.f, -focal_len_} - viewport_u_ / 2 - viewport_v_ / 2;
    }
};

#endif