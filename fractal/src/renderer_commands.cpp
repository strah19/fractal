#include "renderer_commands.h"
#include <glad/glad.h>

namespace Fractal {
    void RendererCommands::initialize() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
    }

    void RendererCommands::clear(float r, float g, float b, float a) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(r, g, b, a);
    }

    void RendererCommands::set_viewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
        glViewport(x, y, w, h);
    }
} 