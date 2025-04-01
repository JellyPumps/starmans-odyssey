/*
* Created by Sarthak Rai on 3/31/25.
* Entry point
*/

#include "Window.hpp"
#include "Input.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/Camera.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Shader.hpp"
#include <SDL2/SDL.h>
#include <glm/gtc/matrix_transform.hpp>

int main() {
    Window window("SDL2 Window", 800, 600, SDL_WINDOW_SHOWN);
    Renderer r(window);
    r.init();

    Input input;
    bool running = true;
    float delta_time = 0.0f;

    // ---- Setup Camera ----
    Camera camera(45.0f, 800.0f/600.0f, 0.1f, 1000.0f);
    camera.set_position(glm::vec3(0.0f, 0.0f, 5.0f));

    // ---- Load Shaders ----
    r.load_shader("basic", "assets/shaders/basic.vert", "assets/shaders/basic.frag");

    // ---- Cube ----
    float size = 1.0f;
    float half = size * 0.5f;

    std::vector<Vertex> vertices = {
            // ---- Front Face ----
            {{-half, -half,  half}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
            {{ half, -half,  half}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
            {{ half,  half,  half}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{-half,  half,  half}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},

            // --- Back Face ----
            {{-half, -half, -half}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
            {{ half, -half, -half}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
            {{ half,  half, -half}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
            {{-half,  half, -half}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},

            // ---- Top Face ----
            {{-half,  half,  half}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
            {{ half,  half,  half}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
            {{ half,  half, -half}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{-half,  half, -half}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},

            // ---- Bottom Face ----
            {{-half, -half,  half}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
            {{ half, -half,  half}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
            {{ half, -half, -half}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
            {{-half, -half, -half}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},

            // ---- Right Face ----
            {{ half, -half,  half}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{ half, -half, -half}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{ half,  half, -half}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
            {{ half,  half,  half}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

            // ---- Left Face ----
            {{-half, -half,  half}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{-half, -half, -half}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{-half,  half, -half}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
            {{-half,  half,  half}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}
        };

        std::vector<unsigned int> indices = {
            // Front face
            0, 1, 2, 2, 3, 0,
            // Back face
            4, 5, 6, 6, 7, 4,
            // Top face
            8, 9, 10, 10, 11, 8,
            // Bottom face
            12, 13, 14, 14, 15, 12,
            // Right face
            16, 17, 18, 18, 19, 16,
            // Left face
            20, 21, 22, 22, 23, 20
        };

    Mesh cube(vertices, indices);


    // ---- Event Callbacks ----
    input.set_window_event_callback([&running, &window, r, &camera](const SDL_Event& event){
        if (event.window.event == SDL_WINDOWEVENT_CLOSE) { running = false; }
        else if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
            int new_width = event.window.data1;
            int new_height = event.window.data2;
            window.resize(new_width, new_height);
            r.set_viewport(0, 0, new_width, new_height);
            camera.set_perspective(45.0f, static_cast<float>(new_width)/new_height, 0.1f, 1000.0f);
        }
    });

    Uint32 last_frame_time = SDL_GetTicks();
    while (running) {
        // ---- Calculate ----
        Uint32 current_time = SDL_GetTicks();
        delta_time = (current_time - last_frame_time) / 1000.0f;
        last_frame_time = current_time;

        // ---- Events -----
        input.process_events();

        if (input.is_key_down(SDLK_ESCAPE)) { running = false; }

        // ---- Camera ----
        camera.update_from_input(delta_time, input);

        // ---- Renderer ----
        r.begin_frame();
        r.submit_camera(camera);

        // ---- Draw ----
        if (Shader *shader = r.get_shader("basic")) {
            static float rotation = 0.0f;
            rotation += 3.0f * delta_time;

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

            shader->set_uniform("uViewPos", camera.get_position());
            shader->set_uniform("uObjectColor", glm::vec3(1.0f, 0.5f, 0.31f));

            r.draw_mesh(cube, *shader, model);
        }

        // ---- Updates ----
        window.swap_buffers();

        // ---- End Frame ----
        r.end_frame();

        // Cap to 60 FPS
        Uint32 frame_time = SDL_GetTicks() - current_time;
        if (frame_time > 16) { SDL_Delay(16 - frame_time); }
    }

    SDL_Quit();
    return 0;
}
