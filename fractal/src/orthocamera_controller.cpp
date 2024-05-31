/**
 * @file orthocamera_controller.cpp
 * @author strah19
 * @date May 29 2024
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
 * This file contains the orthocamera controller class.
 */

#include "orthocamera_controller.h"
#include "keyboard_event.h"
#include <GLFW/glfw3.h>

namespace Fractal {
	constexpr float ZOOM_SPEED = 3.0f;

	OrthoCameraController::OrthoCameraController(const glm::vec2& window_size) {
		m_aspect_ratio = (float) window_size.x / (float) window_size.y;
		m_camera = OrthoCamera(m_aspect_ratio * m_zoom, -m_aspect_ratio * m_zoom, m_zoom, -m_zoom);
	}

	void OrthoCameraController::update() {
		if (!m_freeze) {
			m_camera.set_position(m_camera_pos);
			if (KeyboardEvents::GetKeyPress(GLFW_KEY_D))
				m_camera_pos.x += m_camera_speed.x;
			if (KeyboardEvents::GetKeyPress(GLFW_KEY_A))
				m_camera_pos.x -= m_camera_speed.x;
			if (KeyboardEvents::GetKeyPress(GLFW_KEY_W))
				m_camera_pos.y += m_camera_speed.y;
			if (KeyboardEvents::GetKeyPress(GLFW_KEY_S))
				m_camera_pos.y -= m_camera_speed.y;
		}
	}

	void OrthoCameraController::on_event(Event& event) {
		EventDispatcher dispatcher(&event);
		if (!m_freeze) 
			dispatcher.dispatch<MouseWheelEvent>(BIND_EVENT(mouse_wheel_handler));
		dispatcher.dispatch<ResizeEvent>(BIND_EVENT(resize_handler));
	}

	void OrthoCameraController::mouse_wheel_handler(const MouseWheelEvent& mousewheel) {
		m_zoom += mousewheel.m_yoffset * ZOOM_SPEED;
		m_camera.set_projection(m_aspect_ratio * m_zoom, -m_aspect_ratio * m_zoom, m_zoom, -m_zoom);
	}

	void OrthoCameraController::resize_handler(const ResizeEvent& resize) {
		m_aspect_ratio = (float) resize.m_width / (float) resize.m_height;
		m_camera.set_projection(m_aspect_ratio * m_zoom, -m_aspect_ratio * m_zoom, m_zoom, -m_zoom);
	}
}