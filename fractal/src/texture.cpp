#include "texture.h"
#include "log.h"

#include <iostream>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Fractal {
	void Texture::initialize(const char* file_path) {
		m_path = file_path;

		int w, h, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* m_data = stbi_load(file_path, &w, &h, &channels, 0);

		m_width = w;
		m_height = h;
		if (channels == 4) {
			m_internal_format = GL_RGBA8;
			m_data_format = GL_RGBA;
		}
		else if (channels == 3) {
			m_internal_format = GL_RGB8;
			m_data_format = GL_RGB;
		}

		if (m_data) {
			glCreateTextures(GL_TEXTURE_2D, 1, &m_texture_id);
			glTextureStorage2D(m_texture_id, 1, m_internal_format, m_width, m_height);

			glTextureParameteri(m_texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_texture_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureParameteri(m_texture_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_texture_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(m_texture_id, 0, 0, 0, m_width, m_height, m_data_format, GL_UNSIGNED_BYTE, m_data);
			FRACTAL_LOG_GOOD("Loaded texture '%s'", file_path);
		}
		else
			FRACTAL_LOG_ERROR("Failed to load texture '%s'", file_path);

		stbi_image_free(m_data);
	}

	void Texture::initialize(uint32_t m_width, uint32_t m_height) {
		m_internal_format = GL_RGBA8;
		m_data_format = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_texture_id);
		glTextureStorage2D(m_texture_id, 1, m_internal_format, m_width, m_height);

		glTextureParameteri(m_texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_texture_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_texture_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_texture_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	Texture::~Texture() {
		glDeleteTextures(1, &m_texture_id);
	}

	void Texture::set_data(void* m_data) {
		uint32_t bpp = m_data_format == GL_RGBA ? 4 : 3;
		glTextureSubImage2D(m_texture_id, 0, 0, 0, m_width, m_height, m_data_format, GL_UNSIGNED_BYTE, m_data);
	}

	void Texture::bind(uint32_t slot) {
		glBindTextureUnit(slot, m_texture_id);
	}

	void Texture::unbind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}