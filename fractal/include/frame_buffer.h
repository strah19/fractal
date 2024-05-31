#ifndef OPENGL_FRAME_BUFFER_H
#define OPENGL_FRAME_BUFFER_H

#include <memory>

namespace Fractal {
	class FrameBuffer {
	public:
		FrameBuffer(uint32_t width, uint32_t height);
		FrameBuffer() = default;

		void init(uint32_t width, uint32_t height);
		virtual ~FrameBuffer();

		void bind();
		void unbind();
		uint32_t get_color_attachment() { return m_color_attachment; }
		uint32_t get_buffer_stencil_attachment() const { return m_depth_stencil_attachment; }
	private:
		uint32_t m_frame_buffer_id;
		uint32_t m_color_attachment;
		uint32_t m_depth_stencil_attachment;
	};

	class RenderBuffer {
	public:
		RenderBuffer(uint32_t width, uint32_t height);
		RenderBuffer() = default;

		void init(uint32_t width, uint32_t height);
	private:
		uint32_t m_rbo;
	};
}

#endif // !OPENGL_FRAME_BUFFER_H