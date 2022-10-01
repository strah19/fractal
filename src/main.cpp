#include <iostream>
#include "application.h"

class Sandbox : public Fractal::Application {
public:
    void on_create() {
        debug_window = Fractal::Window::create_glfw_window(Fractal::WindowProperties("Debug Window", 400, 400), BIND_EVENT(debugger_window_events));
    }

    void on_update() {
        if (!debug_window->destroyed())
            debug_window->update();
    }

    void on_destroy() {
        delete debug_window;
    }

    void on_user_event(Fractal::Event& event) {

    }

    void debugger_window_events(Fractal::Event& event) {
        Fractal::EventDispatcher dispatcher(&event);
        dispatcher.dispatch<Fractal::QuitEvent>(BIND_EVENT(debugger_window_on_close));
    }

	void debugger_window_on_close(const Fractal::QuitEvent& event) {
		debug_window->destroy();
	}
private:
    Fractal::Window* debug_window;
};

int main() {
    Sandbox sandbox;
    sandbox.initialize("Sandbox", 600, 600);
    sandbox.run();
    return 0;
}