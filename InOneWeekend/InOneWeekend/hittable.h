//
//  hittable.h
//  InOneWeekend
//
//  Created by Sun on 2023/12/18.
//

#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"

// To resolve the circular reference issue.
class material;

struct hit_record {
    point3 p;
    vec3 normal;
    shared_ptr<material> mat;   // when hit surface, point material pointer.
    double t;
    bool front_face;
    
    void set_face_normal(const ray& r, const vec3& outward_normal) {
        // Sets the hit record normal vector.
        // ** the parameter `outward_normal` is assumed to have unit length.
        
        // normal에 대한 design decision으로, normal은 항상 ray에 대해 반대로 향하도록 함. (ray inside the sphere의 경우 만족하지 않음)
        // 이를 위해 먼저 normal과 ray가 교차한 surface를 기준으로 어느 쪽에 있는지 판단해야 함. -> dot product 사용!!
        // ray 방향 벡터와 교차점에 대한 outward normal 벡터의 내적이 0보다 작다는 건 두 벡터가 이루는 각이 90~180도 사이로, 서로 반대 방향을 나타내고 있다는 것. 즉 구 외부에서 온 ray에 의해 생성된 교차점에 대한 노말임을 알 수 있음. 내부에서 왔다면 (normal을 구하는 방식상) outward normal과 같은 방향(0~90도)일 것이고, 여기서는 내부에서 온 레이에 대해서만 normal을 outside가 아닌 local(inward)로 바꿔주기 위해 minus를 붙여줌.
        
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual ~hittable() = default;
    
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif /* HITTABLE_H */

// Note
// hittable은 ray가 hit할 수 있는 모든 것에 대한 abstract class.
// hit에 대해 tmin에서 tmax까지의 유효 범위를 설정해 이 범위 내에서만 교차를 계산하는 게 편리함.
// 추가로 광선이 어떤 것과 교차할 때 normal을 계산할지 정해야 하는데, 더 가까운 점에서 교차할 것이기에 가장 가까운 것의 normal만 필요할 것.
// 여기서는 ray가 hit한 그 지점에 대한 잡다한 데이터들을 번들로 묶어 저장하는 구조체(hit_record)를 사용해 계산!
