//
//  main.cpp
//  InOneWeekend
//
//  Created by Sun on 2023/12/16.
//

#include "rtweekend.h"

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"

int main(int argc, const char * argv[]) {
    
    // World ;초록색 ground는 아주 큰 sphere의 북쪽 normal에 대한 RGB color를 이용해 구현.

    hittable_list world;
    
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));
    
    camera cam;
    
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    
    cam.render(world);
    
    return 0;
}
