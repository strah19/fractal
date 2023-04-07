#ifndef GLFW_WINDOW_H
#define GLFW_WINDOW_H

#include "fractal.h"
#include "window.h"
#include "keyboard_event.h"
#include "mouse_event.h"
#include "window_event.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Fractal {
    class GLFWWindow : public Window {
    public:
        GLFWWindow(WindowProperties properties, const EventCallbackFn& event_callback);
        virtual ~GLFWWindow();

        virtual void* get_native_window() override;
        virtual void update() override;
        virtual void destroy() override;
        virtual void quit() override;
    private:
        GLFWwindow* m_window = nullptr;
    };
} // namespace Fractal

#endif // !GLFW_WINDOW_H