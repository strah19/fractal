#ifndef PERSPECTIVE_CAMERA_CONTROLLER_H
#define PERSPECTIVE_CAMERA_CONTROLLER_H

#include "perspectivecamera.h"
#include "event.h"
#include "mouse_event.h"
#include "window_event.h"
#include "keyboard_event.h"

namespace Fractal {	
	class PerspectiveCameraController {
	public:
		PerspectiveCameraController() = default;
		PerspectiveCameraController(const glm::vec2& window_size, const glm::vec3& cam_pos);
		void on_event(Event& event);
		void update();

		void set_freeze(bool freeze);
		inline void set_camera_speed(float speed) { m_speed = speed; }

		inline glm::vec3 get_front_camera() const { return m_camera_front; }
		inline PerspectiveCamera& get_camera() { return m_camera; }

		inline const bool get_mode() const { return m_in_camera_mode; }
		inline const bool get_freeze_state() const { return m_freeze; }
	private:
		void mouse_wheel_handler(MouseWheelEvent& mousewheel);
		void keyboard_handler(KeyboardEvents& keyboard);
		void window_resize_handler(ResizeEvent& resize);
	private:
		float m_aspect_ratio = 0.0f;
		PerspectiveCamera m_camera;
		float m_fov = 60.0f;

		glm::vec3 m_camera_pos = glm::vec3(0, 0, 3 );
		glm::vec3 m_camera_target = glm::vec3(0, 0, 0);

		float m_yaw = -90.0f, pitch = 0.0f;
		glm::vec2 m_last_mouse_position = { 0, 0 };
		glm::vec3 m_latest_camera_position = { 0, 0, 0 };

		glm::vec3 m_camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
		float m_speed = 0.2f;

		bool m_in_camera_mode = false;
		bool m_freeze = false;
	};
}

#endif // !PERSPECTIVE_CAMERA_CONTROLLER_H
