/**
 * @file shader.cpp
 * @author strah19
 * @date October 15, 2023
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
 * This file contains the shader code.
 */

#include "shader.h"
#include "log.h"

#include <glad/glad.h>
#include <gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Fractal {
	static uint32_t current_shader_binded = 0;

	Shader::Shader(const std::string& file_path) {
		init(file_path);
	}

	Shader::~Shader() {
		glDeleteProgram(m_shader_id);
	}

	void Shader::bind() {
		if (m_shader_id != current_shader_binded) {
			glUseProgram(m_shader_id);
			current_shader_binded = m_shader_id;
		}
	}

	void Shader::unbind() {
		glUseProgram(0);
		current_shader_binded = 0;
	}

	void Shader::init(const std::string& file_path) {
		m_shader_id = create_shader(parse_shader(file_path));
		FRACTAL_LOG_GOOD("Asset shader '%s' loaded as shader #%d", file_path.c_str(), m_shader_id);
	}

	uint32_t Shader::compile_shader(const std::string& source, uint32_t type) {
		uint32_t id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (!result)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)malloc(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);
			FRACTAL_LOG_ERROR("Shader failed to load: %s", message);

			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	ShaderSources Shader::parse_shader(const std::string& file_path) {
		std::ifstream stream(file_path);

		enum class ShaderType {
			NONE = -1, VERTEX = GL_VERTEX_SHADER, FRAGMENT = GL_FRAGMENT_SHADER, GEOMETRY = GL_GEOMETRY_SHADER, TESS_EVAL = GL_TESS_EVALUATION_SHADER, TESS_CONTROL = GL_TESS_CONTROL_SHADER
		};

		ShaderType type = ShaderType::NONE;
		std::string line;

		if (!stream.is_open()) {
			FRACTAL_LOG_ERROR("Failed to load asset shader '%s'.", file_path.c_str());
			return ShaderSources();
		}

		ShaderSources ss;
		while (getline(stream, line)) {
			if (line.find("#shader") != std::string::npos) {
				if (line.find("vertex") != std::string::npos)
					type = ShaderType::VERTEX;
				else if (line.find("fragment") != std::string::npos)
					type = ShaderType::FRAGMENT;
				else if (line.find("geometry") != std::string::npos)
					type = ShaderType::GEOMETRY;
				else if (line.find("tess-control") != std::string::npos)
					type = ShaderType::TESS_CONTROL;
				else if (line.find("tess-eval") != std::string::npos)
					type = ShaderType::TESS_EVAL;
			}
			else {
				ss[(uint32_t)type] << line << '\n';
			}
		}

		stream.close();
		return ss;
	}

	uint32_t Shader::create_shader(const ShaderSources& shader_sources) {
		uint32_t program = glCreateProgram();

		for (auto& shader : shader_sources) {
			uint32_t s = compile_shader(shader.second.str(), shader.first);
			glAttachShader(program, s);
			glDeleteShader(s);
		}

		glLinkProgram(program);  
		glValidateProgram(program);

		return program;
	}

	uint32_t ProgramGetUniformLocation(uint32_t id, const std::string& name) {
		return (glGetUniformLocation(id, name.c_str()));
	}

	void ProgramSet1f(uint32_t id, const std::string& name, float value) {
		glProgramUniform1f(id, ProgramGetUniformLocation(id, name), value);
	}

	void ProgramSetMat4f(uint32_t id, const std::string& name, const glm::mat4& mat4) {
		glProgramUniformMatrix4fv(id, ProgramGetUniformLocation(id, name), 1, GL_FALSE, glm::value_ptr(mat4));
	}

	void ProgramSetVec3f(uint32_t id, const std::string& name, const glm::vec3& vec3) {
		glProgramUniform3f(id, ProgramGetUniformLocation(id, name), vec3.x, vec3.y, vec3.z);
	}

	void ProgramSetIntArray(uint32_t id, const std::string& name, int* array) {
		glProgramUniform1iv(id, ProgramGetUniformLocation(id, name), sizeof(array) / sizeof(int), array);
	}

	void Shader::set1f(const std::string& name, float value) {
		ProgramSet1f(m_shader_id, name, value);
	}

	uint32_t Shader::get_uniform_location(const std::string& name) {
		return (glGetUniformLocation(m_shader_id, name.c_str()));
	}

	void Shader::set_mat4f(const std::string& name, const glm::mat4& mat4) {
		glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(mat4));
	}

	void Shader::set_vec2f(const std::string& name, const glm::vec2& vec2) {
		glUniform2f(get_uniform_location(name), vec2.x, vec2.y);
	}

	void Shader::set_vec3f(const std::string& name, const glm::vec3& vec3) {
		glUniform3f(get_uniform_location(name), vec3.x, vec3.y, vec3.z);
	}

	void Shader::set_int_array(const std::string& name, int* array, uint32_t size) {
		glUniform1iv(get_uniform_location(name), size, array);
	}

	std::vector<std::string> GetUniformNames(uint32_t id) {
		GLint i;
		GLint count;

		GLint size;
		GLenum type;

		const GLsizei bufSize = 16;
		GLchar name[bufSize];
		GLsizei length;
		std::vector<std::string> names;

		glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count);

		for (i = 0; i < count; i++) {
			glGetActiveUniform(id, (GLuint)i, bufSize, &length, &size, &type, name);
			names.push_back(name);
		}

		return names;
	}
}