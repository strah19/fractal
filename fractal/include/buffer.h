#ifndef BUFFER_H
#define BUFFER_H

#include <memory>
#include <vector>
#include <string>

namespace Fractal {
	enum class VertexShaderType {
		None, Float, Int
	};

	struct VertexBufferElement {
		uint32_t size;
		uint32_t index;
		VertexShaderType type;
		bool normalized;
		uint32_t offset;

		VertexBufferElement(uint32_t size, bool normalized, VertexShaderType type)
			: size(size), index(0), normalized(normalized), type(type), offset(0) { }
	};

	static uint32_t get_size_in_bytes(VertexShaderType type) {
		switch (type)
		{
		case VertexShaderType::None: return 0;
			break;
		case VertexShaderType::Float: return 4;
			break;
		case VertexShaderType::Int: return 4;
			break;
		default:
			break;
		}

		return 0;
	}

	class VertexBufferLayout {
	public:
		VertexBufferLayout(const std::initializer_list<VertexBufferElement>& elements)
			: m_elements(elements) {
			calculate();
		}

		VertexBufferLayout() { }

		void add_to_buffer(const VertexBufferElement& element) {
			m_elements.push_back(element);
			m_elements.back().index = (uint32_t)(m_elements.size() - 1);
		}

		uint32_t calculate() {
			m_stride = 0;
			for (auto& element : m_elements) {
				element.offset = m_stride;
				m_stride += element.size;
			}

			return m_stride;
		}

		std::vector<VertexBufferElement> get_layout() const { return m_elements; }
		VertexBufferElement last_element() const { return m_elements.back(); }

		uint32_t get_stride() const { return m_stride; }
	private:
		std::vector<VertexBufferElement> m_elements;
		uint32_t m_stride = 0;
	};

	class VertexBuffer {
	public:
		VertexBuffer(uint32_t size);
		VertexBuffer(float* vertices, uint32_t size);
		virtual ~VertexBuffer();

		void bind();
		void unbind();
		void set_data(void* data, uint32_t size, uint32_t offset = 0);

		uint32_t get_id() const { return m_vertex_buffer_id; }

		void set_layout(const VertexBufferLayout& lay) { m_layout = std::make_shared<VertexBufferLayout>(lay); }
		std::shared_ptr<VertexBufferLayout> get_layout() { return m_layout; }
	private:
		uint32_t m_vertex_buffer_id;
		std::shared_ptr<VertexBufferLayout> m_layout;
	};

	class IndexBuffer {
	public:
		IndexBuffer(uint32_t* indices, uint32_t size);
		IndexBuffer(uint32_t size);
		virtual ~IndexBuffer();
		void set_data(uint32_t* data, uint32_t size);

		void bind();
		void unbind();
		uint32_t get_id() const { return m_index_buffer_id; }
		uint32_t get_count() const { return m_count; }
	private:
		uint32_t m_index_buffer_id;
		uint32_t m_count = 0;
	};

	class UniformBuffer {
	public:
		UniformBuffer(uint32_t size, uint32_t bindpoint);
		virtual ~UniformBuffer();

		uint32_t get_uniform_block_id(uint32_t shader_id, const std::string& block_name);
		void bind_to_bind_point();
		void bind_to_shader(uint32_t shader_id, const std::string& block_name);
		void allocate_data(uint32_t size);

		void bind();
		void unbind();
		uint32_t get_id() const;
		void set_data(void* data, uint32_t size, uint32_t offset);
	private:
		uint32_t m_uniform_buffer_id;
		uint32_t m_uniform_buffer_point;
		uint32_t m_size_of_buffer;
	};

	class IndirectDrawBuffer {
	public:
		IndirectDrawBuffer(uint32_t size);
		virtual ~IndirectDrawBuffer();

		void bind();
		void unbind();
		uint32_t get_id() const;
		void set_data(void* data, uint32_t size, uint32_t offset);
		void allocate_data(uint32_t size, void* data);
	private:
		uint32_t m_indirect_buffer_id;
		uint32_t m_size_of_buffer;
	};

	class ShaderStorageBuffer {
	public:
		ShaderStorageBuffer(uint32_t size, uint32_t bindpoint);
		virtual ~ShaderStorageBuffer();

		uint32_t get_uniform_block_id(uint32_t shader_id, const std::string& block_name);
		void bind_to_bind_point();
		void bind_to_shader(uint32_t shader_id, const std::string& block_name);

		void bind();
		void unbind();
		uint32_t get_id() const;
		void set_data(void* data, uint32_t size, uint32_t offset);
		void allocate_data(uint32_t size, void* data);
	private:
		uint32_t m_shader_storage_id;
		uint32_t m_binding_point;
		uint32_t m_size_of_buffer;
	};
}

#endif // !BUFFER_H