#pragma once
#include <cmath>
#include <array>

namespace Math {

class Vector2 {
public:
    // Construction
    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(float x, float y) : x(x), y(y) {}
    
    // Constants
    static Vector2 zero() { return {0, 0}; }
    static Vector2 one() { return {1, 1}; }
    static Vector2 unit_x() { return {1, 0}; }
    static Vector2 unit_y() { return {0, 1}; }

    // Arithmetic operators
    Vector2 operator+(const Vector2& rhs) const { return {x + rhs.x, y + rhs.y}; }
    Vector2 operator-(const Vector2& rhs) const { return {x - rhs.x, y - rhs.y}; }
    Vector2 operator*(float scalar) const { return {x * scalar, y * scalar}; }
    Vector2 operator/(float scalar) const { return *this * (1.0f / scalar); }
    
    // Compound assignment
    Vector2& operator+=(const Vector2& rhs) { x += rhs.x; y += rhs.y; return *this; }
    Vector2& operator-=(const Vector2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
    Vector2& operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }
    Vector2& operator/=(float scalar) { return *this *= (1.0f / scalar); }
    
    // Comparison
    bool operator==(const Vector2& rhs) const { return x == rhs.x && y == rhs.y; }
    bool operator!=(const Vector2& rhs) const { return !(*this == rhs); }
    
    // Vector operations
    [[nodiscard]] float dot(const Vector2& rhs) const { return x * rhs.x + y * rhs.y; }
    [[nodiscard]] float length() const { return std::sqrt(length_squared()); }
    [[nodiscard]] float length_squared() const { return dot(*this); }
    [[nodiscard]] Vector2 normalized() const { return *this / length(); }
    void normalize() { *this /= length(); }
    
    // Access
    const float* _data() const { return &x; }
    float* _data() { return &x; }

    float x, y;
};

class Vector3 {
public:
    // Construction
    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    
    // Constants
    static Vector3 zero() { return {0, 0, 0}; }
    static Vector3 one() { return {1, 1, 1}; }
    static Vector3 unit_x() { return {1, 0, 0}; }
    static Vector3 unit_y() { return {0, 1, 0}; }
    static Vector3 unit_z() { return {0, 0, 1}; }
    static Vector3 up() { return unit_y(); }
    static Vector3 right() { return unit_x(); }
    static Vector3 forward() { return unit_z(); }

    // Arithmetic operators
    Vector3 operator+(const Vector3& rhs) const { return {x + rhs.x, y + rhs.y, z + rhs.z}; }
    Vector3 operator-(const Vector3& rhs) const { return {x - rhs.x, y - rhs.y, z - rhs.z}; }
    Vector3 operator*(float scalar) const { return {x * scalar, y * scalar, z * scalar}; }
    Vector3 operator/(float scalar) const { return *this * (1.0f / scalar); }
    
    // Compound assignment
    Vector3& operator+=(const Vector3& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
    Vector3& operator-=(const Vector3& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
    Vector3& operator*=(float scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
    Vector3& operator/=(float scalar) { return *this *= (1.0f / scalar); }
    
    // Unary
    Vector3 operator-() const { return {-x, -y, -z}; }
    
    // Comparison
    bool operator==(const Vector3& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
    bool operator!=(const Vector3& rhs) const { return !(*this == rhs); }
    
    // Vector operations
    [[nodiscard]] float dot(const Vector3& rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z; }
    [[nodiscard]] Vector3 cross(const Vector3& rhs) const {
        return {
            y * rhs.z - z * rhs.y,
            z * rhs.x - x * rhs.z,
            x * rhs.y - y * rhs.x
        };
    }
    
    [[nodiscard]] float length() const { return std::sqrt(length_squared()); }
    [[nodiscard]] float length_squared() const { return dot(*this); }
    [[nodiscard]] Vector3 normalized() const { return *this / length(); }
    void normalize() { *this /= length(); }
    
    // Access
    [[nodiscard]] const float* _data() const { return &x; }
    float* _data() { return &x; }

    float x, y, z;
};

// Non-member operators
inline Vector3 operator*(float scalar, const Vector3& vec) { return vec * scalar; }
inline Vector2 operator*(float scalar, const Vector2& vec) { return vec * scalar; }

} // Math