#ifndef OPENGL_VERTEX_ARRAY_H
#define OPENGL_VERTEX_ARRAY_H

#include "buffer.h"

namespace Fractal {
	enum class VertexBufferFormat {
		VNCVNCVNC,
		VVVCCCNNN
	};

	class VertexArray {
	public:
		VertexArray();
		virtual ~VertexArray();

		void bind();
		void unbind();

		std::vector<std::shared_ptr<VertexBuffer>> get_vertex_buffer() { return m_vertex_buffers; };
		uint32_t get_index_buffer_size() const { return m_index_size; }

		void add_vertex_buffer(VertexBuffer* vertex_buf, VertexBufferFormat format);
		void set_index_buffer_size(uint32_t index_buf) { m_index_size = index_buf; }

		void enable_vertex_attrib(uint32_t index);
		void set_array_for_instancing(std::shared_ptr<VertexBuffer>& vertex_buf, uint32_t offset_sizes[], uint32_t stride_sizes[]);

		uint32_t get_id() const { return m_vertex_array_buffer_id; }
	private:
		uint32_t m_vertex_array_buffer_id;

		std::vector<std::shared_ptr<VertexBuffer>> m_vertex_buffers;

		uint32_t m_index_size = 0;
	};
}

#endif // !OPENGL_VERTEX_ARRAY_H
