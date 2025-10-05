#ifndef CAMERA_H
#define CAMERA_H

#include <cassert>
#include "rt/math/vec3.hpp"

/**
 * @class Camera
 * @brief Defines the perspective in which the world is viewed.
 */
class Camera {
public:
    /**
     * @brief Constructs a Camera that determines the viewable perspective based on the defined parameters.
     * @param position Coordinate position of the Camera.
     * @param look_at Coordinate where the Camera is pointing.
     * @param up Camera's up direction vector.
     * @param vertical_fov Effective fov in degrees (90 is typically a good default).
     * @param num_samples Number of ray samples per pixel (for antialiasing).
     * @param aspect_ratio Width to height ratio of the produced image.
     * @param image_height Height of the image, controls the number of generated rays.
     */
    Camera(
        const coord3& position,
        const coord3& look_at,
        const uvec3& up,
        const float vertical_fov,
        const int num_samples,
        const float aspect_ratio,
        const int image_height
    ) :
    position_           {position},
    look_at_            {look_at},
    up_                 {up},
    w_                  {unit(position - look_at)},
    u_                  {unit(cross(up, w_))},
    v_                  {unit(cross(w_, u_))},
    focal_len_          {(position - look_at).length()},
    vertical_fov_       {vertical_fov},
    num_samples_        {num_samples},
    image_width_        {static_cast<int>(static_cast<float>(image_height) * aspect_ratio)},
    image_height_       {image_height},
    viewport_height_    {2 * std::tan(static_cast<float>(vertical_fov_ * (M_PI / 180.0)) / 2)},    // 2 * tan(R/2) * focal_len
    viewport_width_     {viewport_height_ * (static_cast<float>(image_width_) / static_cast<float>(image_height_))},
    viewport_u_         {focal_len_ * viewport_width_ * u_},
    viewport_v_         {focal_len_ * viewport_height_ * -v_} {}

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
    // Camera placement
    coord3 position_;           // Camera position
    coord3 look_at_;            // Look at this coord
    uvec3 up_;                  // Vertical (up) vector
    uvec3 w_, u_, v_;           // Camera coordinate frame

    float focal_len_;           // Controls fov
    float vertical_fov_;        // Vertical view angle (in degrees, controls fov)
    int num_samples_;           // Samples per pixel for antialiasing
    int image_width_;
    int image_height_;
    float viewport_height_;
    float viewport_width_;
    vec3 viewport_u_;           // Viewport width vec3 representation
    vec3 viewport_v_;           // Viewport height vec3 representation

    friend class Renderer;      // Only the renderer will need the pixel delta functions

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
        return position_ - (focal_len_ * w_) - viewport_u_ / 2 - viewport_v_ / 2;
    }
};

#endif