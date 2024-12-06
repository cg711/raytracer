/**
 * Casey Gehling
 * 
 * Defines tri intersection logic as well as mesh import and rendering logic.
 */


#ifndef TRI_H
#define TRI_H

#include "hittable.h"
#include "third_party/tiny_obj_loader.h"

class tri : public hittable {
    public:
        tri(const point3& Q, const vec3& u, const vec3& v, shared_ptr<material> mat) : Q(Q), u(u), v(v), mat(mat) {

            auto n = cross(u, v);
            normal = unit_vector(n);
            D = dot(normal, Q);
            w = n / dot(n,n);

            set_bounding_box();
        }

        virtual void set_bounding_box() {
            auto bbox_diagonal1 = aabb(Q, Q + u + v);
            auto bbox_diagonal2 = aabb(Q + u, Q  + v);
            bbox = aabb(bbox_diagonal1, bbox_diagonal2);
        }

        aabb bounding_box() const override { return bbox; }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        // Calculate the dot product of the normal and ray direction (denominator)
            double denom = dot(normal, r.direction());

            // Early exit if the ray is parallel to the plane
            if (std::fabs(denom) < 1e-8) {
                return false;
            }

            // Calculate intersection parameter t
            double t = (D - dot(normal, r.origin())) / denom;

            // Exit early if t is outside the valid interval
            if (!ray_t.contains(t)) {
                return false;
            }

            // Calculate the intersection point
            point3 intersection = r.at(t);

            // Perform the barycentric coordinate test
            vec3 planar_hitpt = intersection - Q;
            double alpha = dot(w, cross(planar_hitpt, v));
            double beta = dot(w, cross(u, planar_hitpt));

            // Check if the intersection point is inside the triangle
            if (alpha < 0 || beta < 0 || (alpha + beta) > 1) {
                return false;
            }

            // Populate the hit record
            rec.t = t;
            rec.p = intersection;
            rec.u = alpha;
            rec.v = beta;
            rec.mat = mat;
            rec.set_face_normal(r, normal);

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


// Takes .obj file path input and material, parses obj into individual tris returned in hittable list.
inline shared_ptr<hittable_list> mesh(
    std::string input_file,
    shared_ptr<material> mat
) {
    // Load the OBJ file
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, input_file.c_str());

    if (!success) {
        std::cerr << "Error loading OBJ file: " << err << std::endl;
        exit(EXIT_FAILURE);
    }

    auto tris = make_shared<hittable_list>();

    // Process shapes
    for (const auto& shape : shapes) {
        size_t index_offset = 0;

        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f) {
            int fv = shape.mesh.num_face_vertices[f];

            // Skip non-triangular faces
            if (fv != 3) {
                index_offset += fv;
                continue;
            }

            // Retrieve the vertices of the triangle
            std::vector<tinyobj::index_t> face_indices;
            for (size_t v = 0; v < fv; ++v) {
                face_indices.push_back(shape.mesh.indices[index_offset + v]);
            }
            index_offset += fv;

            // Get vertex positions
            point3 Q(
                attrib.vertices[3 * face_indices[0].vertex_index],
                attrib.vertices[3 * face_indices[0].vertex_index + 1],
                attrib.vertices[3 * face_indices[0].vertex_index + 2]
            );

            point3 v1(
                attrib.vertices[3 * face_indices[1].vertex_index],
                attrib.vertices[3 * face_indices[1].vertex_index + 1],
                attrib.vertices[3 * face_indices[1].vertex_index + 2]
            );

            point3 v2(
                attrib.vertices[3 * face_indices[2].vertex_index],
                attrib.vertices[3 * face_indices[2].vertex_index + 1],
                attrib.vertices[3 * face_indices[2].vertex_index + 2]
            );

            // Calculate u and v vectors
            vec3 u = v1 - Q;
            vec3 v = v2 - Q;

            // Add the triangle
            tris->add(make_shared<tri>(Q, u, v, mat)); 
        }
    }

    std::clog << "Finished loading obj" << std::endl;

    return tris;
}



#endif