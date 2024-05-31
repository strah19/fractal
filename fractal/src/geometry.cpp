/**
 * @file file.cpp
 * @author strah19
 * @date May 30 2024
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
 * This file contains code that sends basic shape data to the renderer.
 */

#include "geometry.h"
#include "log.h"

namespace Fractal {
	using namespace Geometry;

	Mesh Geometry::create_geometry(const glm::mat4& matrix, const glm::vec4& color, float texture_id, const glm::vec2 tex_coords[], uint32_t vertex_count, const glm::vec4 positions[]) {
		Mesh mesh;
		for (size_t i = 0; i < vertex_count; i++) {
			Vertex vertex;
			vertex.position = matrix * positions[i];
			vertex.color = color;
			vertex.texture_coordinates = tex_coords[i];
			vertex.texture_id = texture_id;
			vertex.material_id = (float)0;

			mesh.vertices.push_back(vertex);
		}

		return mesh;
	}

	glm::mat4 Geometry::get_model_matrix(const glm::vec3& position, const glm::vec3& scalar) {
		return glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z }) * glm::scale(glm::mat4(1.0f), { scalar.x, scalar.y, scalar.z });
	}

	glm::mat4 Geometry::get_rotated_model_matrix(const glm::vec3& position, const glm::vec3& scalar, const glm::vec3& rotation_orientation, float degree) {
		glm::mat4 trans = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z });
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), { scalar.x, scalar.y, scalar.z });
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(degree), rotation_orientation);

		return (trans * rotate * scale);
	}

	static uint32_t index_offset = 0;

	uint32_t Geometry::add_indice(uint32_t indices, uint32_t offset) {
		return offset + indices;
	}

	void Geometry::update_index_offset(uint32_t* indices, uint32_t count) {
		*indices += count;
	}

	static RendererFrame* renderer;
	void set_renderer(RendererFrame* ren) {
		renderer = ren;
	}

	void Quad::draw_quad(const glm::vec3& position, const glm::vec2& scalar, const glm::vec4& color) {
		glm::mat4 model = (renderer->get_flags() & RenderFlags::TopLeft) ? get_model_matrix({ position.x + (scalar.x / 2), position.y + (scalar.y / 2), position.z },
			glm::vec3(scalar.x, scalar.y, 1.0f)) : get_model_matrix(position, { scalar.x, scalar.y, 1.0f });
		Mesh m = create_geometry(model, color, -1.0f, TEX_COORDS, QUAD_VERTEX_COUNT, QUAD_POSITIONS);

		add_indices(m);
		renderer->submit(m);
	}

	void Quad::draw_quad(const glm::vec3& position, const glm::vec2& scalar, uint32_t texture, const glm::vec4& color) {
		glm::mat4 model = (renderer->get_flags() & RenderFlags::TopLeft) ? get_model_matrix({ position.x + (scalar.x / 2), position.y + (scalar.y / 2), position.z },
			glm::vec3(scalar.x, scalar.y, 1.0f)) : get_model_matrix(position, { scalar.x, scalar.y, 1.0f });
		Mesh m = create_geometry(model, color, renderer->get_graphics_device()->calculate_texture_index(texture), TEX_COORDS, QUAD_VERTEX_COUNT, QUAD_POSITIONS);

		add_indices(m);
		renderer->submit(m);
	}

	void Quad::draw_quad(const glm::vec3& position, float degree, const glm::vec3& orientation, const glm::vec2& scalar, const glm::vec4& color) {
		glm::mat4 model = (renderer->get_flags() & RenderFlags::TopLeft) ? get_rotated_model_matrix({ position.x + (scalar.x / 2), position.y + (scalar.y / 2), position.z },
			glm::vec3(scalar.x, scalar.y, 1.0f), orientation, degree) : get_rotated_model_matrix(position, { scalar.x, scalar.y, 1.0f }, orientation, degree);
		Mesh m = create_geometry(model, color, -1.0f, TEX_COORDS, QUAD_VERTEX_COUNT, QUAD_POSITIONS);

		add_indices(m);
		renderer->submit(m);
	}

	void Quad::draw_quad(const QuadModel& model) {
		glm::mat4 mat = (renderer->get_flags() & RenderFlags::TopLeft) ? get_rotated_model_matrix({ model.position.x + (model.scalar.x / 2), model.position.y + (model.scalar.y / 2), model.position.z },
			model.scalar, model.orientation, model.degree) :
			get_rotated_model_matrix(model.position, model.scalar, model.orientation, model.degree);
		Mesh m = create_geometry(mat, model.color, model.texture_id, model.tex_coords, QUAD_VERTEX_COUNT, QUAD_POSITIONS);

		add_indices(m);
		renderer->submit(m);
	}

	void Quad::add_indices(Mesh& mesh) {
		if (renderer->get_graphics_device()->empty())
			index_offset = 0;

		mesh.indices.push_back(add_indice(index_offset, 0));
		mesh.indices.push_back(add_indice(index_offset, 1));
		mesh.indices.push_back(add_indice(index_offset, 2));
		mesh.indices.push_back(add_indice(index_offset, 2));
		mesh.indices.push_back(add_indice(index_offset, 3));
		mesh.indices.push_back(add_indice(index_offset, 0));

		update_index_offset(&index_offset, 4);
	}

	void Cube::draw_cube(const glm::vec3& position, const glm::vec3& scalar, const glm::vec4& color) {
		glm::mat4 model = (renderer->get_flags() & RenderFlags::TopLeft) ? get_model_matrix({ position.x + (scalar.x / 2), position.y + (scalar.y / 2), position.z + (scalar.z / 2) },
			glm::vec3(scalar.x, scalar.y, scalar.z)) : get_model_matrix(position, { scalar.x, scalar.y, scalar.z });
		Mesh m = create_geometry(model, color, -1.0f, CUBE_TEX_COORDS, CUBE_VERTEX_COUNT, CUBE_POSITIONS);

		add_indices(m);
		renderer->submit(m);
	}

	void Cube::add_indices(Mesh& mesh) {
		if (renderer->get_graphics_device()->empty())
			index_offset = 0;

		for (int i = 0; i < CUBE_INDICES_COUNT; i++) {
			mesh.indices.push_back(add_indice(index_offset, cube_indices[i]));
		}

		update_index_offset(&index_offset, 8);
	}
}