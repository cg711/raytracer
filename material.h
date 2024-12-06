/**
 * Casey Gehling
 * 
 * Defines multiple materials applicable to the surfaces of intersectables.
 */

#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "texture.h"

class material {
    public:
        virtual ~material() = default;

        virtual color emitted(double u, double v, const point3& p) const {
            return color(0,0,0);
        }

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
                return false;
        }
};

// Diffuse material
class lambertian : public material {
    public:
        // Solid color diffuse
        lambertian(const color& albedo) : texture(make_shared<solid_color>(albedo)) {}
        // Textured diffuse
        lambertian(shared_ptr<texture> tex) : texture(tex) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            auto scatter_direction = rec.normal + random_unit_vector();

            if (scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }

            scattered = ray(rec.p, scatter_direction, r_in.time());
            attenuation = texture->value(rec.u, rec.v, rec.p);
            return true;
        }

    private:
        shared_ptr<texture> texture;
};

// Metallic material
class metal : public material {
    public:
        // Solid color metal
        metal(const color& albedo, double fuzz) : tex(make_shared<solid_color>(albedo)), fuzz(fuzz < 1 ? fuzz : 1) {}

        // Textured metal
        metal(shared_ptr<texture> tex, double fuzz) : tex(tex), fuzz(fuzz < 1 ? fuzz : 1) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
            scattered = ray(rec.p, reflected, r_in.time());
            attenuation = tex->value(rec.u, rec.v, rec.p);
            return (dot(scattered.direction(), rec.normal) > 0);
        }
    
    private:
        shared_ptr<texture> tex;
        double fuzz;
};

// Dielectric material (glass)
class dielectric : public material {
    public: 

        // Refraction index determines how much the path of light is bend. Specify angle.
        dielectric(double refraction_index) : refraction_index(refraction_index) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            attenuation = color(1.0,1.0,1.0);
            double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;

            vec3 unit_direction = unit_vector(r_in.direction());

            double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

            bool cannot_refact = ri * sin_theta > 1.0;
            vec3 direction;

            if (cannot_refact || reflectance(cos_theta, ri) > random_double()) {
                direction = reflect(unit_direction, rec.normal);
            } else {
                direction = refract(unit_direction, rec.normal, ri);
            }

            scattered = ray(rec.p, direction, r_in.time());
            
            return true;
        }
    private:
        double refraction_index;

        static double reflectance(double cosine, double refraction_index) {
            // Schlicks
            auto r0 = (1 - refraction_index) / (1 + refraction_index);
            r0 = r0 * r0;
            return r0 + (1 - r0) * std::pow((1 - cosine),5);
        }

};

// Point light material
class diffuse_light : public material {
    public:

        // Textured diffuse
        diffuse_light(shared_ptr<texture> texture) : tex(texture) {}

        // Colored diffuse
        diffuse_light(const color& emit) : tex(make_shared<solid_color>(emit)) {}

        // Emit color of texture to surrounding intersectables
        color emitted(double u, double v, const point3& p) const override {
            return tex->value(u,v,p);
        }
    private:
        shared_ptr<texture> tex;
};

// Volumetric material. Wrap in constant medium.
class isotropic : public material {
    public:
        // Solid color volume material
        isotropic(const color& albedo) : texture(make_shared<solid_color>(albedo)) {}

        // Textured volume material
        isotropic(shared_ptr<texture> texture) : texture(texture) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attentuation, ray& scattered) const override {
            scattered = ray(rec.p, random_unit_vector(), r_in.time());
            attentuation = texture->value(rec.u, rec.v, rec.p);
            return true;
        }
    private:
        shared_ptr<texture> texture;
};

#endif