#include "utility.h"
#include "config.h"

#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "application.h"

namespace Fractal {
    int version_major() {
        return FRACTAL_VERSION_MAJOR;
    }

    int version_minor() {
        return FRACTAL_VERSION_MINOR;
    }

    float Time::get_time() {
        return glfwGetTime();
    }

    void set_icon(const char* path) {
        GLFWimage images[1]; 
        images[0].pixels = stbi_load(path, &images[0].width, &images[0].height, 0, 4); 
        glfwSetWindowIcon(static_cast<GLFWwindow*>(Application::get().get_window()->get_native_window()), 1, images); 
        stbi_image_free(images[0].pixels);  
    }

    std::string get_name_of_path(const std::string& path) {
        return path.substr(path.find_last_of("/\\") + 1);;
    }
} // namespace Fractal
