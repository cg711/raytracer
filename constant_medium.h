/**
 * Casey Gehling
 * 
 * Defines intersection logic for a constant medium. Used in tandum with isotropic materials
 * to provide volumetric output.
 */
#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include "hittable.h"
#include "material.h"
#include "texture.h"
#include "constants.h"

class constant_medium : public hittable {
    public:

        // Define medium boundary, density, and texture.
        constant_medium(shared_ptr<hittable> boundary, double density, shared_ptr<texture> texture) : boundary(boundary), neg_inverse_density(-1 / density), phase_function(make_shared<isotropic>(texture)) {}

        // Define medium boundary, density, and constant color.
        constant_medium(shared_ptr<hittable> boundary, double density, const color& albedo) : boundary(boundary), neg_inverse_density(-1 / density), phase_function(make_shared<isotropic>(albedo)) {}

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            hit_record rec1, rec2;

            if(!boundary->hit(r, interval::universe, rec1)) {
                return false;
            }

            if(!boundary->hit(r, interval(rec1.t + 0.0001, infinity), rec2)) {
                return false;
            }

            if(rec1.t < ray_t.min) rec1.t = ray_t.min;
            if(rec2.t > ray_t.max) rec2.t = ray_t.max;

            if (rec1.t >= rec2.t) {
                return false;
            }

            if (rec1.t < 0) {
                rec1.t = 0;
            }

            auto ray_length = r.direction().length();
            auto distance_within_boundary = (rec2.t - rec1.t) * ray_length;
            auto hit_distance = neg_inverse_density * std::log(random_double());

            if (hit_distance > distance_within_boundary) { 
                return false;
            }

            rec.t = rec1.t + hit_distance / ray_length;
            rec.p = r.at(rec.t);

            rec.normal = vec3(1,0,0);
            rec.front_face = true;
            rec.mat = phase_function; //texture

            return true;
        }

        aabb bounding_box() const override { return boundary->bounding_box(); }

    private:
        shared_ptr<hittable> boundary;
        double neg_inverse_density;
        shared_ptr<material> phase_function;
};

#endif