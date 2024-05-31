/**
 * @file camera.cpp
 * @author strah19
 * @date May 29 2024
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
 * This file contains the base camera class.
 */

#include "camera.h"

namespace Fractal {
	void Camera::calculate_view() {
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0, 0, 1));

		m_view_matrix = glm::inverse(transform);
	}
}