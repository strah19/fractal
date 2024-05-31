/**
 * @file buffer.cpp
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
 * This file contains all the OpenGL buffer types.
 */

#include "buffer.h"
#include <glad/glad.h>

namespace Fractal {
	static uint32_t current_index_buffer_id = 0;
	static uint32_t current_vertex_buffer_id = 0;
	static uint32_t current_uniform_buffer_id = 0;

	VertexBuffer::VertexBuffer(float* vertices, uint32_t size) {
		glGenBuffers(1, &m_vertex_buffer_id);
		bind();
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	VertexBuffer::VertexBuffer(uint32_t size) {
		glGenBuffers(1, &m_vertex_buffer_id);
		bind();
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(1, &m_vertex_buffer_id);
	}

	void VertexBuffer::bind() {
		if (current_vertex_buffer_id != m_vertex_buffer_id) {
			glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_id);
			current_index_buffer_id = m_vertex_buffer_id;
		}
	}

	void VertexBuffer::unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		current_index_buffer_id = 0;
	}

	void VertexBuffer::set_data(void* data, uint32_t size, uint32_t offset) {
		bind();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}

	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t size) {
		glGenBuffers(1, &m_index_buffer_id);
		bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
		m_count = size / sizeof(*indices);
	}

	IndexBuffer::IndexBuffer(uint32_t size) {
		glGenBuffers(1, &m_index_buffer_id);
		bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		m_count = 0;
	}

	void IndexBuffer::set_data(uint32_t* data, uint32_t size) {
		bind();
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
		m_count = size / sizeof(*data);
	}

	IndexBuffer::~IndexBuffer() {
		glDeleteBuffers(1, &m_index_buffer_id);
	}

	void IndexBuffer::bind() {
		if (current_index_buffer_id != m_index_buffer_id) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer_id);
			current_index_buffer_id = m_index_buffer_id;
		}
	}

	void IndexBuffer::unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		current_index_buffer_id = 0;
	}

	UniformBuffer::UniformBuffer(uint32_t size, uint32_t bindpoint) {
		glGenBuffers(1, &m_uniform_buffer_id);
		m_uniform_buffer_point = bindpoint;
		bind();
		allocate_data(size);
		m_size_of_buffer = size;
	}

	UniformBuffer::~UniformBuffer() {
		glDeleteBuffers(1, &m_uniform_buffer_id);
	}

	void UniformBuffer::bind() {
		if (current_uniform_buffer_id != m_uniform_buffer_id) {
			glBindBuffer(GL_UNIFORM_BUFFER, m_uniform_buffer_id);
			current_uniform_buffer_id = m_uniform_buffer_id;
		}
	}

	void UniformBuffer::unbind() {
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	uint32_t UniformBuffer::get_id() const {
		return m_uniform_buffer_id;
	}

	void UniformBuffer::set_data(void* data, uint32_t size, uint32_t offset) {
		bind();
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	}

	uint32_t UniformBuffer::get_uniform_block_id(uint32_t shader_id, const std::string& block_name) {
		return glGetUniformBlockIndex(shader_id, block_name.c_str());
	}

	void UniformBuffer::bind_to_shader(uint32_t shader_id, const std::string& block_name) {
		glUniformBlockBinding(shader_id, get_uniform_block_id(shader_id, block_name), m_uniform_buffer_point);
		bind_to_bind_point();
	}

	void UniformBuffer::bind_to_bind_point() {
		glBindBufferRange(GL_UNIFORM_BUFFER, m_uniform_buffer_point, m_uniform_buffer_id, 0, m_size_of_buffer);
	}

	void UniformBuffer::allocate_data(uint32_t size) {
		glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	static uint32_t current_indirect_draw_buffer = 0;
	IndirectDrawBuffer::IndirectDrawBuffer(uint32_t size) {
		glGenBuffers(1, &m_indirect_buffer_id);
		bind();
		allocate_data(size, nullptr);
		m_size_of_buffer = size;
	}

	IndirectDrawBuffer::~IndirectDrawBuffer() {
		glDeleteBuffers(1, &m_indirect_buffer_id);
	}

	void IndirectDrawBuffer::bind() {
		if (current_indirect_draw_buffer != m_indirect_buffer_id) {
			glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_indirect_buffer_id);
			current_indirect_draw_buffer = m_indirect_buffer_id;
		}
	}

	void IndirectDrawBuffer::unbind() {
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
	}

	uint32_t IndirectDrawBuffer::get_id() const {
		return m_indirect_buffer_id;
	}

	void IndirectDrawBuffer::set_data(void* data, uint32_t size, uint32_t offset) {
		bind();
		glBufferSubData(GL_DRAW_INDIRECT_BUFFER, offset, size, data);
	}

	void IndirectDrawBuffer::allocate_data(uint32_t size, void* data) {
		glBufferData(GL_DRAW_INDIRECT_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}

	static uint32_t current_shader_storage_id = 0;
	ShaderStorageBuffer::ShaderStorageBuffer(uint32_t size, uint32_t bindpoint) {
		glGenBuffers(1, &m_shader_storage_id);
		bind();
		allocate_data(size, nullptr);
		m_size_of_buffer = size;
		m_binding_point = bindpoint;
	}

	ShaderStorageBuffer::~ShaderStorageBuffer() {
		glDeleteBuffers(1, &m_shader_storage_id);
	}

	void ShaderStorageBuffer::bind() {
		if (current_shader_storage_id != m_shader_storage_id) {
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_shader_storage_id);
			current_shader_storage_id = m_shader_storage_id;
		}
	}

	void ShaderStorageBuffer::unbind() {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	uint32_t ShaderStorageBuffer::get_id() const {
		return m_shader_storage_id;
	}

	void ShaderStorageBuffer::set_data(void* data, uint32_t size, uint32_t offset) {
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data);
	}

	void ShaderStorageBuffer::allocate_data(uint32_t size, void* data) {
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_DRAW); 
	}

	uint32_t ShaderStorageBuffer::get_uniform_block_id(uint32_t shader_id, const std::string& block_name) {
		return glGetUniformBlockIndex(shader_id, block_name.c_str());
	}

	void ShaderStorageBuffer::bind_to_shader(uint32_t shader_id, const std::string& block_name) {
		glShaderStorageBlockBinding(shader_id, get_uniform_block_id(shader_id, block_name), m_binding_point);
		bind_to_bind_point();
	}

	void ShaderStorageBuffer::bind_to_bind_point() {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_binding_point, m_shader_storage_id);
	}
}