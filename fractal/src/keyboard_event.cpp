/**
 * @file keyboard_event.cpp
 * @author strah19
 * @date May 30 2024
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the MIT License as
 * published by the Free Software Foundation.
 *
 * @section DESCRIPTION
 *
 * This file contains code that connects keyboard control to user.
 */

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