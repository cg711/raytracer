#ifndef QUAD_H
#define QUAD_H

#include "hittable.h"

class quad : public hittable {
    public:
        quad(const point3& Q, const vec3& u, const vec3& v, shared_ptr<material> mat, bool inward_normals = false)
            : Q(Q), u(u), v(v), mat(mat) {

            // Compute the normal vector
            auto n = cross(u, v);
            normal = unit_vector(inward_normals ? -n : n); // Flip normal if inward_normals is true
            D = dot(normal, Q);
            w = n / dot(n, n);

            set_bounding_box();
        }

        virtual void set_bounding_box() {
            auto bbox_diagonal1 = aabb(Q, Q + u + v);
            auto bbox_diagonal2 = aabb(Q + u, Q + v);
            bbox = aabb(bbox_diagonal1, bbox_diagonal2);
        }

        aabb bounding_box() const override { return bbox; }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            auto denom = dot(normal, r.direction());

            // Don't hit if ray is parallel to plane
            if (std::fabs(denom) < 1e-8) {
                return false;
            }

            // Return false if hit point t is outside ray interval
            auto t = (D - dot(normal, r.origin())) / denom;
            if (!ray_t.contains(t)) {
                return false;
            }

            auto intersection = r.at(t);

            // Determine if ray hits within shape
            vec3 planar_hitpt = intersection - Q;
            auto alpha = dot(w, cross(planar_hitpt, v));
            auto beta = dot(w, cross(u, planar_hitpt));

            if (!is_interior(alpha, beta, rec)) {
                return false;
            }

            // Ray hits the shape
            rec.t = t;
            rec.p = intersection;
            rec.mat = mat;
            rec.set_face_normal(r, normal); // Normal direction depends on constructor setting

            return true;
        }

        virtual bool is_interior(double a, double b, hit_record& rec) const {
            interval unit_interval = interval(0, 1);

            if (!unit_interval.contains(a) || !unit_interval.contains(b)) {
                return false;
            }

            rec.u = a;
            rec.v = b;
            return true;
        }

    private:
        point3 Q;
        vec3 u, v;
        vec3 w;
        shared_ptr<material> mat;
        aabb bbox;
        vec3 normal;
        double D;
};


inline shared_ptr<hittable_list> box(const point3& a, const point3& b, shared_ptr<material> mat) {
    // Returns the 3D box (six sides) that contains the two opposite vertices a & b.

    auto sides = make_shared<hittable_list>();

    // Construct the two opposite vertices with the minimum and maximum coordinates.
    auto min = point3(std::fmin(a.x(),b.x()), std::fmin(a.y(),b.y()), std::fmin(a.z(),b.z()));
    auto max = point3(std::fmax(a.x(),b.x()), std::fmax(a.y(),b.y()), std::fmax(a.z(),b.z()));

    auto dx = vec3(max.x() - min.x(), 0, 0);
    auto dy = vec3(0, max.y() - min.y(), 0);
    auto dz = vec3(0, 0, max.z() - min.z());

    sides->add(make_shared<quad>(point3(min.x(), min.y(), max.z()),  dx,  dy, mat)); // front
    sides->add(make_shared<quad>(point3(max.x(), min.y(), max.z()), -dz,  dy, mat)); // right
    sides->add(make_shared<quad>(point3(max.x(), min.y(), min.z()), -dx,  dy, mat)); // back
    sides->add(make_shared<quad>(point3(min.x(), min.y(), min.z()),  dz,  dy, mat)); // left
    sides->add(make_shared<quad>(point3(min.x(), max.y(), max.z()),  dx, -dz, mat)); // top
    sides->add(make_shared<quad>(point3(min.x(), min.y(), min.z()),  dx,  dz, mat)); // bottom

    return sides;
}

// Used for creating a skybox
inline shared_ptr<hittable_list> cube_map(
    shared_ptr<image_texture> left,
    shared_ptr<image_texture> right,
    shared_ptr<image_texture> front,
    shared_ptr<image_texture> back,
    shared_ptr<image_texture> top,
    shared_ptr<image_texture> bottom,
    double sr // scene radius
) {
    auto map = make_shared<hittable_list>();

    auto dx = vec3(2 * sr, 0, 0);
    auto dy = vec3(0, 2 * sr, 0);
    auto dz = vec3(0, 0, 2 * sr);

    // Add quads with texture corrections
    map->add(make_shared<quad>(point3(-sr, -sr, -sr), dz, dy, make_shared<diffuse_light>(left), true)); // left
    map->add(make_shared<quad>(point3(sr, -sr, -sr), dz, dy, make_shared<diffuse_light>(right), true)); // right
    map->add(make_shared<quad>(point3(-sr, -sr, sr), dx, dy, make_shared<diffuse_light>(front), true)); // front
    map->add(make_shared<quad>(point3(-sr, -sr, -sr), dx, dy, make_shared<diffuse_light>(back), true)); // back
    map->add(make_shared<quad>(point3(-sr, sr, -sr), dx, dz, make_shared<diffuse_light>(top), true)); // top
    map->add(make_shared<quad>(point3(-sr, -sr, -sr), dx, dz, make_shared<diffuse_light>(bottom), true)); // bottom

    return map;
}


#endif