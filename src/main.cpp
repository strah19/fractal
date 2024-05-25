#include <iostream>
#include "application.h"
#include "entry_point.h"
#include "fractal.h"
#include "imgui.h"
#include <glm/glm.hpp>
#include <glfw/glfw3.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

class Sandbox : public Fractal::Application {
public:
    void on_create() {
		camera = Fractal::PerspectiveCameraController({ WINDOW_WIDTH, WINDOW_HEIGHT }, cam_p);

        renderer = new Fractal::Renderer;
		Fractal::SetRenderer(renderer);

        texture = new Fractal::Texture();
        texture->initialize("resources/texture.png");
    }

    void on_update() {
        camera.Update();

        if (p.y > 0) {
            p.x = i_p.x + (i_v.x * t);
            p.y = i_p.y + (i_v.y * t) + (.5 * g * (t * t));
            p.z = i_p.z + (i_v.z * t);
            
            if (p.x - p_delta.x > .005 || p.y - p_delta.y > .005 || p.z - p_delta.z > .005) {
                p_delta = p;
                if (traj_index < 1000)
                    trajectory_points[traj_index++] = p;
            }
        }

        Fractal::RendererCommands::clear(background_color.r, background_color.g, background_color.b, background_color.a);

		renderer->BeginScene(&camera.GetCamera());
		
        texture->bind();
        render_plane();


		renderer->EndScene();
        t += get_frame_time();
        traj_timer = t;
    }

    void render_plane() {
        Fractal::Cube::DrawCube({ 0, 0, 0 }, { WINDOW_WIDTH, line_thickness, line_thickness }, { 0, 0, 0, 1 });
        Fractal::Cube::DrawCube({ 0, 0, 0 }, { line_thickness, WINDOW_HEIGHT, line_thickness }, { 0, 0, 0, 1 });
        Fractal::Cube::DrawCube({ 0, 0, 0 }, { line_thickness, line_thickness, WINDOW_HEIGHT }, { 0, 0, 0, 1 });

        Fractal::Quad::DrawQuad(p, { 1, 1 }, texture->get_texture_id());
        for (int i = 0; i < traj_index; i++) {
            Fractal::Cube::DrawCube(trajectory_points[i], { .03, .03, .03 }, {1, 0, 0, 1});
        }
    }

    void on_gui() {
        if (!camera.get_mode() || camera.get_freeze_state()) {
            ImGui::Begin("Control Panel");

            ImGui::SliderFloat("Line Thickness", &line_thickness, 0.0f, 1.0f);
            ImGui::ColorPicker4("Background Color", &background_color.x);
            ImGui::Separator();
            glm::vec3 position = camera.GetCamera().GetPosition();
            ImGui::Text("Camera Position: %f %f %f", position.x, position.y, position.z);

            ImGui::Separator();
            ImGui::SliderFloat3("Inital Position", &i_p[0], -100, 100);
            ImGui::SliderFloat3("Inital Velocity", &i_v[0], -20, 20);
            ImGui::End();

            ds_gui();
        }
    }

    void ds_gui() {
        Fractal::DeviceStatistics ds = renderer->GetGraphicsDevice()->get_device_stats();

        ImGui::Begin("Device Statistics");
        ImGui::Separator();
        ImGui::Text("Draw Count: %d", ds.draw_count);
        ImGui::Separator();
        ImGui::Text("Vertex Count: %d", ds.num_of_vertices);
        ImGui::Text("Index Count: %d", ds.num_of_indices);
        ImGui::Separator();
        ImGui::Text("Max Vertex Count: %d", ds.max_vertex_count);
        ImGui::Text("Max Index Count: %d", ds.max_index_count);
        ImGui::Separator();
        ImGui::Text("FPS: %d", get_fps());
        ImGui::End();
    }

    ~Sandbox() {
        delete renderer;
        delete texture;
    }

    void on_user_event(Fractal::Event& event) {
        camera.OnEvent(event);
        Fractal::EventDispatcher dispatch(&event);
        dispatch.dispatch<Fractal::KeyboardEvents>(BIND_EVENT(keyboard_event));
    }

	void keyboard_event(Fractal::KeyboardEvents& keyboard) {
        if (keyboard.GetKeyPress(GLFW_KEY_R)) {
            p = i_p;
            p_delta = p;
            t = 0;
            traj_index = 0;
        }
    }
private:
    Fractal::PerspectiveCameraController camera;
    Fractal::Renderer* renderer;

    Fractal::Texture* texture;

    float g = -9.81;
    double t = 0.0;
    glm::vec3 p = { 0, 0, 0 };
    glm::vec3 i_p = { 0, 30, 0 };
    glm::vec3 i_v = { 0, 0, 0 };

    glm::vec3 cam_p = { -17, 12, 24 };

    glm::vec3 trajectory_points[1000];
    float traj_timer = 0;
    int traj_index = 0;
    glm::vec3 p_delta = { 0, 0, 0 };

    glm::vec4 background_color = { 0.4, 0.6, 0.6, 1 };
    float line_thickness = 0.01f;
};

Fractal::Application* Fractal::create_application() {
    Sandbox* sandbox = new Sandbox;
    sandbox->initialize("Sandbox", WINDOW_WIDTH, WINDOW_HEIGHT, WINF_FULLSCREEN);
    return sandbox;
}