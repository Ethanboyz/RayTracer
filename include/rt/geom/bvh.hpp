#ifndef AABB_TREE_NODE_H
#define AABB_TREE_NODE_H

#include <algorithm>
#include <memory>
#include "rt/geom/hittable.hpp"
#include "rt/geom/hittable_list.hpp"
#include "rt/geom/aabb.hpp"
#include "rt/utilities.hpp"

using std::shared_ptr;
using std::fabs;
using Utilities::random_int;

/**
 * @class Bvh
 * @brief Implementation of a BVH consisting of a binary tree of Aabb nodes, where each node contains one Aabb, and leaves contain the primitives.
 *
 * Each object can be treated as the root of its own tree.
 */
class Bvh final : public Hittable {
    static constexpr int MAX_LEAF{2};           // Max number of primitives per binary set of children (keep as 2 until robust Leaf structure is made)
    static constexpr int NUM_BINS{16};          // Number of bins/buckets to divide axis into
    static constexpr int INTERSECT_COST{1};     // Cost of ray-primitive intersection
    static constexpr int TRAVERSAL_COST{1};     // Cost of "traversal" from node to children
public:
    explicit Bvh(HittableList list) : Bvh(list.objects(), 0, list.size()) {}

    /**
     * @brief Construct a new BVH node.
     *
     * BVH nodes works as a one-class binary tree built with a simple object-splitting heuristics algorithm.
     * @param primitives Objects to be stored in the tree leaves.
     * @param start Start index of objects.
     * @param end End index of objects.
     */
    Bvh(std::vector<shared_ptr<Hittable>>& primitives, size_t start, size_t end) {
        const int axis{random_int(Interval{0, 2})};
        const auto comparator{axis == 0 ? box_x_compare :
                                        axis == 1 ? box_y_compare :
                                        box_z_compare};

        const size_t range{end - start};
        if (range == 1) {
            left_ = primitives[start];
            right_ = primitives[start];
        } else if (range == 2) {
            left_ = primitives[start];
            right_ = primitives[start + 1];
        } else {
            std::sort(std::begin(primitives) + start, std::begin(primitives) + end, comparator);
            size_t mid{start + range / 2};
            left_ = make_shared<Bvh>(primitives, start, mid);
            right_ = make_shared<Bvh>(primitives, mid, end);
        }
        bbox_ = Aabb{left_->bounding_box(), right_ ? right_->bounding_box() : Aabb{}};
    }

    /**
     * @brief Populates hit_record with Ray-Hittable intersect info if ray intersects the current AABB bounding box.
     * @param ray Checked for intersections with the current AABB.
     * @param t Intersections only count if they occur in the specified t Interval.
     * @param hit_record Updated with hit information of smallest t if ray intersection occurs.
     * @return True if ray intersects the current AABB, false otherwise.
     */
    bool ray_hit(const Ray& ray, const Interval<float>& t, HitRecord& hit_record) const override {
        if (!bbox_.ray_hit(ray, t)) {
            return false;
        }

        const bool hit_left{left_->ray_hit(ray, t, hit_record)};
        const bool hit_right{right_->ray_hit(ray, Interval{t.min(), hit_left ? hit_record.t() : t.max()}, hit_record)};
        return hit_left || hit_right;
    }

    /** @return Axis-aligned bounding box of this node. */
    [[nodiscard]] Aabb bounding_box() const override { return bbox_; }

private:
    // Children can be another BVH node or a leaf (renderable objects)
    shared_ptr<Hittable> left_;
    shared_ptr<Hittable> right_;

    Aabb bbox_;

    // Comparator functions
    /** @return True if box a's specified axis Interval has a smaller min compared to box b's corresponding axis, false if equal/greater */
    static bool box_compare(const shared_ptr<Hittable>& a, const shared_ptr<Hittable>& b, const int axis_index) {
        const Interval a_axis{a->bounding_box()[axis_index]};
        const Interval b_axis{b->bounding_box()[axis_index]};
        return a_axis.min() < b_axis.min();
    }

    /** @return True if box a's x-axis Interval has a smaller min compared to box b's x-axis, false if equal/greater */
    static bool box_x_compare(const shared_ptr<Hittable>& a, const shared_ptr<Hittable>& b) {
        return box_compare(a, b, 0);
    }

    /** @return True if box a's y-axis Interval has a smaller min compared to box b's y-axis, false if equal/greater */
    static bool box_y_compare(const shared_ptr<Hittable>& a, const shared_ptr<Hittable>& b) {
        return box_compare(a, b, 1);
    }

    /** @return True if box a's z-axis Interval has a smaller min compared to box b's z-axis, false if equal/greater */
    static bool box_z_compare(const shared_ptr<Hittable>& a, const shared_ptr<Hittable>& b) {
        return box_compare(a, b, 2);
    }
};

#endif