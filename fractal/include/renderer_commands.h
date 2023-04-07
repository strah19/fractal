#ifndef RENDERER_COMMANDS_H
#define RENDERER_COMMANDS_H

#include <stdint.h>

namespace Fractal {
    class RendererCommands {
    public:
        static void initialize();
        static void clear(float r, float g, float b, float a);
        static void set_viewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
    };
}

#endif //!RENDERER_COMMANDS_H