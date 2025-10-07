#ifndef AABB_TREE_NODE_H
#define AABB_TREE_NODE_H

#include <algorithm>
#include <memory>
#include "rt/geom/hittable.hpp"
#include "rt/geom/hittable_list.hpp"
#include "rt/geom/aabb.hpp"

using std::shared_ptr;
using std::fabs;

// Bin for SAH, stores count_ primitives and their encompassing AABB
struct Bin {
    int count_ = 0;
    Aabb bbox_ = Aabb{};
};

/**
 * @class AabbTreeNode
 * @brief Implementation of a BVH consisting of a binary tree of Aabb nodes, where each node contains one Aabb, and leaves contain the primitives.
 *
 * Each object can be treated as the root of its own tree.
 */
class AabbTreeNode final : public Hittable {
    static constexpr int MAX_LEAF{2};           // Max number of primitives per binary set of children (keep as 2 until robust Leaf structure is made)
    static constexpr int NUM_BINS{16};          // Number of bins/buckets to divide axis into
    static constexpr int INTERSECT_COST{1};     // Cost of ray-primitive intersection
    static constexpr int TRAVERSAL_COST{1};     // Cost of "traversal" from node to children
public:
    explicit AabbTreeNode(HittableList list) : AabbTreeNode(list.objects(), 0, list.size()) {}

