/**
 * @file PerspectiveCamera.cpp
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
 * This file contains the matrix transformations of a perspective camera.
 */

#include "perspectivecamera.h"

namespace Fractal {
	PerspectiveCamera::PerspectiveCamera(float fov, float aspect_ratio) {
		set_projection(fov, aspect_ratio);
	}

	void PerspectiveCamera::set_projection(float fov, float aspect_ratio) {
		calculate_view();
		m_projection_matrix = glm::perspective(fov, aspect_ratio, 0.1f, 100.0f);
	}
}