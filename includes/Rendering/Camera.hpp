/*
* Created by Sarthak Rai on 3/31/25.
*/

#pragma once

#include "Input.hpp"
#include "glm/glm.hpp"

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
        [[nodiscard]] glm::mat4x4 get_view_matrix() const { return view_; }
        [[nodiscard]] glm::mat4x4 get_projection_matrix() const { return projection_; }
        [[nodiscard]] glm::mat4x4 get_view_projection_matrix() const { return view_projection_; }

        [[nodiscard]] glm::vec3 get_position() const { return position_; }
        [[nodiscard]] glm::vec3 get_rotation() const { return rotation_; }
        [[nodiscard]] glm::vec3 get_forward() const { return forward_; }
        [[nodiscard]] glm::vec3 get_up() const { return up_; }
        [[nodiscard]] glm::vec3 get_right() const { return right_; }

        [[nodiscard]] float get_fov() const { return fov_; }
        [[nodiscard]] float get_near_plane() const { return near_plane_; }
        [[nodiscard]] float get_far_plane() const { return far_plane_; }

        // Setters
        void set_position(const glm::vec3 &position);
        void set_rotation(const glm::vec3 &euler_degrees);
        void look_at(const glm::vec3 &target, glm::vec3 up = glm::vec3(0, 1, 0));
        void set_perspective(float fov_deg, float aspect_ratio, float near_plane, float far_plane);
        void set_orthographic(float size, float aspect_ratio, float near_plane, float far_plane);
        void set_projection_type(ProjectionType type) {
            projection_type_ = type;
            dirty_ = true;
        }

        // Movement
        void translate(const glm::vec3 &translation);
        void rotate(float pitch_deg, float yaw_deg, float roll_deg = 0.0f);

        // Input
        void update_from_input(float delta_time, const Input &input);
    private:
        // Camera state
        glm::vec3 position_ = glm::vec3(0, 0, 0);
        glm::vec3 rotation_ = glm::vec3(0, 0, 0); // Degrees
        glm::vec3 forward_ = glm::vec3(0, 0, 1);
        glm::vec3 up_ = glm::vec3(0, 1, 0);
        glm::vec3 right_ = glm::vec3(1, 0, 0);

        // Projection
        ProjectionType projection_type_ = ProjectionType::Perspective;
        float fov_ = 60.0f;
        float ortho_size_ = 10.0f; // World Units
        float aspect_ratio_ = 16.0/9.0f;
        float near_plane_ = 0.1f;
        float far_plane_ = 1000.0f;

        // Matrices
        glm::mat4x4 view_;
        glm::mat4x4 projection_;
        glm::mat4x4 view_projection_;
        bool dirty_ = true;

        // Input state
        float movement_speed_ = 16.0f;
        float rotation_speed_ = 1.0f;
        float mouse_sensitivity_ = 0.1f;
        glm::vec2 last_mouse_position_ = glm::vec2(0, 0);
        bool first_mouse_ = true;

        void update_basis_vectors();
    };
} // Renderer