    /**
     * @brief Construct a new AabbTreeNode.
     *
     * AabbTreeNode works as a one-class binary tree built with a binned SAH object-splitting heuristics algorithm.
     * @param primitives Objects to be stored in the tree leaves.
     * @param start Start index of objects.
     * @param end End index of objects.
     */
    AabbTreeNode(std::vector<shared_ptr<Hittable>>& primitives, size_t start, size_t end) {
        Aabb node_bounds{};             // Bounds over objects's bounding boxes
        Aabb centroid_bounds{};         // Bounds over objects's bounding boxes's centroids

        // For the completely-encompassing bounding box
        for (size_t i = start; i < end; i++) {
            Aabb object_bbox{primitives[i]->bounding_box()};
            node_bounds = Aabb{node_bounds, object_bbox};
            centroid_bounds = Aabb{centroid_bounds, Aabb{object_bbox.centroid(), object_bbox.centroid()}};
        }
        bbox_ = node_bounds;
        size_t count{end - start};

        if (count <= MAX_LEAF || node_bounds.degenerate()) {
            if (count == 1) {
                left_ = primitives[start];
                right_ = nullptr;
            } else if (count == 2) {
                left_ = primitives[start];
                right_ = primitives[start + 1];
            } else {
                size_t mid = start + count / 2;
                left_ = make_shared<AabbTreeNode>(primitives, start, mid);
                right_ = make_shared<AabbTreeNode>(primitives, mid, end);
            }
            return;
        }

        // Choosing a split axis based on which axis interval is widest, then sorting primitives along the axis
        int axis{};
        if (fabs(centroid_bounds.x().range()) >= fabs(centroid_bounds.y().range()) && fabs(centroid_bounds.x().range()) >= fabs(centroid_bounds.z().range())) {
            axis = 0;
        } else if (fabs(centroid_bounds.y().range()) >= fabs(centroid_bounds.z().range())) {
            axis = 1;
        } else {
            axis = 2;
        }
        const float centroid_min{centroid_bounds[axis].min()};
        const float centroid_max{centroid_bounds[axis].max()};
        const float centroid_range{centroid_max - centroid_min};

        if (centroid_range <= 1e-6f) {
            size_t mid = start + count / 2;
            left_ = make_shared<AabbTreeNode>(primitives, start, mid);
            right_ = make_shared<AabbTreeNode>(primitives, mid, end);
            return;
        }

        // Fill NUM_BINS bins with objects by centroid
        std::array<Bin, NUM_BINS> bins;
        for (size_t i = start; i < end; i++) {
            const Aabb b = primitives[i]->bounding_box();
            const float c = b.centroid()[axis];
            int bin_index = static_cast<int>(NUM_BINS * (c - centroid_min) / centroid_range);
            bin_index = std::ranges::clamp(bin_index, 0, NUM_BINS - 1);
            bins[bin_index].count_++;
            bins[bin_index].bbox_ = Aabb{bins[bin_index].bbox_, b};
        }

        // Compute where to split between the bins with leftward and rightward obj counts
        Aabb left_bounds[NUM_BINS];
        int left_count[NUM_BINS];
        Aabb right_bounds[NUM_BINS];
        int right_count[NUM_BINS];

        auto acc_box = Aabb{};
        size_t acc_count = 0;
        for (size_t i = 0; i < NUM_BINS; i++) {
            acc_box = Aabb{acc_box, bins[i].bbox_};
            acc_count += bins[i].count_;
            left_bounds[i] = acc_box;
            left_count[i] = acc_count;
        }

        acc_box = Aabb{};
        acc_count = 0;
        for (int i = NUM_BINS - 1; i > 0; i--) {
            acc_box = Aabb(acc_box, bins[i].bbox_);
            acc_count += bins[i].count_;
            right_bounds[i] = acc_box;
            right_count[i] = acc_count;
        }

        // Evaluate SAH at splits w/ cost function
        float inv_surface_area = 1.f / node_bounds.surface_area();
        float best_cost = INTERSECT_COST * count;
        int best_split_bin = -1;

        for (int i = 0; i < NUM_BINS - 1; i++) {
            if (left_count[i] == 0 || right_count[i + 1] == 0) {
                continue;
            }
            float left_cost = left_bounds[i].surface_area() * left_count[i] * inv_surface_area * INTERSECT_COST;
            float right_cost = right_bounds[i + 1].surface_area() * right_count[i + 1] * inv_surface_area * INTERSECT_COST;

            if (float cost = TRAVERSAL_COST + left_cost + right_cost; cost < best_cost) {
                best_cost = cost;
                best_split_bin = i;
            }
        }

        // Split or make leaf
        if (best_split_bin == -1 || best_cost >= INTERSECT_COST * count) {
            int mid = start + count / 2;
            left_ = make_shared<AabbTreeNode>(primitives, start, mid);
            right_ = make_shared<AabbTreeNode>(primitives, mid, end);
            return;
        }

        // Partition objects by the chosen split bin (in-place) (bin threshold is best_split_bin vs best_split_bin + 1)
        auto bin_of = [&](const size_t i) -> int {
            const Aabb b = primitives[i]->bounding_box();
            const float c = b.centroid()[axis];
            const int bin_index = static_cast<int>(NUM_BINS * (c - centroid_min) / centroid_range);
            return std::ranges::clamp(bin_index, 0, NUM_BINS - 1);
        };

        // Elements with bin <= best_split_bin go left
        size_t mid = start;
        for (size_t i = start; i < end; i++) {
            if (bin_of(i) <= best_split_bin) {
                std::swap(primitives[i], primitives[mid++]);
            }
        }

        // Handle degenerate partition (all on one side)
        if (mid == start || mid == end) {
            mid = start + count / 2;
        }

        // Recursive calls
        left_ = make_shared<AabbTreeNode>(primitives, start, mid);
        right_ = make_shared<AabbTreeNode>(primitives, mid, end);
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
    Aabb bounding_box() const override { return bbox_; }

private:
    // Children can be another AabbTreeNode or a leaf (renderable objects)
    shared_ptr<Hittable> left_;
    shared_ptr<Hittable> right_;

    Aabb bbox_;

    // Comparison functions
    /** @return True if the specified axis Interval of bounding box a has a smaller minimum, false if equal or larger. */
    static bool box_compare(const shared_ptr<Hittable>& a, const shared_ptr<Hittable>& b, const int axis_index) {
        const Interval a_axis_interval{a->bounding_box()[axis_index]};
        const Interval b_axis_interval{b->bounding_box()[axis_index]};
        return a_axis_interval.min() < b_axis_interval.min();
    }
    /** @return True if the x-axis Interval of bounding box a has a smaller minimum, false if equal or larger. */
    static bool box_x_compare(const shared_ptr<Hittable>& a, const shared_ptr<Hittable>& b) {
        return box_compare(a, b, 0);
    }
    /** @return True if the y-axis Interval of bounding box a has a smaller minimum, false if equal or larger. */
    static bool box_y_compare(const shared_ptr<Hittable>& a, const shared_ptr<Hittable>& b) {
        return box_compare(a, b, 1);
    }
    /** @return True if the z-axis Interval of bounding box a has a smaller minimum, false if equal or larger. */
    static bool box_z_compare(const shared_ptr<Hittable>& a, const shared_ptr<Hittable>& b) {
        return box_compare(a, b, 2);
    }
};

#endif