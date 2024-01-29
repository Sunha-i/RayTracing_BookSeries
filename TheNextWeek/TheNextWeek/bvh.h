//
//  bvh.h
//  TheNextWeek
//
//  Created by Sun on 2024/01/20.
//

#ifndef BVH_H
#define BVH_H

#include "rtweekend.h"

#include "hittable.h"
#include "hittable_list.h"

#include <algorithm>

class bvh_node : public hittable {
public:
    bvh_node(const hittable_list& list) : bvh_node(list.objects, 0, list.objects.size()) {}
    bvh_node(const std::vector<shared_ptr<hittable>>& src_objects, size_t start, size_t end) {
        auto objects = src_objects; // Create a modifiable array of the source scene objects
        
        // First, randomly choose an axis.
        int axis = random_int(0,2);
        auto comparator = (axis == 0) ? box_x_compare
                        : (axis == 1) ? box_y_compare
                                      : box_z_compare;
        
        size_t object_span = end - start;
        
        // Then, sort the primitives.
        if (object_span == 1) {
            left = right = objects[start];
        } else if (object_span == 2) {
            if (comparator(objects[start], objects[start+1])) {
                left = objects[start];
                right = objects[start+1];
            } else {
                left = objects[start+1];
                right = objects[start];
            }
        } else {
            std::sort(objects.begin() + start, objects.begin() + end, comparator);

            auto mid = start + object_span/2;
            left = make_shared<bvh_node>(objects, start, mid);
            right = make_shared<bvh_node>(objects, mid, end);
        }
        
        // Lastly, put half in each subtree.
        bbox = aabb(left->bounding_box(), right->bounding_box());
    }
    
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        // check whether the box for the node is hit.
        // and if so, check the children and sort out any details.
        
        if (!bbox.hit(r, ray_t))
            return false;
        
        bool hit_left = left->hit(r, ray_t, rec);
        bool hit_right = right->hit(r, interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);
        
        return hit_left || hit_right;
    }
    
    aabb bounding_box() const override { return bbox; }
    
private:
    shared_ptr<hittable> left;
    shared_ptr<hittable> right;
    aabb bbox;
    
    static bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis_index)
    {
        return a->bounding_box().axis(axis_index).min < b->bounding_box().axis(axis_index).min;
    }
    
    static bool box_x_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
        return box_compare(a, b, 0);
    }
    
    static bool box_y_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
        return box_compare(a, b, 1);
    }
    
    static bool box_z_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
        return box_compare(a, b, 2);
    }
};

#endif /* BVH_H */

// Note
// BVH is really a container (just like lists of hittables), but it can respond to the query "does this ray hit you?".
// Instead of having two classes - one for the tree, and one for the nodes in the tree, take just one class and have the root just be a node we point to.
// As long as the list of objects in a bvh_node gets divided into two sub-lists, the hit function will work.
// This class chooses the middle ground for division, and at each node split the list along one axis.
