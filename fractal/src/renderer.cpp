/**
 * @file renderer.cpp
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
 * This file contains code the main rendering code.
 */

#include "renderer.h"
#include "log.h"
#include "renderer_commands.h"
#include <gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <cstring>

namespace Fractal {
	void DeviceStatistics::reset() {
		num_of_indices = 0;
		num_of_vertices = 0;
		draw_count = 0;
	}

	template <typename V>
	GraphicsDevice<V>::GraphicsDevice(uint32_t max_vertex_count, uint32_t max_index_count) {
		m_vbo = new VertexBuffer(sizeof(V) * max_vertex_count);
		m_vao = new VertexArray();
		m_ds.max_vertex_count = max_vertex_count;

		m_vert_base = new V[max_vertex_count];
		m_indx_base = new uint32_t[max_vertex_count];

		m_ibo = new IndexBuffer(sizeof(uint32_t) * max_index_count);
		m_ds.max_index_count = max_index_count;

		m_vao->set_index_buffer_size(m_ibo->get_count());
	}

	template <typename V>
	GraphicsDevice<V>::~GraphicsDevice() {
		delete m_vao;
		delete m_vbo;
		delete m_ibo;

		delete[] m_vert_base;
		delete[] m_indx_base;

		m_shader = nullptr;
	}

	BatchGraphicsDevice::BatchGraphicsDevice(uint32_t max_vertex_count, uint32_t max_index_count) : GraphicsDevice(max_vertex_count, max_index_count) {
		VertexBufferLayout layout;
		layout.add_to_buffer(VertexBufferElement(3, false, VertexShaderType::Float));
		layout.add_to_buffer(VertexBufferElement(4, false, VertexShaderType::Float));
		layout.add_to_buffer(VertexBufferElement(2, false, VertexShaderType::Float));
		layout.add_to_buffer(VertexBufferElement(2, false, VertexShaderType::Float));

		m_vbo->set_layout(layout);
		m_vao->add_vertex_buffer(m_vbo, VertexBufferFormat::VNCVNCVNC);
	}

	void BatchGraphicsDevice::init() {
		m_idb = new IndirectDrawBuffer(sizeof(m_commands));
	}

	BatchGraphicsDevice::~BatchGraphicsDevice() {
		delete m_idb;
	}

	void BatchGraphicsDevice::setup() {
		memset(m_textures, NULL, sizeof(uint32_t) * MAX_TEXTURE_SLOTS);
		m_texture_slot_index = 0;
		m_ds.reset();

		m_index_offset = 0;
		m_cmd_vertex_base = 0;
		m_current_draw_command_vertex_size = 0;

		m_vert_ptr = m_vert_base;
		m_indx_ptr = m_indx_base;
	}

	void BatchGraphicsDevice::add_vertex(Vertex* v) {
		*m_vert_ptr = *v;
		m_vert_ptr++;
		m_ds.num_of_vertices++;
	}

	void BatchGraphicsDevice::add_index(uint32_t index) {
		*m_indx_ptr = index;
		m_indx_ptr++;
		m_ds.num_of_indices++;
	}

	bool BatchGraphicsDevice::submit(Mesh& mesh) {
		if (m_ds.num_of_vertices + mesh.vertices.size() > m_ds.max_vertex_count || m_ds.num_of_indices + mesh.indices.size() > m_ds.max_index_count)
			return false;

		for (auto& vertex : mesh.vertices) {
			if (m_ds.num_of_vertices >= m_ds.max_vertex_count)
				break;
			add_vertex(&vertex);
			m_index_offset++;
		}

		for (auto& index : mesh.indices) {
			if (m_ds.num_of_indices >= m_ds.max_index_count)
				break;
			add_index(index);
			m_current_draw_command_vertex_size++;
		}

		return true;
	}

