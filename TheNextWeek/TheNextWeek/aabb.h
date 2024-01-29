//
//  aabb.h
//  TheNextWeek
//
//  Created by Sun on 2024/01/20.
//

#ifndef AABB_H
#define AABB_H

#include "rtweekend.h"

class aabb {
public:
    interval x, y, z;
    
    aabb() {}   // The default AABB is empty, since intervals are empty by default.
    aabb(const interval& ix, const interval& iy, const interval& iz) : x(ix), y(iy), z(iz) {}
    
    aabb(const point3& a, const point3& b) {
        // Treate the two points a and b as extrema for the bounding box, so we don't require a particular minimum/maximum coordinate order.
        x = interval(fmin(a[0],b[0]), fmax(a[0],b[0]));
        y = interval(fmin(a[1],b[1]), fmax(a[1],b[1]));
        z = interval(fmin(a[2],b[2]), fmax(a[2],b[2]));
    }
    
    aabb(const aabb& box0, const aabb& box1) {
        // Use this to construct an axis-aligned bounding box from two input boxes.
        x = interval(box0.x, box1.x);
        y = interval(box0.y, box1.y);
        z = interval(box0.z, box1.z);
    }
    
    const interval& axis(int n) const {
        if (n == 1)    return y;
        if (n == 2)    return z;
        return x;
    }
    
    bool hit(const ray& r, interval ray_t) const {
        // Key thing is dividing objects into subsets. Not dividing the screen or the volume.
        // "whether or not ray hits bouding object"에 대한 boolean 값을 반환하는 함수!
        // if (ray hits bounding object)
        //     return whether ray hits bounded objects
        // else
        //     return false
        
        // 구체적으로 cube(3D AABB)-ray intersection은 axis x,y,z에 대한 t-interval들이 겹치는지에 대한 판단으로 이루어짐!
        // bool overlap(d, D, e, E, f, F)
        //     f = max(d, e)
        //     F = min(D, E)
        //     return (f < F)
        // +) ray가 negative x direction으로 향하는 경우를 고려해 plane-ray가 교차했을 때의 t값인 d,D에 대해 d<D를 보장해줘야 함.
        // 이를 위해 optimized aabb hit method에서는 간단하게 invD를 통해 방향을 판별하고 t0, t1을 swap해주는 방식을 사용.

        // ++) 여기서 t는 ray를 given a t returns a location P(t)인 P(t)=A+tb로 정의했을 떄의 값을 말함.
        // 즉, hit point에서의 t값((plane-A)/b)이며 각 축에 대해 t-interval을 구하면서 업데이트하는 방식!
        
        for (int a = 0; a < 3; a++) {
            auto invD = 1 / r.direction()[a];
            auto orig = r.origin()[a];

            auto t0 = (axis(a).min - orig) * invD;
            auto t1 = (axis(a).max - orig) * invD;

            if (invD < 0)
                std::swap(t0, t1);

            if (t0 > ray_t.min) ray_t.min = t0;
            if (t1 < ray_t.max) ray_t.max = t1;

            if (ray_t.max <= ray_t.min)
                return false;
        }
        return true;
    }
};

#endif /* AABB_H */

// Note
// Axis-Aligned Bounding Boxes (AABBs)는 ray를 bounding volume과 교차시키기 위한 good division..
// This is based on the observation that an n-dimensional AABB is just the intersection of n axis-aligned intervals, often called "slabs".
// slab method를 사용해 ray-slab intersection을 판단!
