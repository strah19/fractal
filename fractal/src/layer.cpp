#include "layer.h"
#include "log.h"
#include <algorithm>

namespace Fractal {
	void LayerStack::destroy() {
		for (Layer* m_layer : m_layers) {
			m_layer->on_detach();
			FRACTAL_LOG("Deleted layer '%s'", m_layer->get_name().c_str());
			delete m_layer;
		}
	}

	void LayerStack::push_layer(Layer* m_layer) {
		m_layers.emplace(m_layers.begin() + m_insert_index, m_layer);
		m_insert_index++;
	}

	void LayerStack::pop_layer(Layer* m_layer) {
		auto it = std::find(m_layers.begin(), m_layers.begin() + m_insert_index, m_layer);
		if (it != m_layers.begin() + m_insert_index) {
			m_layer->on_detach();
			m_layers.erase(it);
			m_insert_index--;
		}
	}
}