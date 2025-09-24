#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>
#include <memory>
#include "sphere.h"
#include "hittable.h"
#include "interval.h"

using std::make_shared;
using std::shared_ptr;

// Replace list with something else like BVH
class HittableList : public Hittable {
public:
    HittableList() = default;
    explicit HittableList(shared_ptr<Hittable> object) { objs.push_back(object); }

    void clear() { objs.clear(); }
    void add(shared_ptr<Hittable> object) { objs.push_back(object); }

    // Returns true if ray hits any Hittable in the HittableList. Populates hit_record with record of the closest hit (lowest t) of the ray.
    bool ray_hit(const Ray& r, const Interval<float>& t, HitRecord& hit_record) const override {
        HitRecord rec;
        bool anything_hit = false;
        float closest_t = t.max();

        for (const shared_ptr<Hittable>& object : objs) {
            if (object->ray_hit(r, Interval(t.min(), closest_t), rec)) {
                anything_hit = true;
                closest_t = rec.t();
                hit_record = rec;
            }
        }
        return anything_hit;
    }

private:
    std::vector<shared_ptr<Hittable>> objs;
};

#endif