//
//  camera.h
//  InOneWeekend
//
//  Created by Sun on 2023/12/18.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

#include "color.h"
#include "hittable.h"

#include <iostream>

class camera {
public:
    double aspect_ratio = 1.0;  // Ratio of image width over height
    int    image_width  = 100;  // Rendered image width in pixel count
    
    void render(const hittable& world) {
        initialize();
        
        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
        
        // 각 행은 왼쪽에서 오른쪽으로, 그 행들은 위에서 아래로 입력됨.
        // 여기서는 P(0,0)을 기준으로 하여 각 pixel들의 center를 구하고, camera_center를 이용해 eye->pixel로의 ray를 정의.
        for (int j = 0; j < image_height; ++j) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
                auto ray_direction = pixel_center - center;
                ray r(center, ray_direction);

                color pixel_color = ray_color(r, world);
                write_color(std::cout, pixel_color);
            }
        }
        
        std::clog << "\rDone.                 \n";
    }
    
private:
    int    image_height;   // Rendered image height
    point3 center;         // Camera center
    point3 pixel00_loc;    // Location of pixel 0,0
    vec3   pixel_delta_u;  // Offset to pixel to the right
    vec3   pixel_delta_v;  // Offset to pixel below
    
    void initialize() {
        // Calculate the image height, and ensure that it's at least 1.
        // 만약 픽셀들의 수직 간격과 수평 간격이 같다면 그걸 둘러싼 뷰포트는 여기서의 rendered image와 동일한 aspect ratio를 가질 것.
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;
        
        center = point3(0, 0, 0);
        
        // Determine viewport dimensions.; scene ray를 통과시키기 위한 가상 뷰포트
        // camera_center에서 viewport_center로의 벡터는 뷰포트에 orthogonal하고, 그 거리(focal_length)는 one unit으로 설정.
        // 여기서 aspect_ratio를 이용해 viewport_width를 구하지 않는 이유는 실제 이미지 ratio랑 다를 수 있기 때문.
        // image_height 계산에서 가까운 정수로 내림되거나 (< 1) 조건문에 걸린다면 ideal ratio랑 다를 것.
        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);
        
        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        // image coord 기준에서 top-left인 첫번째 픽셀로부터 bottom-right로 내려가면서 마지막 픽셀까지 가려면,
        // image coord의 Y축이 반전되어 있어야 함!! (설정한 right-handed 좌표계 기준)
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);
        
        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        // delta vector는 pixel_center 사이의 간격을 나타내기에 아래와 같이 구함.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;
        
        // Calculate the location of the upper left pixel.
        // viewport_upper_left는 뷰포트의 top-left 지점을 나타내며, pixel00_loc는 pixel (0,0)의 위치를 나타냄.
        // 픽셀의 위치는 사각형인 픽셀의 중심으로 나타내기에 두 delta vector에 0.5를 곱한 값으로 계산됨.
        auto viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    color ray_color(const ray& r, const hittable& world) const {
        // linearly blend white and blue depending on the height of the y coordinate.
        // to implement a simple gradient, use lerp.
        // blendedValue = (1-a) * startValue + a * endValue
        
        // 아직 빛과 같은 요소들이 없기 때문에 color map을 이용해서 normal(N)을 시각화할 것.
        // N을 unit length vector로 생각하면, 각 component는 -1과 1사이. -> 0~1의 간격으로 mapping해야 (r,g,b)로 나타낼 수 있음.
        // 이를 위해 +1만큼의 offset에 범위의 간격인 2를 1로 줄이는 0.5 * color(N.x()+1, N.y()+1, N.z()+1)을 구현한 것.
        
        hit_record rec;
        
        if (world.hit(r, interval(0, infinity), rec)) {
            return 0.5 * (rec.normal + color(1,1,1));
        }
        
        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0-a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }
};

#endif /* CAMERA_H */
