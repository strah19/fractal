/**
 * @file mouse_event.cpp
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
 * This file contains code for controlling the mouse.
 */

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