/**
 * @file orthocamera.cpp
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
 * This file contains the ortho camera object.
 */

#include "orthocamera.h"

namespace Fractal {
	OrthoCamera::OrthoCamera(float left, float right, float bottom, float top) {
		set_projection(left, right, bottom, top);
	}

	void OrthoCamera::set_projection(float left, float right, float bottom, float top) {
		m_projection_matrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_left = left;
		m_right = right;
		m_bottom = bottom;
		m_top = top;
		calculate_view();
	}
}