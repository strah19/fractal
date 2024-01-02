#ifndef APPLICATION_H
#define APPLICATION_H

#include "fractal.h"
#include "window.h"
#include "event.h"
#include "keyboard_event.h"
#include "mouse_event.h"
#include "window_event.h"
#include "layer.h"
#include "imgui_layer.h"

int main(int argc, char* argv[]);

namespace Fractal {
    class Application {
    public:
        virtual ~Application();
        void initialize(const char* name = "Fractal Application", uint32_t width = 1280, uint32_t height = 720, int flags = 0);
        void run();

        static Application& get() { return *m_instance; }
        Window* get_window() { return m_window; }
        void push_layer(Layer* layer);

        virtual void on_user_event(Event& event) { }
        virtual void on_create() { }
        virtual void on_update() { }
        virtual void on_destroy() { }
        virtual void on_gui() { }
    protected:
        WindowProperties m_properties;
        Window* m_window = nullptr;
        LayerStack m_layers; 
    	ImGuiLayer* m_imgui_layer;
    private:
  		void on_close(const QuitEvent& event);
		void on_resize(const ResizeEvent& event);
		void on_event(Event& event);  
    private:
        static Application* m_instance;
        float m_lastframe_time = 0.0f;
		friend int ::main(int argc, char** argv);
    };

    static Application* create_application();
} // namespace Fractal

#endif // !APPLICATION_H