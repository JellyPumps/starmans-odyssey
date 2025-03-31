/*
* Created by Sarthak Rai on 3/31/25.
*/

#pragma once

#include <cmath>
#include <array>    // Stack storage

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
        std::array<std::array<float, 4>, 4> data;

        // Constructors
        Matrix4x4();
        explicit Matrix4x4(const std::array<std::array<float, 4>, 4>& cols);

        // Static Factories
        static Matrix4x4 identity();
        static Matrix4x4 zero();

        // SIMD Accelerated operations
        Matrix4x4 operator*(const Matrix4x4& other) const;

        // Accessors
        const float *operator[](size_t col) const { return data[col].data(); }
        float *operator[](size_t col) { return data[col].data(); }
    };
}

