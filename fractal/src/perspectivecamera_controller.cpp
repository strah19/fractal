/**
 * @file perspectivecamera_controller.cpp
 * @author strah19
 * @date October 3 2021
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
 * This file contains the controller of a perspective camera.
 */

#include "perspectivecamera_controller.h"
#include <GLFW/glfw3.h>
#include "log.h"

namespace Fractal {
	constexpr float SMALLEST_ZOOM = 1.0f;
	constexpr float HIGHEST_ZOOM = 60.0f;

	PerspectiveCameraController::PerspectiveCameraController(const glm::vec2& window_size, const glm::vec3& cam_pos) {
		m_aspect_ratio = (float)window_size.x / (float)window_size.y;
		m_camera = PerspectiveCamera(glm::radians(m_fov), m_aspect_ratio);
		
		m_camera_pos = cam_pos;
		m_last_mouse_position = { window_size.x / 2, window_size.y / 2 };
		m_latest_camera_position = m_camera_pos;
	}

	void PerspectiveCameraController::update() {
		if (!m_freeze) {
			if (m_latest_camera_position != m_camera_pos)
				m_camera.set_position(m_camera_pos);
			m_camera.set_matrix_view(glm::lookAt(m_camera_pos, m_camera_pos + m_camera_front, m_camera_up));

			m_latest_camera_position = m_camera_pos;

				if (KeyboardEvents::GetKeyPress(GLFW_KEY_D))
					m_camera_pos += glm::normalize(glm::cross(m_camera_front, m_camera_up)) * m_speed;
				if (KeyboardEvents::GetKeyPress(GLFW_KEY_A))
					m_camera_pos -= glm::normalize(glm::cross(m_camera_front, m_camera_up)) * m_speed;
				if (KeyboardEvents::GetKeyPress(GLFW_KEY_W))
					m_camera_pos += m_speed * m_camera_front;
				if (KeyboardEvents::GetKeyPress(GLFW_KEY_S))
					m_camera_pos -= m_speed * m_camera_front;

			float xoffset = MousePositionEvent::GetMousePosition().x - m_last_mouse_position.x;
			float yoffset = m_last_mouse_position.y - MousePositionEvent::GetMousePosition().y;
			m_last_mouse_position = { MousePositionEvent::GetMousePosition().x, MousePositionEvent::GetMousePosition().y };

			const float sensitivity = 0.1f;
			xoffset *= sensitivity;
			yoffset *= sensitivity;

			m_yaw += xoffset;
			pitch += yoffset;

			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;

			glm::vec3 direction;
			direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(pitch));
			direction.y = sin(glm::radians(pitch));
			direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(pitch));
			m_camera_front = glm::normalize(direction);
		}
	}

	void PerspectiveCameraController::on_event(Event& event) {
		EventDispatcher dispatcher(&event);

		dispatcher.dispatch<KeyboardEvents>(BIND_EVENT(keyboard_handler));
		dispatcher.dispatch<MouseWheelEvent>(BIND_EVENT(mouse_wheel_handler));
		dispatcher.dispatch<ResizeEvent>(BIND_EVENT(window_resize_handler));
	}

	void PerspectiveCameraController::mouse_wheel_handler(MouseWheelEvent& mousewheel) {
		m_fov -= mousewheel.m_yoffset;
		if (m_fov < SMALLEST_ZOOM)
			m_fov = SMALLEST_ZOOM;
		if (m_fov > HIGHEST_ZOOM)
			m_fov = HIGHEST_ZOOM;
		m_camera.set_projection(glm::radians(m_fov), m_aspect_ratio);
	}

	void PerspectiveCameraController::window_resize_handler(ResizeEvent& resize) {
		m_aspect_ratio = (float)resize.m_width / (float)resize.m_height;
		m_camera.set_projection(glm::radians(m_fov), m_aspect_ratio);
	}

	void PerspectiveCameraController::set_freeze(bool freeze) {
		m_freeze = freeze;

		if (m_freeze)
			cursor_enable();
		else
			cursor_disable();
	}

	void PerspectiveCameraController::keyboard_handler(KeyboardEvents& keyboard) {
		if (KeyboardEvents::GetKeyPress(GLFW_KEY_M) && !m_freeze) {
			m_in_camera_mode = !m_in_camera_mode;
			m_last_mouse_position = { MousePositionEvent::GetMousePosition().x, MousePositionEvent::GetMousePosition().y };
			if (!m_in_camera_mode)
				cursor_enable();
			else
				cursor_disable();
		}
		if (KeyboardEvents::GetKeyPress(GLFW_KEY_F)) {
			set_freeze(!m_freeze);
		}
	}
}