#ifndef ERROR_H
#define ERROR_H

namespace Fractal {
    void fatal_error(const char* fmt, ...);
    void report_warning(const char* fmt, ...);
    void report_error(const char* fmt, ...);
} // namespace Fractal

#endif // !ERROR_H