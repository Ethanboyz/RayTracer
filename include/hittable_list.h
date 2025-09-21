#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <vector>
#include <memory>

using std::make_shared;
using std::shared_ptr;

// Replace list with something else like BVH
class HittableList : Hittable {
public:
    HittableList() {}
    HittableList(shared_ptr<Hittable> object) { objs.push_back(object); }

    void clear() { objs.clear(); }
    void add(shared_ptr<Hittable> object) { objs.push_back(object); }

    bool ray_hit(const Ray &r, const float min_t, const float max_t, HitRecord &hit_record) const override {
        HitRecord rec;
        bool anything_hit = false;

        for (const shared_ptr<Hittable>& object : objs) {
            if (object->ray_hit(r, min_t, max_t, rec)) {
                anything_hit = true;
                hit_record = rec;
            }
        }
        return anything_hit;
    }

private:
    std::vector<shared_ptr<Hittable>> objs;
};

#endif