//
//  vec3.h
//  TheNextWeek
//
//  Created by Sun on 2024/01/18.
//

#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;
using std::fabs;

class vec3 {
public:
    double e[3];

    vec3() : e{0,0,0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(double t) {
        return *this *= 1/t;
    }

    double length() const {
        return sqrt(length_squared());
    }

    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }
    
    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        auto s = 1e-8;
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }
    
    static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }
    
    static vec3 random(double min, double max) {
        return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
    }
};

// point3 is just an alias for vec3, but useful for geometric clarity in the code.
using point3 = vec3;


// Vector Utility Functions

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

inline vec3 operator/(vec3 v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3 &u, const vec3 &v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

inline vec3 random_in_unit_disk() {
    // using the same kind of method in random_in_unit_sphere()
    // just for "two dimensions".
    while (true) {
        auto p = vec3(random_double(-1,1), random_double(-1,1), 0);
        if (p.length_squared() < 1)
            return p;
    }
}

inline vec3 random_in_unit_sphere() {
    // use a rejection method to generate the random vector inside of the unit sphere.
    // unit cube 안에서 랜덤한 지점을 pick하지만 unit sphere를 벗어난 범위의 point는 무시.
    while (true) {
        auto p = vec3::random(-1,1);
        if (p.length_squared() < 1)
            return p;
    }
}

inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

inline vec3 random_on_hemisphere(const vec3& normal) {
    // diffuse material(or matte) 표면에 부딪힌 광선은 표면에서 멀어지는 어떤 방향으로든 균일한 확률로 튕겨나가기 때문에..
    // 이 매트한 표면을 구현하는 가장 직관적인 방법은 표면에서 광선을 모든 방향으로 동일하게 무작위로 반사시키는 것.
    // 이에 따라 unit sphere 내에서의 normalized random vector를 생성하고, 해당 벡터가 surface normal쪽 반구에 들어가지 않는다면 invert하는 방식!
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0)      // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

inline vec3 reflect(const vec3& v, const vec3& n) {
    // incident ray v에 대해, reflected ray는 v+2b.
    // b의 길이는 v를 unit normal에 대해 정사영시킨 길이이고, 방향은 n과 같음.
    // 이때 ray와 normal은 반대 방향을 향할 것이므로 내적의 결과에 음수를 곱해 그 길이를 구함.
    
    return v - 2*dot(v,n)*n;
}

inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    // Snell's Law: η×sinθ = ηʹ×sinθʹ (η is refractive indices)
    // 결과적으로 구해야 하는 refracted ray Rʹ은 Rʹ⟂ + R'∥ 로 나눌 수 있고, (어떠한 증명을 통해) 각각을 얻을 수 있음.
    // Rʹ⟂ = η/ηʹ(R+cosθn), R'∥ = -√(1-❘R'⟂❘^2)n. 여기서 cosθ만 내적을 이용해 구해주면 됨!
    // R과 n을 unit vector로 제한한다면 cosθ = -R·n. η/ηʹ는 파라미터로 받는 refraction ratio.
    
    auto cos_theta = fmin(dot(-uv,n), 1.0);
    vec3 r_out_perp =  etai_over_etat * (uv + cos_theta * n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

#endif /* VEC3_H */

// Note
// 대부분의 그래픽스 프로그램은 geometric vector와 color를 저장하는 클래스를 가지고 있음.
// 보통은 {3D position (xyz) + homogeneous coordinate} 또는 {RGB + alpha transparency component}를 위한 4D이지만
// 여기서는 3차원 좌표계를 이용해서 색, 위치, 방향, 오프셋...등 모든 것을 구현할 것.
// 다만 그 의도와 사용을 분명히 하기 위해 point3와 color라는 두 개의 type alias를 사용.
