#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>
#include <memory>
#include "rt/geom/hittable.hpp"
#include "rt/geom/aabb.hpp"

using std::make_shared;
using std::shared_ptr;

/**
 * @class HittableList
 * @brief Stores a list of Hittable objects.
 *
 * This can be passed into the renderer to render all contained objects.
 */
class HittableList final : public Hittable {
public:
    /** @brief Constructs an empty HittableList. */
    constexpr HittableList() = default;

    /** @brief Clears the HittableList of all stored objects. */
    void clear() { objects_.clear(); }

    /**
     * @brief Adds a new Hittable to the current HittableList.
     * @param object Hittable object to be added.
     */
    void add(shared_ptr<Hittable> object);

    /** @return Aabb that encompasses the objects in the HittableList. */
    [[nodiscard]] Aabb bounding_box() const override { return bbox_; }

    /**
     * @brief Finds the smallest t of a ray with record of the closest hit (lowest t) of the ray.
     * @param ray Checked for intersections with the current Hittable object.
     * @param t Intersections only count if they occur in the specified t Interval.
     * @param hit_record Updated with hit information if ray intersection occurs.
     * @return True if ray intersects any Hittable in the HittableList, false otherwise.
     */
    bool ray_hit(const Ray& ray, const Interval<float>& t, HitRecord& hit_record) const override;

private:
    std::vector<shared_ptr<Hittable>> objects_;
    Aabb bbox_;
};

#endif