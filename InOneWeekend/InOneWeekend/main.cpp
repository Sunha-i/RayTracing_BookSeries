//
//  main.cpp
//  InOneWeekend
//
//  Created by Sun on 2023/12/16.
//

#include <iostream>

int main(int argc, const char * argv[]) {
    
    // Image
    int image_width = 256;
    int image_height = 256;
    
    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    
    // 각 행은 왼쪽에서 오른쪽으로, 그 행들은 위에서 아래로 입력됨.
    // rgb 각 요소는 내부적으로 0.0~1.0 사이의 실수 값으로 나타내지만, 출력 전에 0~255 사이의 정수 값으로 변환되어야 함.
    // ➡️ Red는 fully off(black)에서 fully on(bright red)로, ⬇️ green은 black에서 bright green으로 갈테니 bottom right 코너는 노란색이 될 것!
    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto r = double(i) / (image_width - 1);
            auto g = double(j) / (image_height - 1);
            auto b = 0;
            
            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);
            
            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
    
    std::clog << "\rDone.                 \n";
    
    return 0;
}
