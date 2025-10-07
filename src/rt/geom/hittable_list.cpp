#include "rt/geom/hittable_list.hpp"

// Add object to vector of objects, then add it's bounding box to list's bounding box
void HittableList::add(shared_ptr<Hittable> object) {
    objects_.push_back(object);
    bbox_ = Aabb{bbox_, object->bounding_box()};
}

bool HittableList::ray_hit(const Ray& ray, const Interval<float>& t, HitRecord& hit_record) const {
    HitRecord rec;
    bool anything_hit{false};
    float closest_t{t.max()};

    for (const shared_ptr<Hittable>& object : objects_) {
        if (object->ray_hit(ray, Interval(t.min(), closest_t), rec)) {
            anything_hit = true;
            closest_t = rec.t();
            hit_record = rec;
        }
    }
    return anything_hit;
}