	void BatchGraphicsDevice::render() {
		m_vao->bind();
		m_ibo->bind();
		m_vbo->bind();
		(*m_shader)->bind();

		m_idb->bind();
		m_idb->set_data(m_commands, sizeof(m_commands), 0);

		for (uint32_t i = 0; i < m_texture_slot_index; i++)
			if (m_textures[i])
				glBindTextureUnit(i, m_textures[i]);
		uint32_t vertex_buf_size = (uint32_t)((uint8_t*)m_vert_ptr - (uint8_t*)m_vert_base);
		uint32_t index_buf_size = (uint32_t)((uint8_t*)m_indx_ptr - (uint8_t*)m_indx_base);

		m_vbo->set_data(m_vert_base, vertex_buf_size);
		m_ibo->set_data(m_indx_base, index_buf_size);

		m_vao->set_index_buffer_size(m_ibo->get_count());

		RendererCommands::draw_multi_indirect(nullptr, m_ds.draw_count + 1, 0);
	}

	void BatchGraphicsDevice::next_command() {
		m_ds.draw_count++;
		m_cmd_vertex_base += m_ds.num_of_vertices;
		m_current_draw_command_vertex_size = 0;
	}

	void BatchGraphicsDevice::make_command() {
		m_commands[m_ds.draw_count].vertex_count = m_current_draw_command_vertex_size;
		m_commands[m_ds.draw_count].instance_count = 1;
		m_commands[m_ds.draw_count].first_index = 0;
		m_commands[m_ds.draw_count].base_vertex = m_cmd_vertex_base;
		m_commands[m_ds.draw_count].base_instance = m_ds.draw_count;
	}

	float BatchGraphicsDevice::calculate_texture_index(uint32_t id) {
		float texture_id = -1.0f;

		for (uint32_t i = 0; i < m_texture_slot_index; i++)
			if (m_textures[i] == id)
				texture_id = (float)i;

		if (texture_id == -1.0f) {
			m_textures[m_texture_slot_index] = id;
			texture_id = (float)m_texture_slot_index;
			m_texture_slot_index++;

			if (m_texture_slot_index == MAX_TEXTURE_SLOTS) {
				FRACTAL_LOG_ERROR("Too many textures in one batch. Create a new draw call!");
				return -1.0f;
			}
		}

		return texture_id;
	}

	RendererFrame::~RendererFrame() {
		m_camera = nullptr;
	}

	Renderer::Renderer() {
		m_default_shader.init("resources/shaders/default_shader.glsl");
		init_renderer_shader(&m_default_shader);
		m_current_shader = &m_default_shader;

		m_gd = new BatchGraphicsDevice(MAX_VERTEX_COUNT, MAX_INDEX_COUNT);
		m_gd->init();
		m_ssbo = new ShaderStorageBuffer(sizeof(glm::mat4), 0);
	}

	void Renderer::init_renderer_shader(Shader* shader) {
		shader->bind();
		int sampler[MAX_TEXTURE_SLOTS];
		for (int i = 0; i < MAX_TEXTURE_SLOTS; i++)
			sampler[i] = i;
		shader->set_int_array("textures", sampler, MAX_TEXTURE_SLOTS);
		shader->unbind();
	}

	void Renderer::begin_scene(Camera* camera) {
		m_camera = camera;
		m_proj_view = camera->get_projection() * camera->get_view();
		m_current_shader = &m_default_shader;
		m_gd->setup();
	}

	void Renderer::end_scene() {
		m_gd->set_shader(&m_current_shader);

		m_gd->make_command();
		m_gd->next_command();
		m_ssbo->bind();
		m_ssbo->set_data((void*)&m_proj_view, sizeof(glm::mat4), 0);
		m_ssbo->bind_to_bind_point();
		m_gd->render();
	}

	void Renderer::submit(Mesh& mesh) {
		if (!m_gd->submit(mesh)) {
			end_scene();
			m_gd->setup();
			if (!m_gd->submit(mesh))
				FRACTAL_LOG_ERROR("Singular mesh is too big. Split it up!");
		}
	}
}