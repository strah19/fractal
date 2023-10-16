#include "renderer_commands.h"
#include <glad/glad.h>

namespace Fractal {
	static int prim;

    void RendererCommands::initialize() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        
		prim = TRIANGLE;
    }

    void RendererCommands::clear(float r, float g, float b, float a) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(r, g, b, a);
    }

    void RendererCommands::set_viewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
        glViewport(x, y, w, h);
    }

	void RendererCommands::DrawVertexArray(VertexArray* vertex_array) {
		glDrawElements(DecodeType(), vertex_array->GetIndexBufferSize(), GL_UNSIGNED_INT, 0);
	}

	void RendererCommands::DrawVertexArrayInstanced(VertexArray* vertex_array, uint32_t instance_count) {
		glDrawArraysInstanced(DecodeType(), 0, vertex_array->GetIndexBufferSize(), instance_count);
	}

	void RendererCommands::DrawMultiIndirect(const void* indirect, uint32_t count, uint32_t stride) {
		glMultiDrawElementsIndirect(DecodeType(), GL_UNSIGNED_INT, indirect, count, stride);
	}

	void RendererCommands::PolygonMode(uint32_t face, uint32_t mode) {
		glPolygonMode(face, mode);
	}

	void RendererCommands::SetPrimType(int prim_type) {
		prim = prim_type;
	}

	int RendererCommands::DecodeType() {
		switch (prim) {
		case TRIANGLE: return GL_TRIANGLES;
		case LINE: return GL_LINES;
		case POINT: return GL_POINTS;
		default: return GL_TRIANGLES;
		}
	}

	void RendererCommands::LineWidth(float width) {
		glLineWidth(width);
	}
} 