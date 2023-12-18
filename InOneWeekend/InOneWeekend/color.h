//
//  color.h
//  InOneWeekend
//
//  Created by Sun on 2023/12/17.
//

#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

using color = vec3;

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
    
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();
    
    // Divide the color by the number of samples.
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;
    
    // Write the translated [0,255] value of each color component.
    // rgb 각 요소는 내부적으로 0.0~1.0 사이의 실수 값으로 나타내지만, 출력 전에 0~255 사이의 정수 값으로 변환되어야 함.
    // ++) 이전처럼 pixel color에 255.999를 곱해 변환하는 대신 intensity의 max 범위를 0.999로 설정.
    static const interval intensity(0.000, 0.999);
    out << static_cast<int>(256 * intensity.clamp(r)) << ' '
        << static_cast<int>(256 * intensity.clamp(g)) << ' '
        << static_cast<int>(256 * intensity.clamp(b)) << '\n';
}

#endif /* COLOR_H */
