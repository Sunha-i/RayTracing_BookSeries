//
//  main.cpp
//  InOneWeekend
//
//  Created by Sun on 2023/12/16.
//

#include "color.h"
#include "vec3.h"

#include <iostream>

int main(int argc, const char * argv[]) {
    
    // Image
    int image_width = 256;
    int image_height = 256;
    
    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    
    // 각 행은 왼쪽에서 오른쪽으로, 그 행들은 위에서 아래로 입력됨.
    // 이번에도 마찬가지로 → Red는 fully off(black)에서 fully on(bright red)로, ↓ green은 black에서 bright green으로 갈테니 bottom right 코너는 노란색이 될 것!
    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto pixel_color = color(double(i) / (image_width - 1),
                                     double(j) / (image_height - 1), 0);
            write_color(std::cout, pixel_color);
        }
    }
    
    std::clog << "\rDone.                 \n";
    
    return 0;
}
