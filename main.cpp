/**
 * Casey Gehling
 * 
 * Defines multiple scenes to be rendered.
 * Usage: ./main <scene_number> > <output_file.ppm>
 */

#include "constants.h"
#include "camera.h"
#include "material.h"
#include "sphere.h"
#include "hittable_list.h"
#include "bvh.h"
#include <iostream>
#include "quad.h"
#include "constant_medium.h"
#include "tri.h"


void moon_scene() {
    auto moon_texture = make_shared<image_texture>("textures/moon_texture.jpeg");
    auto moon_surface = make_shared<lambertian>(moon_texture);
    auto moon = make_shared<sphere>(point3(0,0,0), 2, moon_surface);

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background = color(0, 0, 0);

    cam.vfov     = 20;
    cam.lookfrom = point3(0,0,12);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(hittable_list(moon));
}

void perlin_scene() {
    hittable_list world;

    auto pertext = make_shared<noise_texture>(4);
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<lambertian>(pertext)));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background = color(0.7, 0.5, 1.00);

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}


void quads_scene() {
    hittable_list world;

    // Materials
    auto left_red     = make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto back_green   = make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue   = make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal   = make_shared<lambertian>(color(0.2, 0.8, 0.8));

    // Quads
    world.add(make_shared<quad>(point3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), left_red));
    world.add(make_shared<quad>(point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.add(make_shared<quad>(point3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(make_shared<quad>(point3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), lower_teal));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background = color(0.7, 0.5, 1.00);

    cam.vfov     = 80;
    cam.lookfrom = point3(0,0,9);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void light_scene() {
    hittable_list world;

    auto pertext = make_shared<noise_texture>(4);
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<lambertian>(pertext)));

    auto difflight = make_shared<diffuse_light>(color(4,4,4));
    world.add(make_shared<quad>(point3(3,1,-2), vec3(2,0,0), vec3(0,2,0), difflight));
    world.add(make_shared<sphere>(point3(0,7,0), 2, difflight));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background        = color(0,0,0);

    cam.vfov     = 20;
    cam.lookfrom = point3(26,3,6);
    cam.lookat   = point3(0,2,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void cornell_smoke_scene() {
    hittable_list world;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));

    world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(make_shared<quad>(point3(113,554,127), vec3(330,0,0), vec3(0,0,305), light));
    world.add(make_shared<quad>(point3(0,555,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    shared_ptr<hittable> box1 = box(point3(0,0,0), point3(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));

    shared_ptr<hittable> box2 = box(point3(0,0,0), point3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));

    world.add(make_shared<constant_medium>(box1, 0.01, color(0,0,0)));
    world.add(make_shared<constant_medium>(box2, 0.01, color(1,1,1)));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 200;
    cam.max_depth         = 50;
    cam.background        = color(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void diamond_block_scene() {
    hittable_list world;

    // Floor
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9)))));

    // Diamond block
    auto diamond_block_texture = make_shared<image_texture>("textures/diamond.jpg");
    shared_ptr<hittable> diamond_block = box(point3(0,0,0), point3(2,2,2), make_shared<lambertian>(diamond_block_texture));
    world.add(diamond_block);

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 400;
    cam.max_depth         = 50;
    cam.background        = color(0.70, 0.80, 1.00);

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void tri_test_scene() {
    hittable_list world;


    // Mats
    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto diamond_block_texture = make_shared<image_texture>("textures/diamond.jpg");

    // Solid color triangle
    world.add(make_shared<tri>(point3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), red));

    // Textured triangle
    world.add(make_shared<tri>(point3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), make_shared<lambertian>(diamond_block_texture)));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background = color(0.7, 0.5, 1.00);

    cam.vfov     = 80;
    cam.lookfrom = point3(0,0,9);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void obj_test_scene() {
    hittable_list world;

    // Floor
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9)))));

    // Mats
    auto red = make_shared<metal>(color(.65, .05, .05), 0.5);

    // Solid color triangle
    world.add(mesh("models/sword.obj", red));

    // skybox textures
    auto left = make_shared<image_texture>("skybox/left.jpg");
    auto right = make_shared<image_texture>("skybox/right.jpg");
    auto top = make_shared<image_texture>("skybox/top.jpg");
    auto bottom = make_shared<image_texture>("skybox/bottom.jpg");
    auto front = make_shared<image_texture>("skybox/front.jpg");
    auto back = make_shared<image_texture>("skybox/back.jpg");

    world.add(cube_map(left,right,front,back,top,bottom,100));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background = color(0.7, 0.5, 1.00);

    cam.vfov     = 80;
    cam.lookfrom = point3(0,5,10);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 3;

    cam.render(world);
}

