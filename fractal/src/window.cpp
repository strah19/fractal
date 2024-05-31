/**
 * @file utility.cpp
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
 * This file contains creation of different types of windows.
 */

#include "window.h"
#include "glfw_window.h"

namespace Fractal {
    Window::Window(WindowProperties properties, const EventCallbackFn& event_callback) : m_properties(properties), m_event_callback(event_callback) { }

    Window* Window::create_glfw_window(WindowProperties properties, const EventCallbackFn& event_callback) {
        return new GLFWWindow(properties, event_callback);
    }
} 