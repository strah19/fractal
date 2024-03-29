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

		OrthoCamera& GetCamera() { return camera; }
		void OnEvent(Event& event);
		void Update();
		inline void Freeze(bool freeze) { this->freeze = freeze; }
		inline float Zoom() const { return zoom; }
	private:
		void MouseWheelHandler(const MouseWheelEvent& mousewheel);
		void ResizeHandler(const ResizeEvent& resize);
	private:
		OrthoCamera camera;

		float aspect_ratio = 0.0f;
		float zoom = -1.0f;
		glm::vec3 camera_pos = { 0, 0, 0 };
		glm::vec2 camera_speed = glm::vec2(0.2f, 0.2f);
		bool freeze = false;
	};
}

#endif // !ORTHO_CAMERA_CONTROLLER_H
