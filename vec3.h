/**
 * Casey Gehling
 * 
 * Defines vec3 type, <x,y,z> valued tuple.
 */


#ifndef VEC3_H
#define VEC3_H

class vec3 {
    public:

        double e[3];

        // constructors
        vec3() {
            e[0] = 0;
            e[1] = 0;
            e[2] = 0;
        }

        vec3(double e0, double e1, double e2) {
            e[0] = e0;
            e[1] = e1;
            e[2] = e2;
        }

        // getters
        double x() const {return e[0];}
        double y() const {return e[1];}
        double z() const {return e[2];}

        // Invert
        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]);}

        // Index access
        double operator[](int i) const {return e[i];}
        double& operator[](int i) {return e[i];}

        // Vector addition
        vec3& operator+=(const vec3& v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        // Scalar mult
        vec3& operator*=(double s) {
            e[0] *= s;
            e[1] *= s;
            e[2] *= s;
            return *this;
        }

        vec3& operator/=(double t) {
            return *this *= 1/t;
        }

        // Magnitude
        double length() const {
            return std::sqrt(length_squared());
        }

        double length_squared() const {
            return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
        }

        bool near_zero() const {
            auto s = 1e-8;
            return (std::fabs(e[0]) < s) && (std::fabs(e[1] < s)) && (std::fabs(e[2]) < s);
        }

        static vec3 random() {
            return vec3(random_double(), random_double(), random_double());
        }

        static vec3 random(double min, double max) {
            return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
        }
};

// point3 and vec3 are synonymous but useful for clarity
using point3 = vec3;

// Vector utilities

// Output stream 'e1 e2 e3'
inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// +
inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

// -
inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// *
inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

// Scalar *
inline vec3 operator*(const double s, const vec3& v) {
    return vec3(s * v.e[0], s * v.e[1], s * v.e[2]);
}

// Communative scalar *
inline vec3 operator*(const vec3& v, const double s) {
    return s * v;
}

// /
inline vec3 operator/(const vec3& v, double s) {
    return (1/s) * v;
}

// Dot product
inline double dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

// Cross product
inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// Normalize
inline vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

inline vec3 random_in_unit_disk() {
    while (true) {
        auto p = vec3(random_double(-1,1), random_double(-1,1), 0);
        if (p.length_squared() < 1) return p;
    }
}

// Random unit vector
inline vec3 random_unit_vector() {
    while (true) {
        auto p = vec3::random(-1,1);
        auto lensq = p.length_squared();
        // Reject points outside unit circle radius and in floating-point error region
        if (lensq <= 1 && 1e-60 < lensq) {
            return p / sqrt(lensq);
        }
    }
}

inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if(dot(on_unit_sphere, normal) > 0.0) {
        return on_unit_sphere;
    } else {
        return -on_unit_sphere;
    }
}

inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v,n) * n;
}

inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = std::fmin(dot(-uv,n), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

#endif