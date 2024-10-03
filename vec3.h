// 3D Vector definitions

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

#endif