void skybox_test_scene() {
    hittable_list world;

    // skybox textures
    auto left = make_shared<image_texture>("skybox/left.jpg");
    auto right = make_shared<image_texture>("skybox/right.jpg");
    auto top = make_shared<image_texture>("skybox/top.jpg");
    auto bottom = make_shared<image_texture>("skybox/bottom.jpg");
    auto front = make_shared<image_texture>("skybox/front.jpg");
    auto back = make_shared<image_texture>("skybox/back.jpg");

    world.add(cube_map(left,right,front,back,top,bottom,100));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 500;
    cam.background = color(0.7, 0.5, 1.00);

    cam.lookfrom = point3(0, 11, 10);  // Position inside the cube
    cam.vfov = 90;             
    cam.lookat   = point3(200,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void ray_intersection_scene() {
    hittable_list world;
    auto red = make_shared<lambertian>(color(.65, .05, .05));

    world.add(make_shared<sphere>(point3(-2,0, 0), 3, make_shared<lambertian>(color(0.5,0.5,0.5))));
    world.add(make_shared<tri>(point3(5,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), red));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 500;
    cam.background = color(0.7, 0.5, 1.00);

    cam.lookfrom = point3(0, 11, 10);  // Position inside the cube
    cam.vfov = 90;             
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);

}

void volume_scene() {
    hittable_list world;

    auto sphere_ob = make_shared<sphere>(point3(0,3, 0), 3, make_shared<lambertian>(color(0,0,0)));
    world.add(make_shared<constant_medium>(sphere_ob, 0.5, color(0,0,0)));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9)))));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 200;
    cam.max_depth         = 500;
    cam.background = color(1, 1, 1.00);

    cam.lookfrom = point3(0, 8, 6);  // Position inside the cube
    cam.vfov = 90;             
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void motion_blur_scene() {
    hittable_list world;

    auto sphere_ob = make_shared<sphere>(point3(0,3, 0), point3(0,0,0), 3, make_shared<lambertian>(color(0,0,0)));
    world.add(sphere_ob);

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 200;
    cam.max_depth         = 500;
    cam.background = color(1, 1, 1.00);

    cam.lookfrom = point3(0, 8, 6);  // Position inside the cube
    cam.vfov = 90;             
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void perlin_ball_scene() {
    hittable_list world;

    auto tex1 = make_shared<noise_texture>(0);
    auto tex2 = make_shared<noise_texture>(4);
    world.add(make_shared<sphere>(point3(-3.5,3, 0), 3, make_shared<lambertian>(tex1)));
    world.add(make_shared<sphere>(point3(3.5,3, 0), 3, make_shared<lambertian>(tex2)));

    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9)))));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 200;
    cam.max_depth         = 500;
    cam.background = color(1, 1, 1.00);

    cam.lookfrom = point3(0, 9, 7);  // Position inside the cube
    cam.vfov = 90;             
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void materials_scene() {
    hittable_list world;
    // auto sphere_ob = make_shared<sphere>(point3(0,3, 0), 3, make_shared<perlin>());
    world.add(make_shared<sphere>(point3(-3,3, 0), 1, make_shared<lambertian>(color(0.5,1,0.5))));
    world.add(make_shared<sphere>(point3(0,3, 0), 1, make_shared<metal>(color(1,0.5,0.5), 0.5)));
    world.add(make_shared<sphere>(point3(3,3, 0), 1, make_shared<dielectric>(0.5)));
    world.add(make_shared<sphere>(point3(-3,6, -1), 0.5, make_shared<diffuse_light>(color(7,7,7))));
    world.add(make_shared<sphere>(point3(0,6, -1), 0.5, make_shared<diffuse_light>(color(7,7,7))));
    world.add(make_shared<sphere>(point3(3,6, -1), 0.5, make_shared<diffuse_light>(color(7,7,7))));


    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9)))));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 200;
    cam.max_depth         = 500;
    cam.background = color(0.5, 0.5, 0.5);

    cam.lookfrom = point3(0, 3, -5);  // Position inside the cube
    cam.vfov = 90;             
    cam.lookat   = point3(0,3,10);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

int main(int argc, const char * argv[]) {
    if (argc != 2) {
        printf("Usage: ./main <scene_number> > <output_file.ppm>");
        return -1;
    }
    int scene = atoi(argv[1]);

    switch(scene) {
        case 1: moon_scene(); break;
        case 2: perlin_scene(); break;
        case 3: quads_scene(); break;
        case 4: light_scene(); break;
        case 5: cornell_smoke_scene(); break;
        case 6: diamond_block_scene(); break;
        case 7: tri_test_scene(); break;
        case 8: obj_test_scene(); break;
        case 9: skybox_test_scene(); break;
        case 10: ray_intersection_scene(); break;
        case 11: volume_scene(); break;
        case 12: motion_blur_scene(); break;
        case 13: perlin_ball_scene(); break;
        case 14: materials_scene(); break;
    }
}