/*
* Created by Sarthak Rai on 3/31/25.
*/

#pragma once

#include "Math/Vectors.hpp"
#include "Math/Matrix4x4.hpp"

namespace Renderer {
    class Camera {
    public:
        enum class ProjectionType {
            Perspective,
            Orthographic
        };

        Camera(float fov_deg = 60.0f, float aspect_ration = 16.0f/9.0f, float near_plane = 0.1f, float far_plane = 1000.0f);

        void update_matrices();

        // Getters
        [[nodiscard]] Math::Matrix4x4 get_view_matrix() const { return view_; }
        [[nodiscard]] Math::Matrix4x4 get_projection_matrix() const { return projection_; }
        [[nodiscard]] Math::Matrix4x4 get_view_projection_matrix() const { return view_projection_; }

        [[nodiscard]] Math::Vector3 get_position() const { return position_; }
        [[nodiscard]] Math::Vector3 get_rotation() const { return rotation_; }
        [[nodiscard]] Math::Vector3 get_forward() const { return forward_; }
        [[nodiscard]] Math::Vector3 get_up() const { return up_; }
        [[nodiscard]] Math::Vector3 get_right() const { return right_; }

        [[nodiscard]] float get_fov() const { return fov_; }
        [[nodiscard]] float get_near_plane() const { return near_plane_; }
        [[nodiscard]] float get_far_plane() const { return far_plane_; }

        // Setters
        void set_position(const Math::Vector3 &position);
        void set_rotation(const Math::Vector3 &euler_degrees);
        void look_at(const Math::Vector3 &target, const Math::Vector3 &up = Math::Vector3(0, 1, 0));
        void set_perspective(float fov_deg, float aspect_ratio, float near_plane, float far_plane);
        void set_orthographic(float size, float aspect_ratio, float near_plane, float far_plane);
        void set_projection_type(ProjectionType type) {
            projection_type_ = type;
            dirty_ = true;
        }

        // Movement
        void translate(const Math::Vector3 &translation);
        void rotate(float pitch_deg, float yaw_deg, float roll_deg = 0.0f);
    private:
        // Camera state
        Math::Vector3 position_ = Math::Vector3(0, 0, 0);
        Math::Vector3 rotation_ = Math::Vector3(0, 0, 0); // Degrees
        Math::Vector3 forward_ = Math::Vector3(0, 0, -1);
        Math::Vector3 up_ = Math::Vector3(0, 1, 0);
        Math::Vector3 right_ = Math::Vector3(1, 0, 0);

        // Projection
        ProjectionType projection_type_ = ProjectionType::Perspective;
        float fov_ = 60.0f;
        float ortho_size_ = 10.0f; // World Units
        float aspect_ratio_ = 16.0/9.0f;
        float near_plane_ = 0.1f;
        float far_plane_ = 1000.0f;

        // Matrices
        Math::Matrix4x4 view_;
        Math::Matrix4x4 projection_;
        Math::Matrix4x4 view_projection_;
        bool dirty_ = true;

        // Input state
        float movement_speed_ = 16.0f;
        float rotation_speed_ = 1.0f;
        float mouse_sensitivity_ = 0.1f;
        Math::Vector2 last_mouse_position_ = Math::Vector2(0, 0);
        bool first_mouse_ = true;

        void update_basis_vectors();
    };
} // Renderer
