/*
* Created by Sarthak Rai on 3/31/25.
*/

#pragma once

namespace Math {

class Vector2 {
public:
    Vector2(const float x, const float y) : x_(x), y_(y) {}
private:
    // Floats
    float x_;
    float y_;
};

class Vector3{
public:
  Vector3(const float x, const float y, const float z) : x_(x), y_(y), z_(z) {}
private:
    // Floats
    float x_;
    float y_;
    float z_;
};

} // Math
