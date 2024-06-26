#ifndef PERSPECTIVE_CAMERA_H
#define PERSPECTIVE_CAMERA_H

#include "camera.h"

namespace Fractal {
	class PerspectiveCamera : public Camera {
	public:
		PerspectiveCamera(float fov, float aspect_ratio);
		PerspectiveCamera() = default;

		void set_projection(float fov, float aspect_ratio);
	};
}

#endif // !PERSPECTIVE_CAMERA_H
