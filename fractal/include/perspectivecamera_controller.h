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
		PerspectiveCameraController(const glm::vec2& window_size);
		void OnEvent(Event& event);
		void Update();
		void SetFreeze(bool freeze);
		inline void SetCameraSpeed(float speed) { this->speed = speed; }

		inline glm::vec3 GetFrontCamera() const { return camera_front; }
		inline PerspectiveCamera& GetCamera() { return camera; }

		inline const bool get_mode() const { return in_camera_mode; }
		inline const bool get_freeze_state() const { return freeze; }
	private:
		void MouseWheelHandler(MouseWheelEvent& mousewheel);
		void KeyboardHandler(KeyboardEvents& keyboard);
		void WindowResizeHandler(ResizeEvent& resize);
	private:
		float aspect_ratio = 0.0f;
		PerspectiveCamera camera;
		float fov = 60.0f;

		glm::vec3 camera_pos = glm::vec3(0, 0, 3 );
		glm::vec3 camera_target = glm::vec3(0, 0, 0);

		float yaw = -90.0f, pitch = 0.0f;
		glm::vec2 last_mouse_position = { 0, 0 };
		glm::vec3 latest_camera_position = { 0, 0, 0 };

		glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
		float speed = 0.2f;

		bool in_camera_mode = false;
		bool freeze = false;
	};
}

#endif // !PERSPECTIVE_CAMERA_CONTROLLER_H
