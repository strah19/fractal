#ifndef ORTHO_CAMERA_CONTROLLER_H
#define ORTHO_CAMERA_CONTROLLER_H

#include "orthocamera.h"
#include "event.h"
#include "window_event.h"
#include "mouse_event.h"

namespace Fractal {
	class OrthoCameraController {
	public:
		OrthoCameraController() = default;
		OrthoCameraController(const glm::vec2& window_size);

		OrthoCamera& get_camera() { return m_camera; }
		void on_event(Event& event);
		void update();
		inline void freeze(bool freeze) { m_freeze = freeze; }
		inline float zoom() const { return m_zoom; }
	private:
		void mouse_wheel_handler(const MouseWheelEvent& mousewheel);
		void resize_handler(const ResizeEvent& resize);
	private:
		OrthoCamera m_camera;

		float m_aspect_ratio = 0.0f;
		float m_zoom = -1.0f;
		glm::vec3 m_camera_pos = { 0, 0, 0 };
		glm::vec2 m_camera_speed = glm::vec2(0.2f, 0.2f);
		bool m_freeze = false;
	};
}

#endif // !ORTHO_CAMERA_CONTROLLER_H
