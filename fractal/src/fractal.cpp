#include "fractal.h"
#include "config.h"

namespace Fractal {
    int version_major() {
        return FRACTAL_VERSION_MAJOR;
    }

    int version_minor() {
        return FRACTAL_VERSION_MINOR;
    }
} // namespace Fractal
