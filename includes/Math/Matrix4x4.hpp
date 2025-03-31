/*
* Created by Sarthak Rai on 3/31/25.
*/

#pragma once

#include "Vectors.hpp"

#include <array> // Stack storage
#include <cmath>

#if defined(__x86_64__) || defined(_M_X64)
    #include <immintrin.h>  // SSE/AVX
    #define USE_SSE
#elif defined(__aarch64__) || defined(_M_ARM64)
    #include <arm_neon.h>   // NEON
    #define USE_NEON
#endif

namespace Math {
    class Matrix4x4 {
    public:
        // Column-major storage
        union {
            std::array<std::array<float, 4>, 4> data;
            float elements[16];
        };

        // Constructors
        Matrix4x4() : elements{1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1} {};
        explicit Matrix4x4(const std::array<std::array<float, 4>, 4>& cols);

        // Static Factories
        static Matrix4x4 identity() { return {}; }
        static Matrix4x4 zero();
        
        // Operations
        Matrix4x4 operator*(const Matrix4x4& other) const;
        Vector3 operator*(const Vector3& vector3) const;
        Vector3 transformation_direction(const Vector3& dir) const;

        // Utitlity
        Matrix4x4 inverse() const;
        Matrix4x4 transpose() const;
        float determinant() const;
        [[nodiscard]] Matrix4x4 multiply_scalar(const Matrix4x4 &other) const;
        [[nodiscard]] Matrix4x4 multiply_sse(const Matrix4x4 &other) const;

        // Transformations
        static Matrix4x4 translation(const Vector3& trans);
        static Matrix4x4 rotation_x(float angle);
        static Matrix4x4 rotation_y(float angle);
        static Matrix4x4 rotation_z(float angle);
        static Matrix4x4 rotationxyz(Vector3 radians);
        static Matrix4x4 perspective(float fov, float aspect, float near, float far);
        static Matrix4x4 orthographic(float left, float right, float bottom, float top, float near, float far);
        static Matrix4x4 look_at(const Vector3& eye, const Vector3& target, const Vector3& up = Vector3(0, 1, 0));


        // Accessors
        const float* _data() const { return elements; }
        float* _data() { return elements; }
    };
}

