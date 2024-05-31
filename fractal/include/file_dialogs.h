#ifndef FILE_DIALOG_H
#define FILE_DIALOG_H

#include "platform.h"
#include <string>

#define MAX_FILENAME_LEN 256

namespace Fractal {    
	class FileDialogs {
    public:
        static std::string open(const char* filter);
        static std::string save(const char* filter, const char* name);
	
        static bool browse_folder(char* out, int max_size, const char* starting_dir);
	};
}

#endif // !FILE_DIALOG_H