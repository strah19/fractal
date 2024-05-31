/**
 * @file vertex_array.cpp
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
 * This file contains vertex array code.
 */

#include "vertex_array.h"
#include <glad/glad.h>

namespace Fractal {
	static uint32_t current_vertex_array_id = 0;

	static GLenum VertexShaderTypeToOpenGL(VertexShaderType type) {
		switch (type) {
		case VertexShaderType::Float: return GL_FLOAT;
		case VertexShaderType::Int: return GL_INT;
		case VertexShaderType::None: return GL_NONE;
		}
		return GL_NONE;
	}

	VertexArray::VertexArray() {
		glGenVertexArrays(1, &m_vertex_array_buffer_id);
	}

	VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &m_vertex_array_buffer_id);
	}

	void VertexArray::bind() {
		if (current_vertex_array_id != m_vertex_array_buffer_id) {
			glBindVertexArray(m_vertex_array_buffer_id);
			current_vertex_array_id = m_vertex_array_buffer_id;
		}
	}

	void VertexArray::unbind() {
		glBindVertexArray(0);
		current_vertex_array_id = 0;
	}

	void VertexArray::add_vertex_buffer(VertexBuffer* vertex_buf, VertexBufferFormat format) {
		uint32_t stride = vertex_buf->get_layout()->calculate();

		vertex_buf->bind();
		bind();

		for (auto& elements : vertex_buf->get_layout()->get_layout()) {
			switch (format) {
			case VertexBufferFormat::VNCVNCVNC:
				glVertexAttribPointer(elements.index, elements.size, VertexShaderTypeToOpenGL(elements.type), elements.normalized ? GL_TRUE : GL_FALSE,
					stride * get_size_in_bytes(elements.type),
					(void*)(elements.offset * get_size_in_bytes(elements.type)));
				break;
			case VertexBufferFormat::VVVCCCNNN:
				glVertexAttribPointer(elements.index, elements.size, VertexShaderTypeToOpenGL(elements.type), elements.normalized ? GL_TRUE : GL_FALSE,
					0,
					(void*)(elements.offset * get_size_in_bytes(elements.type)));
				break;
			}

			enable_vertex_attrib(elements.index);
		}
	}

	void VertexArray::enable_vertex_attrib(uint32_t index) {
		glEnableVertexAttribArray(index);
	}

	void VertexArray::set_array_for_instancing(std::shared_ptr<VertexBuffer>& vertex_buf, uint32_t offset_sizes[], uint32_t stride_sizes[]) {
		vertex_buf->bind();
		bind();
		uint32_t i = 0;
		for (auto& elements : vertex_buf->get_layout()->get_layout()) {
			glVertexAttribPointer(elements.index, elements.size, VertexShaderTypeToOpenGL(elements.type), elements.normalized ? GL_TRUE : GL_FALSE,
				stride_sizes[i],
				(void*)offset_sizes[i]);
			i++;
			enable_vertex_attrib(elements.index);
		}
	}
}