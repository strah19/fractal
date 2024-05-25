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

	/**
	* Perspective Camera Controller constructor.
	* 
	* @param const glm::vec2& window_size to calcualte the aspect ratio.
	*/
	PerspectiveCameraController::PerspectiveCameraController(const glm::vec2& window_size, const glm::vec3& cam_pos) {
		aspect_ratio = (float)window_size.x / (float)window_size.y;
		camera = PerspectiveCamera(glm::radians(fov), aspect_ratio);
		
		camera_pos = cam_pos;
		last_mouse_position = { window_size.x / 2, window_size.y / 2 };
		latest_camera_position = camera_pos;
	}

	/**
	* Updates the controllers position and pitch of the camera.
	*/
	void PerspectiveCameraController::Update() {
		if (!freeze) {
			if (latest_camera_position != camera_pos)
				camera.SetPosition(camera_pos);
			camera.SetMatrixView(glm::lookAt(camera_pos, camera_pos + camera_front, camera_up));

			latest_camera_position = camera_pos;

				if (KeyboardEvents::GetKeyPress(GLFW_KEY_D))
				camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * speed;
				if (KeyboardEvents::GetKeyPress(GLFW_KEY_A))
					camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * speed;
				if (KeyboardEvents::GetKeyPress(GLFW_KEY_W))
					camera_pos += speed * camera_front;
				if (KeyboardEvents::GetKeyPress(GLFW_KEY_S))
					camera_pos -= speed * camera_front;

			float xoffset = MousePositionEvent::GetMousePosition().x - last_mouse_position.x;
			float yoffset = last_mouse_position.y - MousePositionEvent::GetMousePosition().y;
			last_mouse_position = { MousePositionEvent::GetMousePosition().x, MousePositionEvent::GetMousePosition().y };

			const float sensitivity = 0.1f;
			xoffset *= sensitivity;
			yoffset *= sensitivity;

			yaw += xoffset;
			pitch += yoffset;

			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;

			glm::vec3 direction;
			direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			direction.y = sin(glm::radians(pitch));
			direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			camera_front = glm::normalize(direction);
		}
	}

	/**
	* Takes the current Ember event and updates keyboard and mouse events.
	* 
	* @param Event& the current Ember event.
	*/
	void PerspectiveCameraController::OnEvent(Event& event) {
		EventDispatcher dispatcher(&event);

		dispatcher.dispatch<KeyboardEvents>(BIND_EVENT(KeyboardHandler));
		dispatcher.dispatch<MouseWheelEvent>(BIND_EVENT(MouseWheelHandler));
		dispatcher.dispatch<ResizeEvent>(BIND_EVENT(WindowResizeHandler));
	}

	/**
	* The mousewheel event handler of the controller.
	* 
	* @param MouseWheelEvent& the event mousewheel.
	*/
	void PerspectiveCameraController::MouseWheelHandler(MouseWheelEvent& mousewheel) {
		fov -= mousewheel.m_yoffset;
		if (fov < SMALLEST_ZOOM)
			fov = SMALLEST_ZOOM;
		if (fov > HIGHEST_ZOOM)
			fov = HIGHEST_ZOOM;
		camera.SetProjection(glm::radians(fov), aspect_ratio);
	}

	/**
	* The Resize event handler.
	* 
	* @param ResizEvent& the resize event.
	*/
	void PerspectiveCameraController::WindowResizeHandler(ResizeEvent& resize) {
		aspect_ratio = (float)resize.m_width / (float)resize.m_height;
		camera.SetProjection(glm::radians(fov), aspect_ratio);
	}

	/**
	* Will freeze the movement of the camera.
	* 
	* @param bool state of the freeze.
	*/
	void PerspectiveCameraController::SetFreeze(bool freeze) {
		this->freeze = freeze;

		if (this->freeze)
			cursor_enable();
		else
			cursor_disable();
	}

	/**
	* The keyboard event handler.
	* 
	* @param KeyboardEvents& keybaord events.
	*/
	void PerspectiveCameraController::KeyboardHandler(KeyboardEvents& keyboard) {
		if (KeyboardEvents::GetKeyPress(GLFW_KEY_M) && !freeze) {
			in_camera_mode = !in_camera_mode;
			last_mouse_position = { MousePositionEvent::GetMousePosition().x, MousePositionEvent::GetMousePosition().y };
			if (!in_camera_mode)
				cursor_enable();
			else
				cursor_disable();
		}
		if (KeyboardEvents::GetKeyPress(GLFW_KEY_F)) {
			SetFreeze(!this->freeze);
		}
	}
}