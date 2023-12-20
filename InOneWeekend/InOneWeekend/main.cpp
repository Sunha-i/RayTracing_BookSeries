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
    
    // World ;hittable object로는 작은 sphere와 ground로 사용할 아주 큰 sphere 두 개가 있음.

    hittable_list world;
    
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));
    
    camera cam;
    
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    
    cam.render(world);
    
    return 0;
}
