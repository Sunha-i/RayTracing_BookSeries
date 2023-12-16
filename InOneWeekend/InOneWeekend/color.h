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

void write_color(std::ostream &out, color pixel_color) {
    
    // rgb 각 요소는 내부적으로 0.0~1.0 사이의 실수 값으로 나타내지만, 출력 전에 0~255 사이의 정수 값으로 변환되어야 함.
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

#endif /* COLOR_H */
