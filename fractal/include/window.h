#ifndef WINDOW_H
#define WINDOW_H

#include "fractal.h"
#include "event.h"

namespace Fractal {
    struct WindowProperties {
        const char* m_name;
        uint32_t m_width = 0;
        uint32_t m_height = 0;

        uint32_t m_x = 0;
        uint32_t m_y = 0;

        WindowProperties() = default;
        WindowProperties(const char* name, uint32_t width, uint32_t height) : m_name(name), m_width(width), m_height(height) { }
    };
    
    class Window {
    public:
    	using EventCallbackFn = std::function<void(Event&)>;

        Window(WindowProperties properties, const EventCallbackFn& event_callback);
        virtual ~Window() = default;

        virtual void* get_native_window() = 0;
        virtual void update() = 0;
        virtual void destroy() = 0;
        
        inline WindowProperties* properties() { return &m_properties; }
        inline bool destroyed() const { return m_destroyed; }
        static Window* create_glfw_window(WindowProperties properties, const EventCallbackFn& event_callback);
    protected:
        WindowProperties m_properties;
        EventCallbackFn m_event_callback;
        bool m_destroyed = false;
    };
} // namespace Fractal


#endif // !WINDOW_H