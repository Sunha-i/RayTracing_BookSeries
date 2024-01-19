//
//  color.h
//  TheNextWeek
//
//  Created by Sun on 2024/01/18.
//

#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

using color = vec3;

inline double linear_to_gamma(double linear_component)
{
    // Gamma Correction:
    // 실제로 70% 정도의 reflectance 렌더 결과가 50% 보다 middle-gray에 가까운 현상이 있음. 이렇게 이미지가 어둡게 나타나는 이유는...
    // 거의 모든 컴퓨터 프로그램이 이미지가 파일로 기록되기 전에 "gamma corrected" 될 것을 가정해서 감마 보정을 수행했기 때문.
    
    // 사람의 눈은 어두움에 더 민감함! 그래서 어두운 영역을 더 넓게 표현해야만 사람 눈에 더 자연스러운 색으로 보일 수 있음.
    // 그래서 디지털 이미지들은 Gamma Encoding 되어 밝게 저장되고, 이를 모니터에서 보여줄 때는 Decoding을 통해 최종 보정된 이미지를 보여주는 것.
    // 이러한 Gamma Correction이 결과적으로 다시 linear한 그래프를 만들어내는 것처럼 보이지만, 그렇게 저장함으로써 어두운 컬러에 더 많은 비트 영역이 할당되고
    // 실제 데이터가 어두운 쪽에 많이 분포해 우리 눈에 보이는 것과 비슷한 이미지로 보여줄 수 있게 됨. (gamma space에 저장해야 하는 이유)

    // Gamma Encoding 보정을 가하기 전의 이미지는 linear color space에 있음. (수학적으로 정확한 포맷, shader나 색 연산시 정확)
    // sRGB는 업계에서 많이 쓰여지는 gamma color space. ("gamma 2.2"를 적용한 색 공간)
    // 여기서는 간단한 근사치로 "gamma 2"를 사용할 것. 현재 linear to gamma space로의 변환이 필요하기 때문에 gamma의 역수로 제곱근 사용.
    
    return sqrt(linear_component);
}

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel, uint8_t* pixels, int& idx) {
    
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();
    
    // Divide the color by the number of samples.
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;
    
    // Apply the linear to gamma transform.
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);
    
    // Write the translated [0,255] value of each color component.
    // rgb 각 요소는 내부적으로 0.0~1.0 사이의 실수 값으로 나타내지만, 출력 전에 0~255 사이의 정수 값으로 변환되어야 함.
    // ++) 이전처럼 pixel color에 255.999를 곱해 변환하는 대신 intensity의 max 범위를 0.999로 설정.
    static const interval intensity(0.000, 0.999);
    pixels[idx++] = static_cast<int>(256 * intensity.clamp(r));
    pixels[idx++] = static_cast<int>(256 * intensity.clamp(g));
    pixels[idx++] = static_cast<int>(256 * intensity.clamp(b));
}

#endif /* COLOR_H */
