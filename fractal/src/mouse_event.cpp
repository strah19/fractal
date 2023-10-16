#include "mouse_event.h"
#include <GLFW/glfw3.h>
#include "application.h"

namespace Fractal {
    glm::vec2 MousePositionEvent::GetMousePosition() {
        static double xpos = 0;
        static double ypos = 0;

        glfwGetCursorPos(static_cast<GLFWwindow*>(Application::get().get_window()->get_native_window()), &xpos, &ypos);
        return glm::vec2(xpos, ypos);
    }

    void cursor_enable() {
        glfwSetInputMode(static_cast<GLFWwindow*>(Application::get().get_window()->get_native_window()), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

	void cursor_disable() {
        glfwSetInputMode(static_cast<GLFWwindow*>(Application::get().get_window()->get_native_window()), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}