//
//  ray.h
//  TheNextWeek
//
//  Created by Sun on 2024/01/18.
//

#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
public:
    ray() {}
    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}
    
    point3 origin() const { return orig; }
    vec3 direction() const { return dir; }
    
    // same as function P(t)
    point3 at(double t) const {
        return orig + t * dir;
    }
    
private:
    point3 orig;
    vec3 dir;
};

#endif /* RAY_H */

// Note
// ray를 함수 P(t) = A + tb로 정의.
// 여기서 P는 3D space에서 선 상의 한 점, A는 ray origin, b는 ray direction이 될 것.
// ray parameter t의 값에 따라 ray 상에서 점이 이동하며, positive인 경우에 half-line 또는 ray라고 함.
