/**
 * Casey Gehling
 * 
 * Defines raytracer camera functionality. Models behavior over pinhole camera, includes
 * configurable options for camera position, viewport, sampling rate, and focus angle.
 *  
 */
#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

#include <thread>
#include <vector>

class camera {

    public:
        // Image config
        double aspect_ratio = 1.0;
        int image_width = 100; // image output width
        int samples_per_pixel = 10; // sampling rate
        int max_depth = 10; // max recursion depth, i..e max number of ray "bounces" into scene
        color background; // scene background color

        double vfov = 90; // vertical fov
        point3 lookfrom = point3(0,0,0); // camera position in scene
        point3 lookat = point3(0,0,-1); // camera point direction
        vec3 vup = vec3(0,1,0); // camera up vector

        double defocus_angle = 0;
        double focus_dist = 10;

        void render(const hittable& world) {
            initialize();

            int lines_left = image_height;

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            std::vector<color> framebuffer(image_width * image_height);

            // Divide rendering into threads
            const int num_threads = std::thread::hardware_concurrency();
            std::vector<std::thread> threads;

            // Per thread, render equally divided chunk of lines. 
            auto render_chunk = [&](int start, int end) {
                for (int j = start; j < end; j++) {
                    // Render debug output
                    std::clog << "\nScanlines remaining: " << lines_left << ' ' << std::flush;
                    for (int i = 0; i < image_width; i++) {
                        color pixel_color(0, 0, 0);
                        for (int sample = 0; sample < samples_per_pixel; sample++) {
                            ray r = get_ray(i, j);
                            pixel_color += ray_color(r, max_depth, world);
                        }
                        framebuffer[j * image_width + i] = pixel_samples_scale * pixel_color;
                    }
                    --lines_left;
                }
            };

            // Launch threads
            int chunk_size = image_height / num_threads;
            for (int t = 0; t < num_threads; t++) {
                int start = t * chunk_size;
                int end = (t == num_threads - 1) ? image_height : start + chunk_size;
                threads.emplace_back(render_chunk, start, end);
            }

            // Join threads
            for (auto& thread : threads) {
                thread.join();
            }

            // Write framebuffer to stdout
            for (int j = 0; j < image_height; j++) {
                for (int i = 0; i < image_width; i++) {
                    write_color(std::cout, framebuffer[j * image_width + i]);
                }
            }
        }

    private:
        int image_height;
        double pixel_samples_scale;
        point3 center;
        point3 pixel00_loc;
        vec3 pixel_delta_u;
        vec3 pixel_delta_v;
        vec3 u,v,w;
        vec3 defocus_disk_u;
        vec3 defocus_disk_v;

        void initialize() {
            // Calculate image height
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            pixel_samples_scale = 1.0 / samples_per_pixel;

            center = lookfrom;
            
            // Viewport (Camera) config
            auto theta = degrees_to_radians(vfov);
            auto h = std::tan(theta/2);
            auto viewport_height = 2 * h * focus_dist;
            auto viewport_width = viewport_height * (double(image_width)/image_height);

            // Calculate u,v,w unit basis vectors for camera coordinate frame
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup,w));
            v = cross(w,u);

            // Calculate the vectors across the horizontal and down the vertical viewport edges
            vec3 viewport_u = viewport_width * u;
            vec3 viewport_v = viewport_height * -v;

            // Pixel deltas
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // Calulate location of upper left pixel
            auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

            auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
            defocus_disk_u = u * defocus_radius;
            defocus_disk_v = v * defocus_radius;
        }

        ray get_ray(int i, int j) const {
            auto offset = sample_square();
            auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);
            auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
            auto ray_direction = pixel_sample - ray_origin;
            auto ray_time = random_double();

            return ray(ray_origin, ray_direction, ray_time);
        }

        vec3 sample_square() const {
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        point3 defocus_disk_sample() const {
            // Returns a random point in the camera defocus disk.
            auto p = random_in_unit_disk();
            return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
        }

        color ray_color(const ray& r, int depth, const hittable& world) const {
            if (depth <= 0) {
                return color(0,0,0);
            }

            hit_record rec;

            // if ray hits nothing, return background color.
            if (!world.hit(r, interval(0.001, infinity), rec)) {
                return background;
            }

            // otherwise, calculate color and draw.
            ray scattered;
            color attenuation;
            color emission_color = rec.mat->emitted(rec.u, rec.v, rec.p);

            if (!rec.mat->scatter(r, rec, attenuation, scattered)) {
                return emission_color;
            }

            color scatter_color = attenuation * ray_color(scattered, depth - 1, world);

            return emission_color + scatter_color;
        }

};

#endif