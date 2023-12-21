//
//  material.h
//  InOneWeekend
//
//  Created by Sun on 2023/12/21.
//

#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"

struct hit_record;

class material {
public:
    virtual ~material() = default;
    
    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
public:
    lambertian(const color& a) : albedo(a) {}
    
    // True Lambertian Reflection:
    // Lambert's Cosine Law는 이상적인 난반사 표면(lambertian surface)에서 관찰되는 빛의 강도가 surface normal과
    // view vector 사이의 각, Φ의 cos에 비례한다는 것을 말함. 이 표면은 lambertian reflectance를 가지는데, 이는 관찰자가 바라보는 각도와 관계없이
    // 같은 겉보기 밝기를 가진다는 것을 의미함. 위 코사인 법칙에 따른다면 표면과 관찰 시점의 각도에 따라서 복사 강도[radiant intensity]*가 달라질 텐데,
    // 왜 이 법칙을 만족하는 lambertian 표면은 각도와 관계없이 같은 radiance를 가지는지**에 대해...다음의 의존성으로 설명이 가능할 것.
    // view vector와 normal vector 사이의 각도가 커지면 view로 향하는 radiance는 작아짐. 그런데 표면으로부터 관찰자의 눈으로 향하는
    // [투영면적 or 입체각] 또한 같은 의존성(cosΦ)으로 감소하고, 그렇게 작아짐에 따라 photon은 관찰자에게 더욱 중첩되어 도달하기 때문에 radiance가 커짐.
    // -> 상반된 [두 관계]가 같은 의존성을 가지는 것! -> 사실상 **를 구현하기 위해 필요한 *를 유도한 느낌의 분포.
    
    // ++) 이에 따라 random_on_hemisphere(rec.normal)을 이용해 diffuse model을 만드는 대신..
    //     Lambertian distribution을 이용해 난반사(diffuse)를 나타내는 Lambertian reflectance을 구현. (더 정확한 표현)
    //     그 결과 법선 방향으로의 더 많은 ray 때문에(..) 이전 방법보다 그림자가 더 돋보이고, 하늘의 색에 의해 구체가 푸른 색조를 띰.
    
    // +) 여기서는 항상 산란하고 reflectance R에 의해 감쇄될 수 있는 Lambertian (diffuse) case로 구현했지만
    //    attenuation 없이, 1-R의 확률로 가끔 산란되는 것으로도 구현할 수 있음. (산란되지 않는 레이는 흡수된다고 생각)
    //    아니면 일정한 확률 p로 산란하고 감쇄될 확률은 albedo/p로 설정하는 방법도 있을 것.
    
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        auto scatter_direction = rec.normal + random_unit_vector();
        
        // Catch degenerate scatter direction
        // random unit vector + normal vector = zero (means exactly opposite) -> leads to (infinities and NaNs).
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;
        
        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }
    
private:
    color albedo;
};

class metal : public material {
public:
    metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}
    
    // Fuzzy Reflection:
    // original endpoint를 중심으로 하는 작은 구에서 ray의 새로운 endpoint를 고름으로써 reflected direction을 randomize 할 수 있음.
    // fuzz factor로 scaling을 해서 새로운 끝점으로 가는 레이를 생성하기 때문에 구의 반지름이 fuzziness를 나타내는 파라미터가 될 수 있는 것.
    // (구가 클수록 반사는 더 흐릿해짐) 이때 큰 구나 grazing ray의 경우 표면 아래로 scatter 될 수 있는데, 해당 레이를 흡수하도록 구현할 수 있음.
    // ++) 여기서는 random_unit_vector() 대신 random_in_unit_sphere()을 사용해야 구 안에서 더 무작위한 벡터을 생성하기 때문에,
    //     구 표면에 대해 균일한 분포를 갖는 점들보다 더 자연스러운 fuzziness를 구현할 수 있음.
    
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
    
private:
    color albedo;
    double fuzz;
};

#endif /* MATERIAL_H */

// Note
// material에 대한 unique behavior를 캡슐화하는 abstract class.
// - scattered ray를 생성하거나 incident ray를 흡수하는 역할.
// - 만약 산란된다면 레이가 얼마나 attenuated 되어야 하는지 나타내는 역할.
