#ifndef ORTHO_CAMERA_H
#define ORTHO_CAMERA_H

#include "camera.h"

namespace Fractal {
	class OrthoCamera : public Camera {
	public:
		OrthoCamera(float left, float right, float bottom, float top);
		OrthoCamera() = default;

		void set_projection(float left, float right, float bottom, float top);

		inline float left() const { return m_left; }
		inline float right() const { return m_right; }
		inline float bottom() const { return m_bottom; }
		inline float top() const { return m_top; }
	private:
		float m_left = 0, m_right = 0, m_bottom = 0, m_top = 0;
	};
}

#endif // !ORTHO_CAMERA_H
