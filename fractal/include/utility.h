#ifndef UTILITY_H
#define UTILITY_H

#include <string>
    
namespace Fractal {
    int version_major();
    int version_minor();

    class Time {
    public:
        static float get_time();
    };

    void set_icon(const char* path);
    std::string get_name_of_path(const std::string& path);
} // namespace Fractal

#endif //!UTILITY_H