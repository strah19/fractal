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
		camera = Fractal::PerspectiveCameraController({ WINDOW_WIDTH, WINDOW_HEIGHT });

        renderer = new Fractal::Renderer;
		Fractal::SetRenderer(renderer);
    }

    void on_update() {
        camera.Update();

        Fractal::RendererCommands::clear(background_color.r, background_color.g, background_color.b, background_color.a);

		renderer->BeginScene(&camera.GetCamera());
		
        render_plane();


		renderer->EndScene();
    }

    void render_plane() {
        Fractal::Cube::DrawCube({ 0, 0, 0 }, { WINDOW_WIDTH, line_thickness, line_thickness }, { 0, 0, 0, 1 });
        Fractal::Cube::DrawCube({ 0, 0, 0 }, { line_thickness, WINDOW_HEIGHT, line_thickness }, { 0, 0, 0, 1 });
        Fractal::Cube::DrawCube({ 0, 0, 0 }, { line_thickness, line_thickness, WINDOW_HEIGHT }, { 0, 0, 0, 1 });
    }

    void on_gui() {
        if (!camera.get_mode() || camera.get_freeze_state() && !show_panel) {
            ImGui::Begin("Control Panel");

            ImGui::SliderFloat("Line Thickness", &line_thickness, 0.0f, 1.0f);
            ImGui::ColorPicker4("Background Color", &background_color.x);
            ImGui::Separator();
            glm::vec3 position = camera.GetCamera().GetPosition();
            ImGui::Text("Camera Position: %f %f %f", position.x, position.y, position.z);
            ImGui::Separator();
            ImGui::Checkbox("Show Panels", &show_panel);
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
        ImGui::End();
    }

    ~Sandbox() {
        delete renderer;
    }

    void on_user_event(Fractal::Event& event) {
        camera.OnEvent(event);
    }
private:
    Fractal::PerspectiveCameraController camera;
    Fractal::Renderer* renderer;

    bool show_panel = false;
    glm::vec4 background_color = { 0.4, 0.6, 0.6, 1 };
    float line_thickness = 0.01f;
};

Fractal::Application* Fractal::create_application() {
    Sandbox* sandbox = new Sandbox;
    sandbox->initialize("Sandbox", WINDOW_WIDTH, WINDOW_HEIGHT, WINF_FULLSCREEN);
    return sandbox;
}