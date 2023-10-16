

#include "orthocamera_controller.h"
#include "keyboard_event.h"
#include <GLFW/glfw3.h>

namespace Fractal {
	constexpr float ZOOM_SPEED = 3.0f;

	/**
	* OrthoCameraController Constructor that creates an ortho camera.
	* 
	* @param const glm::vec2& window size to define the aspect ratio.
	*/
	OrthoCameraController::OrthoCameraController(const glm::vec2& window_size) {
		aspect_ratio = (float) window_size.x / (float) window_size.y;
		camera = OrthoCamera(aspect_ratio * zoom, -aspect_ratio * zoom, zoom, -zoom);
	}

	/**
	* Updates the cameras data.
	*/
	void OrthoCameraController::Update() {
		if (!freeze) {
			camera.SetPosition(camera_pos);
			if (KeyboardEvents::GetKeyPress(GLFW_KEY_D))
				camera_pos.x += camera_speed.x;
			if (KeyboardEvents::GetKeyPress(GLFW_KEY_A))
				camera_pos.x -= camera_speed.x;
			if (KeyboardEvents::GetKeyPress(GLFW_KEY_W))
				camera_pos.y += camera_speed.y;
			if (KeyboardEvents::GetKeyPress(GLFW_KEY_S))
				camera_pos.y -= camera_speed.y;
		}
	}

	/**
	* Updates the controllers events.
	* 
	* @param Event& current Ember event from the dispatcher.
	*/
	void OrthoCameraController::OnEvent(Event& event) {
		EventDispatcher dispatcher(&event);
		if (!freeze) 
			dispatcher.dispatch<MouseWheelEvent>(BIND_EVENT(MouseWheelHandler));
		dispatcher.dispatch<ResizeEvent>(BIND_EVENT(ResizeHandler));
	}

	/**
	* Mousewheel event handler.
	* 
	* @param const MouseWheelEvent& mousewheel event.
	*/
	void OrthoCameraController::MouseWheelHandler(const MouseWheelEvent& mousewheel) {
		zoom += mousewheel.m_yoffset * ZOOM_SPEED;
		camera.SetProjection(aspect_ratio * zoom, -aspect_ratio * zoom, zoom, -zoom);
	}

	/**
	* ResizeEvent event handler.
	*
	* @param const ResizeEvent& resize event.
	*/
	void OrthoCameraController::ResizeHandler(const ResizeEvent& resize) {
		aspect_ratio = (float) resize.m_width / (float) resize.m_height;
		camera.SetProjection(aspect_ratio * zoom, -aspect_ratio * zoom, zoom, -zoom);
	}
}