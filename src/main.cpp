#include <iostream>
#include "application.h"
#include "entry_point.h"
#include "fractal.h"
#include "imgui.h"
#include <glm/glm.hpp>

class Sandbox : public Fractal::Application {
public:
    void on_create() {
		camera = Fractal::OrthoCamera(0, 600, 600, 0);
        camera.SetPosition({ 0, 0, 0 });

        renderer = new Fractal::Renderer;
    }

    void on_update() {
        Fractal::RendererCommands::clear(1.0f, 1.0f, 0.0f, 1.0f);

		renderer->BeginScene(&camera);
		Fractal::Quad::Renderer(renderer);
		Fractal::Quad::DrawQuad({ 100, 100, 0 }, { 100, 100 }, { 1, 0, 0, 1 });
		renderer->EndScene();
    }

    void on_gui() {
	    ImGui::Begin("window");

        ImGui::End();
    }

    ~Sandbox() {
        delete renderer;
    }

    void on_user_event(Fractal::Event& event) {

    }
private:
    Fractal::OrthoCamera camera;
    Fractal::Renderer* renderer;
};

Fractal::Application* Fractal::create_application() {
    Sandbox* sandbox = new Sandbox;
    sandbox->initialize("Sandbox", 600, 600);
    return sandbox;
}
