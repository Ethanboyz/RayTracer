#include "rt/geom/aabb.hpp"
#include "rt/math/ray.hpp"

bool Aabb::ray_hit(const Ray& ray, const Interval<float>& t) const {
    for (int axis{}; axis < 3; axis++) {
        const Interval<float>& bounds{(*this)[axis]};
        const float t0{(bounds.min() - ray.origin()[axis]) / ray.direction()[axis]};
        const float t1{(bounds.max() - ray.origin()[axis]) / ray.direction()[axis]};

        // Prune "unused" Interval space
        Interval ray_t{t};
        if (t0 < t1) {
            if (t0 > ray_t.min()) { ray_t.min(t0); }
            if (t1 < ray_t.max()) { ray_t.max(t1); }
        } else {
            if (t1 > ray_t.min()) { ray_t.min(t1); }
            if (t0 < ray_t.max()) { ray_t.max(t0); }
        }
        if (ray_t.max() <= ray_t.min()) {
            return false;
        }
    }
    return true;
}