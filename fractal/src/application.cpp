#include "application.h"

namespace Fractal {
    Application::~Application() {
        on_destroy(); 
        
        delete m_window;
    }

    void Application::initialize(const char* name, uint32_t width, uint32_t height) {
        m_properties = WindowProperties(name, width, height);

        m_window = Window::create_glfw_window(m_properties, BIND_EVENT(on_event));

        on_create();
    }

    void Application::run() {
        while (!m_window->destroyed()) {
            m_window->update();
            on_update();
        }
    }

    void Application::on_event(Event& event) {
		EventDispatcher dispatcher(&event);

		on_user_event(event);

		dispatcher.dispatch<QuitEvent>(BIND_EVENT(on_close));
		dispatcher.dispatch<ResizeEvent>(BIND_EVENT(on_resize));
	}

	void Application::on_close(const QuitEvent& event) {
		m_window->destroy();
	}

	void Application::on_resize(const ResizeEvent& event) {
		m_window->properties()->m_width = event.m_width;
		m_window->properties()->m_height = event.m_height;
	}
} // namespace Fractal