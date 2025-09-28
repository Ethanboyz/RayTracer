#include "rt/geom/hittable_list.hpp"

bool HittableList::ray_hit(const Ray &ray, const Interval<float> &t, HitRecord &hit_record) const {
    HitRecord rec;
    bool anything_hit{false};
    float closest_t{t.max()};

    for (const shared_ptr<Hittable>& object : objects) {
        if (object->ray_hit(ray, Interval(t.min(), closest_t), rec)) {
            anything_hit = true;
            closest_t = rec.t();
            hit_record = rec;
        }
    }
    return anything_hit;
}