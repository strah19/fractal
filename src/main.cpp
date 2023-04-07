#include <iostream>
#include "application.h"
#include "entry_point.h"
#include "fractal.h"

class Sandbox : public Fractal::Application {
public:
    void on_create() {
    }

    void on_update() {
        Fractal::RendererCommands::clear(1.0f, 0.0f, 0.0f, 1.0f);
    }

    ~Sandbox() {
    }

    void on_user_event(Fractal::Event& event) {

    }
};

Fractal::Application* Fractal::create_application() {
    Sandbox* sandbox = new Sandbox;
    sandbox->initialize("Sandbox", 600, 600);
    return sandbox;
}
