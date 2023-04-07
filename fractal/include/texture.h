#ifndef TEXTURE_H
#define TEXTURE_H

#include <memory>
#include <string>

namespace Fractal {
	class Texture {
	public:
		Texture() = default;

		void initialize(const char* file_path);
		void initialize(uint32_t width, uint32_t height);

		virtual ~Texture();

		void bind(uint32_t slot = 0);
		void unbind();

		void set_data(void* data);
		void* get_data() { return m_data; }

		uint32_t get_width() const { return m_width; }
		uint32_t get_height() const { return m_height; }
		uint32_t get_texture_id() const { return m_texture_id; }
	private:
		uint32_t m_texture_id = 0;

		uint32_t m_width = 0;
		uint32_t m_height = 0;
		uint32_t m_internal_format = 0, m_data_format = 0;
		std::string m_path;
		void* m_data = nullptr;
	};
}

#endif // !OPENGL_TEXTURE_H