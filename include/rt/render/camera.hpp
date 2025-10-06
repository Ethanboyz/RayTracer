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
     * @param focus_distance Distance from Camera where objects are in perfect focus.
     * @param vertical_fov Effective fov in degrees (90 is typically a good default).
     * @param defocus_angle Degrees where "out-of-focus" ray angles vary. Increase for more intense defocus effect.
     * @param num_samples Number of ray samples per pixel (for antialiasing).
     * @param aspect_ratio Width to height ratio of the produced image.
     * @param image_height Height of the image, controls the number of generated rays.
     */
    Camera(
        const coord3& position,
        const coord3& look_at,
        const uvec3& up,
        const float focus_distance,
        const float vertical_fov,
        const float defocus_angle,
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
        focus_distance_     {focus_distance},
        vertical_fov_       {vertical_fov},
        defocus_angle_      {defocus_angle},
        defocus_disk_u_     {u_ * focus_distance_ * std::tan(Utilities::degrees_to_radians(defocus_angle_ / 2))},   // u_ * defocus disk radius
        defocus_disk_v_     {v_ * focus_distance_ * std::tan(Utilities::degrees_to_radians(defocus_angle_ / 2))},   // v_ * defocus disk radius
        num_samples_        {num_samples},
        image_width_        {static_cast<int>(static_cast<float>(image_height) * aspect_ratio)},
        image_height_       {image_height},
        viewport_height_    {2 * std::tan(Utilities::degrees_to_radians(vertical_fov_) / 2)},
        viewport_width_     {viewport_height_ * (static_cast<float>(image_width_) / static_cast<float>(image_height_))},
        viewport_u_         {focus_distance_ * viewport_width_ * u_},
        viewport_v_         {focus_distance_ * viewport_height_ * -v_} {}

    // Accessors
    /** @return Coordinate position of the camera. */
    [[nodiscard]] constexpr coord3 position() const noexcept { return position_; }
    /** @return Distance of the camera where objects are in focus. */
    [[nodiscard]] constexpr float focus_distance() const noexcept { return focus_distance_; }
    /** @return Number of ray samples taken per pixel. */
    [[nodiscard]] constexpr int num_samples() const noexcept { return num_samples_; }
    /** @return Variation angle (degrees) of rays through pixels. */
    [[nodiscard]] constexpr float defocus_angle() const noexcept { return defocus_angle_; }
    /** @return Horizontal radius vector of defocus disk/virtual aperture. */
    [[nodiscard]] constexpr vec3 defocus_disk_u() const noexcept { return defocus_disk_u_; }
    /** @return Vertical radius vector of defocus disk/virtual aperture. */
    [[nodiscard]] constexpr vec3 defocus_disk_v() const noexcept { return defocus_disk_v_; }
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

    // Effects
    float focus_distance_;      // Perfect focus distance
    float vertical_fov_;        // Vertical view angle (in degrees, controls fov)
    float defocus_angle_;       // Variation angle of rays through pixels
    vec3 defocus_disk_u_;       // Horizontal radius vector of defocus disk
    vec3 defocus_disk_v_;       // Vertical radius vector of defocus disk

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
        return position_ - (focus_distance_ * w_) - viewport_u_ / 2 - viewport_v_ / 2;
    }

    /**
     * @Brief Generate a random point on the Camera's defocus disk (virtual aperture).
     *
     * Should be used in the Renderer for generated primary ray origin points to simulate defocus blur.
     * @return Random coordinate on the Camera's defocus disk.
     */
    [[nodiscard]] coord3 defocus_disk_sample() const {
    // Get a random point in the camera defocus disk, then sample a random
        vec3 p;
        while (true) {
            p = vec3{Utilities::random_float({-1, 1}), Utilities::random_float({-1, 1}), 0};
            if (p.length_squared() < 1) {
                break;
            }
        }
        return position_ + p.x() * defocus_disk_u_ + p.y() * defocus_disk_v_;
    }
};

#endif