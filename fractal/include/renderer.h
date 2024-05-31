#ifndef OPEN_RENDERER_H
#define OPEN_RENDERER_H

#include "shader.h"
#include "vertex_array.h"
#include "texture.h"
#include "camera.h"
#include "mesh.h"

namespace Fractal {
	constexpr uint32_t MAX_TEXTURE_SLOTS = 32;
	constexpr size_t MAX_DRAW_COMMANDS = 1000;
	constexpr size_t MAX_VERTEX_COUNT = 10000;
	constexpr size_t MAX_INDEX_COUNT = 10000;
	constexpr size_t QUAD_VERTEX_COUNT = 4;
	constexpr glm::vec2 TEX_COORDS[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
	constexpr glm::vec4 QUAD_POSITIONS[QUAD_VERTEX_COUNT] = {
		{ -0.5f, -0.5f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, 0.0f, 1.0f },
		{ 0.5f,  0.5f, 0.0f, 1.0f },
		{ -0.5f,  0.5f, 0.0f, 1.0f }
	};

	constexpr glm::vec2 CUBE_TEX_COORDS[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
	constexpr size_t CUBE_VERTEX_COUNT = 8;
	constexpr glm::vec4 CUBE_POSITIONS[CUBE_VERTEX_COUNT] = {
		{ -1.0, -1.0,  1.0, 1.0 },
		{ 1.0, -1.0,  1.0, 1.0 },
		{ 1.0,  1.0,  1.0, 1.0 },
		{ -1.0,  1.0,  1.0, 1.0 },
	
		{ -1.0, -1.0, -1.0, 1.0 },
		{ 1.0, -1.0, -1.0, 1.0 },
		{ 1.0,  1.0, -1.0, 1.0 },
		{ -1.0,  1.0, -1.0, 1.0 }
	};

	constexpr size_t CUBE_INDICES_COUNT = 36;
	constexpr int cube_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	struct DeviceStatistics {
		uint32_t num_of_vertices = 0;
		uint32_t num_of_indices = 0;
		uint32_t draw_count = 0;
		uint32_t max_vertex_count = 0;
		uint32_t max_index_count = 0;

		void reset();
	};

	struct DrawElementsCommand {
		uint32_t vertex_count = 0;
		uint32_t instance_count = 0;
		uint32_t first_index = 0;
		uint32_t base_vertex = 0;
		uint32_t base_instance = 0;
	};

	class RendererFrame;

	template <typename V>
	class GraphicsDevice {
	public:
		GraphicsDevice() = default;
		GraphicsDevice(uint32_t max_vertex_count, uint32_t max_index_count);
		virtual ~GraphicsDevice();

		virtual void init() = 0;
		virtual void setup() = 0;
		virtual bool submit(Mesh& mesh) = 0;
		virtual void render() = 0;

		inline void set_shader(Shader** shader) { m_shader = shader; }
		inline bool empty() const { return (m_vert_base == m_vert_ptr); }
		inline const DeviceStatistics get_device_stats() const { return m_ds; }

		inline uint32_t* index_ptr() { return m_indx_ptr; }
	protected:
		//Pointer to another shader that is also a pointer :)
		Shader** m_shader = nullptr;

		VertexArray* m_vao = nullptr;
		VertexBuffer* m_vbo = nullptr;
		IndexBuffer* m_ibo = nullptr;

		V* m_vert_base = nullptr;
		V* m_vert_ptr = nullptr;

		uint32_t* m_indx_base = nullptr;
		uint32_t* m_indx_ptr = nullptr;

		DeviceStatistics m_ds;
	};

	class BatchGraphicsDevice : public GraphicsDevice<Vertex> {
	public:
		BatchGraphicsDevice() = default;
		BatchGraphicsDevice(uint32_t max_vertex_count, uint32_t max_index_count);
		virtual ~BatchGraphicsDevice();

		virtual void init() override;
		virtual void setup() override;
		virtual bool submit(Mesh& mesh) override;
		virtual void render() override;

		virtual void next_command();
		virtual void make_command();

		inline uint32_t index_offset() const { return m_index_offset; }
		float calculate_texture_index(uint32_t id);
	private:
		IndirectDrawBuffer* m_idb = nullptr;

		uint32_t m_texture_slot_index = 0;
		uint32_t m_textures[MAX_TEXTURE_SLOTS] = { 0 };

		uint32_t m_index_offset = 0;
		uint32_t m_cmd_vertex_base = 0;
		uint32_t m_draw_command_size = 0;
		uint32_t m_current_draw_command_vertex_size = 0;
		DrawElementsCommand m_commands[MAX_DRAW_COMMANDS];

		void add_vertex(Vertex* v);
		void add_index(uint32_t index);
	};

	enum RenderFlags {
		None = 0x01,
		TopLeft = 0x02
	};

	class RendererFrame {
	public:
		RendererFrame() = default;
		virtual ~RendererFrame();

		virtual void begin_scene(Camera* camera) = 0;
		virtual void end_scene() = 0;
		virtual void submit(Mesh& mesh) = 0;

		inline int get_flags() const { return m_flags; }
		inline void set_flag(int flag, bool v) { if (v) m_flags |= flag; else m_flags &= ~flag; }
		inline void set_shader(Shader* shader) { m_current_shader = shader; }
		inline Shader* get_current_shader() { return m_current_shader; }

		inline BatchGraphicsDevice* get_graphics_device() { return m_gd; }
	protected:
		Camera* m_camera = nullptr;
		glm::mat4 m_proj_view = glm::mat4(1.0f);
		Shader m_default_shader;
		Shader* m_current_shader = nullptr;
		int m_flags = RenderFlags::None;
		BatchGraphicsDevice* m_gd;
	};

	class Renderer : public RendererFrame {
	public:
		Renderer();

		virtual void begin_scene(Camera* camera) override;
		virtual void end_scene() override;
		virtual void submit(Mesh& mesh) override;

		void init_renderer_shader(Shader* shader);
	private:
		ShaderStorageBuffer* m_ssbo;
	};
}

#endif