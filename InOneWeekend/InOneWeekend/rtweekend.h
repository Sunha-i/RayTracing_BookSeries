//
//  rtweekend.h
//  InOneWeekend
//
//  Created by Sun on 2023/12/18.
//

#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>

// Usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// Common Headers
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif /* RTWEEKEND_H */
