#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Fractal {
	class Camera {
	public:
		inline glm::mat4 get_projection() const { return m_projection_matrix; }
		inline glm::mat4 get_view() const { return m_view_matrix; }
		inline float get_rotation() const { return m_rotation; }
		inline glm::vec3 get_position() const { return m_position; }

		inline void set_matrix_projection(const glm::mat4& projection) { m_projection_matrix = projection; }
		inline void set_matrix_view(const glm::mat4& view) { m_view_matrix = view; }
		inline void set_rotation(float r) { m_rotation = r; calculate_view(); }
		inline void set_position(const glm::vec3& pos) { m_position = pos; calculate_view(); }
	protected:
		void calculate_view();
	protected:
		glm::vec3 m_position = { 0, 0, 3 };

		glm::mat4 m_projection_matrix = glm::mat4(1.0f);
		glm::mat4 m_view_matrix = glm::mat4(1.0f);

		float m_rotation = 0.0f;
	};
}

#endif // !CAMERA_H
