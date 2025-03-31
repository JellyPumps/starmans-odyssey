/*
* Created by Sarthak Rai on 3/31/25.
*/

#include "Math/Matrix4x4.hpp"
#include <ostream>
#include <utility>
#include <cassert>

namespace Math {
  Matrix4x4::Matrix4x4() : data {{
  {1, 0, 0, 0},
  {0, 1, 0, 0},
  {0, 0, 1, 0},
  {0, 0, 0, 1}
   }} {}

  Matrix4x4::Matrix4x4(const std::array<std::array<float, 4>, 4> &cols) : data(cols) {}

  Matrix4x4 Matrix4x4::identity() { return {}; }

  Matrix4x4 Matrix4x4::zero() { return Matrix4x4({{
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  }}); }


  Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &other) const {
    Matrix4x4 result;

    #if defined(USE_SSE)
      // x86 SSE intrinsics
      for (int i = 0; i < 4; i++) {
        __m128 res = _mm_setzero_ps();
        for (int j = 0; j < 4; j++) {
          __m128 r_v = _mm_loadu_ps(&data[j][0]);
          __m128 scalar = _mm_set1_ps(other.data[i][j]);
          res = _mm_add_ps(res, _mm_mul_ps(r_v, scalar));
        }
        _mm_storeu_ps(&result[i][0], res);
      }
    #elif defined(USE_NEON)
      // ARM NEON intrinsics
      for (int i = 0; i < 4; i++) {
        float32x4_t res = vdupq_n_f32(0);
        for (int j = 0; j < 4; j++) {
          float32x4_t r_v = vld1q_f32(&data[j][0]);
          float32x4_t scalar = vdupq_n_f32(other.data[i][j]);
          res = vmlaq_f32(res, r_v, scalar);
        }
        vst1q_f32(&result[i][0], res);
      }
    #else
      // Non-SSE fallback
      for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
          float sum = 0.0f;
          for (int k = 0; k < 4; ++k) {
            sum += data[k][j] * other[i][k];
          }
          result[i][j] = sum;
        }
      }
    #endif

    return result;
  }


}
