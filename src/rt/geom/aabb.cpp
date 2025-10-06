#include "rt/geom/aabb.hpp"
#include "rt/math/ray.hpp"

bool Aabb::ray_hit(const Ray& ray, Interval<float>& t) const {
    for (int axis{}; axis < 3; axis++) {
        const Interval<float>& bounds{(*this)[axis]};
        const float t0{(bounds.min() - ray.origin()[axis]) / ray.direction()[axis]};
        const float t1{(bounds.max() - ray.origin()[axis]) / ray.direction()[axis]};

        if (t0 < t1) {
            if (t0 > t.min()) { t.min(t0); }
            if (t1 < t.max()) { t.max(t1); }
        } else {
            if (t1 > t.min()) { t.min(t1); }
            if (t0 < t.max()) { t.max(t0); }
        }
        if (t.max() <= t.min()) {
            return false;
        }
    }
    return true;
}