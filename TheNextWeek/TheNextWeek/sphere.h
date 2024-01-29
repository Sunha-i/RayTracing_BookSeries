//
//  sphere.h
//  TheNextWeek
//
//  Created by Sun on 2024/01/18.
//

#ifndef SPHERE_H
#define SPHERE_H

#include "rtweekend.h"

#include "hittable.h"

class sphere : public hittable {
public:
    // Stationary Sphere
    sphere(point3 _center, double _radius, shared_ptr<material> _material)
        : center1(_center), radius(_radius), mat(_material), is_moving(false)
    {
        auto rvec = vec3(radius, radius, radius);
        bbox = aabb(center1 - rvec, center1 + rvec);
    }
    // Moving Sphere
    sphere(point3 _center1, point3 _center2, double _radius, shared_ptr<material> _material)
        : center1(_center1), radius(_radius), mat(_material), is_moving(true)
    {
        // its center moves linearly from center1 at t=0 to center2 at t=1.
        // take the box of the sphere at each time, and compute the box around two boxes.
        
        auto rvec = vec3(radius, radius, radius);
        aabb box1(_center1 - rvec, _center1 + rvec);
        aabb box2(_center2 - rvec, _center2 + rvec);
        bbox = aabb(box1, box2);
        
        center_vec = _center2 - _center1;
    }
    
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        // oc는 origin to center를 의미함.
        // arbitrary point인 center C와 point P(x,y,z)에 대해, 원의 방정식을 (x-Cx)^2 + (y-Cy)^2 + (z-Cz)^2 = r^2으로 정의할 수 있고,
        // 이 방정식에서의 좌변을 C에서 P로 향하는 벡터 (P-C)의 내적으로 나타내면 (P-C)·(P-C) = r^2가 됨.
        // 우리가 알고싶은건 ray가 sphere에 부딪히는지, 그 여부이고 만약 그렇다면 광선의 위치 P(t) 또한 위 방정식을 만족할 것.
        // 따라서 ((A+tb)-C)·((A+tb)-C) = r^2를 t에 대한 내림차순으로 정리한 이차방정식의 판별식을 통해 ray와 sphere가 교차하는지 판단하는 코드.
        
        // 그런데 ↑ 이 이차방정식의 일차항 계수인 2b·(A-C)는 짝수이기 때문에 짝수 근의 공식과 판별식을 사용해 더 간단한 식으로 대체 가능함.
        // ++) 벡터의 자기 자신에 대한 내적은 벡터의 길이의 제곱이므로 vec3에서 구현한 메소드 length_squared()를 이용.
        
        point3 center = is_moving ? sphere_center(r.time()) : center1;
        vec3 oc = r.origin() - center;
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - radius * radius;
        
        // discriminant가 음수라면 no hit.
        auto discriminant = half_b*half_b - a*c;
        if (discriminant < 0)   return false;
        auto sqrtd = sqrt(discriminant);
        
        // Find the nearest root that lies in the acceptable range.
        // 방정식의 해로, 교차 지점의 `t`값인 root가 ray의 유효 범위 내에 있는지 확인하는 과정. (이때 더 가까운 해를 찾기 위해 작은 근부터 시도)
        auto root = (-half_b - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (-half_b + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }
        
        // surface normal을 얻어 셰이딩을 표현하기 위한 이 과정에서 normal은 표면에 수직인 벡터를 뜻함.
        // 즉 지구 중심에서 나에게로 향하는 벡터와 같기에 t가 root(근)일 때의 지점을 구해(at->P(t)) 구의 중심에서 교차점으로 향하는 벡터를 구해준 것.
        // normal vector가 임의의 길이를 갖도록 하는 대신, unit length로 정규화하기 위해서는 square root 연산 비용을 고려해야 함!
        // 하지만 특정 지오메트리의 특징을 이용해 제곱근 연산을 피할 수 있는데, 구의 경우 구의 반지름으로 나눠주면 됨.
        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;
        
        return true;
    }
    
    aabb bounding_box() const override { return bbox; }
    
private:
    point3 center1;
    double radius;
    shared_ptr<material> mat;
    bool is_moving;
    vec3 center_vec;
    aabb bbox;
    
    point3 sphere_center(double time) const {
        // Linearly interpolate from center1 to center2 according to time, where t=0 yields center1, and t=1 yields center2.
        return center1 + time * center_vec;
    }
};

#endif /* SPHERE_H */
