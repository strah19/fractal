#include "window.h"
#include "glfw_window.h"
#include "error.h"

namespace Fractal {
    Window::Window(WindowProperties properties, const EventCallbackFn& event_callback) : m_properties(properties), m_event_callback(event_callback) { }

    Window* Window::create_glfw_window(WindowProperties properties, const EventCallbackFn& event_callback) {
        return new GLFWWindow(properties, event_callback);
    }
} // namespace Fractal