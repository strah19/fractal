#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

#include "layer.h"

namespace Fractal {
    class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();

		virtual void on_attach();
		void on_detach();

		void begin();
		void end();
	};
}

#endif // !IMGUI_LAYER_H