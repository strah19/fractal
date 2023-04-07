#include "glfw_window.h"
#include "log.h"

#include <cstdio>

namespace Fractal {
    static void error_callback(int error, const char* description);
    static uint32_t glfw_window_counter = 0;

    GLFWWindow::GLFWWindow(WindowProperties properties, const EventCallbackFn& event_callback) : Window(properties, event_callback) {
        glfwSetErrorCallback(error_callback);
        if (!glfwInit()) FRACTAL_LOG_ERROR("failed to initialize glfw.\n");

        m_window = glfwCreateWindow(properties.m_width, properties.m_height, properties.m_name, nullptr, nullptr);
        if (!m_window) FRACTAL_LOG_ERROR("failed to create glfw window.\n");

        glfw_window_counter++;

        glfwMakeContextCurrent(m_window);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        glfwSwapInterval(1);

        glfwSetWindowUserPointer(m_window, &m_event_callback);

        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
			EventCallbackFn& m_event_callback = *(EventCallbackFn*)glfwGetWindowUserPointer(window);

			ResizeEvent event(width, height);
			m_event_callback(event);
		});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
			EventCallbackFn& m_event_callback = *(EventCallbackFn*)glfwGetWindowUserPointer(window);

			QuitEvent event;
			m_event_callback(event);
		});

        m_destroyed = false;
    }

    GLFWWindow::~GLFWWindow() {
        if (!m_destroyed)
            destroy();
    }

    void GLFWWindow::destroy() {
        m_destroyed = true;
        glfw_window_counter--;
        glfwDestroyWindow(m_window);
    }

    void GLFWWindow::quit() {
        glfwTerminate();
    }

    void* GLFWWindow::get_native_window() {
        return m_window;
    }

    void GLFWWindow::update() {
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    void error_callback(int error, const char* description) {
        fprintf(stderr, "glfw error: %s.\n", description);
    }
} // namespace Fractal