#ifndef LAYER_H
#define LAYER_H

#include <string>
#include <vector>
#include "event.h"

namespace Fractal {
	class Layer {
	public:
		Layer(const std::string& name)
			: m_name(name) { }
		virtual ~Layer() = default;

		virtual void on_attach() {}
		virtual void on_detach() {}
		virtual void on_update(float delta) {}
		virtual void user_def_event(Event& event) {}
		virtual void update_gui() { }

		inline std::string get_name() const { return m_name; }
	protected:
		std::string m_name;
	};

	class LayerStack {
	public:
		LayerStack() = default;
		virtual ~LayerStack() = default;

		void destroy();
		void push_layer(Layer* layer);
		void pop_layer(Layer* layer);

		inline std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		inline std::vector<Layer*>::iterator end() { return m_layers.end(); }
		inline std::vector<Layer*>::reverse_iterator rbegin() { return m_layers.rbegin(); }
		inline std::vector<Layer*>::reverse_iterator rend() { return m_layers.rend(); }

		inline std::vector<Layer*>::const_iterator begin() const { return m_layers.begin(); }
		inline std::vector<Layer*>::const_iterator end() const { return m_layers.end(); }
		inline std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_layers.rbegin(); }
		inline std::vector<Layer*>::const_reverse_iterator rend() const { return m_layers.rend(); }
	private:
		std::vector<Layer*> m_layers;
		unsigned int m_insert_index = 0;
	};
}

#endif // !LAYER_H