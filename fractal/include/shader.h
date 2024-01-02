#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <unordered_map>

namespace Fractal {
	using ShaderSources = std::unordered_map<uint32_t, std::stringstream>;

	class Shader {
	public:
		Shader(const std::string& file_path);
		Shader() = default;

		virtual ~Shader();

		void bind();
		void unbind();

		void init(const std::string& file_path);

		/* Uniforms go here! */
		void set1f(const std::string& name, float value);
		void set_mat4f(const std::string& name, const glm::mat4& mat4);
		void set_vec3f(const std::string& name, const glm::vec3& vec3);
		void set_vec2f(const std::string& name, const glm::vec2& vec2);
		void set_int_array(const std::string& name, int* array, uint32_t size);

		uint32_t get_uniform_location(const std::string& name);
		uint32_t get_id() const { return m_shader_id; }
	private:
		uint32_t m_shader_id;
		ShaderSources parse_shader(const std::string& file_path);
		uint32_t compile_shader(const std::string& source, uint32_t type);
		uint32_t create_shader(const ShaderSources& shader_sources);
	};
}

#endif // !SHADER_H
