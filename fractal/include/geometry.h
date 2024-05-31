#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "renderer.h"

namespace Fractal {
	namespace Geometry {
		Mesh create_geometry(const glm::mat4& matrix, const glm::vec4& color, float texture_id, const glm::vec2 tex_coords[], uint32_t vertex_count, const glm::vec4 positions[]);

		glm::mat4 get_model_matrix(const glm::vec3& position, const glm::vec3& scalar);
		glm::mat4 get_rotated_model_matrix(const glm::vec3& position, const glm::vec3& scalar, const glm::vec3& rotation_orientation, float degree);

		void update_index_offset(uint32_t* indices, uint32_t count);
		uint32_t add_indice(uint32_t indices, uint32_t offset);
	};

	struct QuadModel {
		glm::vec3 position = { 0, 0, 0 };
		glm::vec3 scalar = { 0, 0, 0 };
		float texture_id = -1;
		glm::vec2* tex_coords = nullptr;
		glm::vec4 color = { 0, 0, 0, 0 };
		glm::vec3 orientation = { 0, 0, 0 };
		float degree = 0;
	};

	void set_renderer(RendererFrame* renderer);

	class Quad {
	public:
		static void draw_quad(const glm::vec3& position, const glm::vec2& scalar, const glm::vec4& color);
		static void draw_quad(const glm::vec3& position, const glm::vec2& scalar, uint32_t texture, const glm::vec4& color = { -1, -1, -1, -1 });
		static void draw_quad(const glm::vec3& position, float degree, const glm::vec3& orientation, const glm::vec2& scalar, const glm::vec4& color);
		static void draw_quad(const QuadModel& model);

		static void add_indices(Mesh& mesh);
	};

	class Cube {
	public:
		static void draw_cube(const glm::vec3& position, const glm::vec3& scalar, const glm::vec4& color);

		static void add_indices(Mesh& mesh);
	};
}

#endif // !GEOMETRY_H