/**
 * @file renderer_commands.cpp
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
 * This file contains code that interfaces with OpenGL.
 */

#include "renderer_commands.h"
#include <glad/glad.h>

namespace Fractal {
	static int prim;

    void RendererCommands::initialize() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glEnable(GL_MULTISAMPLE);
        
		prim = TRIANGLE;
    }

    void RendererCommands::clear(float r, float g, float b, float a) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(r, g, b, a);
    }

    void RendererCommands::set_viewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
        glViewport(x, y, w, h);
    }

	void RendererCommands::draw_vertex_array(VertexArray* vertex_array) {
		glDrawElements(decode_type(), vertex_array->get_index_buffer_size(), GL_UNSIGNED_INT, 0);
	}

	void RendererCommands::draw_vertex_array_instanced(VertexArray* vertex_array, uint32_t instance_count) {
		glDrawArraysInstanced(decode_type(), 0, vertex_array->get_index_buffer_size(), instance_count);
	}

	void RendererCommands::draw_multi_indirect(const void* indirect, uint32_t count, uint32_t stride) {
		glMultiDrawElementsIndirect(decode_type(), GL_UNSIGNED_INT, indirect, count, stride);
	}

	void RendererCommands::polygon_mode(uint32_t face, uint32_t mode) {
		glPolygonMode(face, mode);
	}

	void RendererCommands::set_prim_type(int prim_type) {
		prim = prim_type;
	}

	int RendererCommands::decode_type() {
		switch (prim) {
		case TRIANGLE: return GL_TRIANGLES;
		case LINE: return GL_LINES;
		case POINT: return GL_POINTS;
		default: return GL_TRIANGLES;
		}
	}

	void RendererCommands::line_width(float width) {
		glLineWidth(width);
	}
} 