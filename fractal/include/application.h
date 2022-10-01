#ifndef APPLICATION_H
#define APPLICATION_H

#include "fractal.h"
#include "window.h"
#include "event.h"
#include "keyboard_event.h"
#include "mouse_event.h"
#include "window_event.h"

namespace Fractal {
    class Application {
    public:
        virtual ~Application();
        void initialize(const char* name = "Fractal Application", uint32_t width = 1280, uint32_t height = 720);
        void run();

        virtual void on_user_event(Event& event) { }
        virtual void on_create() { }
        virtual void on_update() { }
        virtual void on_destroy() { }
    protected:
        WindowProperties m_properties;
        Window* m_window = nullptr;
    private:
  		void on_close(const QuitEvent& event);
		void on_resize(const ResizeEvent& event);
		void on_event(Event& event);  
    };
} // namespace Fractal

#endif // !APPLICATION_H