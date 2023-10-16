#include "keyboard_event.h"
#include <GLFW/glfw3.h>
#include "application.h"

namespace Fractal {
    int KeyboardEvents::GetKeyBoardState(int const keycode) {
        return glfwGetKey(static_cast<GLFWwindow*>(Application::get().get_window()->get_native_window()), keycode);
    }

    bool KeyboardEvents::GetKeyPress(int const keycode) {
        int state = glfwGetKey(static_cast<GLFWwindow*>(Application::get().get_window()->get_native_window()), keycode);
        return (state == GLFW_PRESS);
    }
}