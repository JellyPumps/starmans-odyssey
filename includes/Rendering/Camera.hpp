/*
* Created by Sarthak Rai on 3/31/25.
*/

#pragma once

#include "Math/Vectors.hpp"

namespace Renderer {

class Camera {
public:
    //
private:
    // Vectors
    Math::Vector3 position_;
    Math::Vector3 rotation_; // Radians

    // Floats
    float fov_ = 0; // Field of view
    float aspect_ratio_ = 0; // Width / Height
    float near_plane_ = 0; // Near plane distance
    float far_plane_ = 0; // Far plane distance

};

} // Renderer
