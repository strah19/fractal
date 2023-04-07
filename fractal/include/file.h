#ifndef FILE_H
#define FILE_H

#include <string>
#include <functional>
#include <sstream>
#include <fstream>

namespace Fractal {
    class File {
    public:
        File(const std::string& filepath);
        File();

        virtual ~File();
        void empty();
        void close();
        void destroy();
        void open(const std::string& filepath);
        std::fstream& handler() { return m_file; }

        bool is_open();
        bool eof();
        std::streampos location();
        void reset();
        void location(const uint32_t read_location);
        std::streamoff size();
        bool is_empty();
        std::string path();

        uint64_t line_count();
        uint64_t character_count();
        uint64_t word_count();

        std::string read();
        std::string read_line(const uint32_t line_number);
        std::string read_word(const uint32_t location);
        void delete_line(const uint32_t line_number);

        template <typename T>
        void read_obj(T& object) {
            m_file.read((char*)&object, sizeof(object));
        }

        template <typename T>
        void write_obj(T& object) {
            m_file.write((char*)&object, sizeof(object));
        }

        template<typename T>
        void write_as_line(const T& stream, const uint32_t line_number) {
            std::string file_data = read();

            int word_counter = 0;
            if (line_number > -1) {
                for (size_t i = 0; i < file_data.length(); i++) {
                    if (file_data[i] == '\n') {
                        if (line_number == word_counter) {
                            std::stringstream ss;
                            ss << stream;
                            file_data.insert(i, ss.str());
                            break;
                        }
                        word_counter++;
                    }
                }

                empty();
                m_file << file_data;
            }
        }

        template <typename T>
        void write(const T& stream) {
            std::stringstream ss;
            ss << stream;

            m_file << ss.str();
        }

        void do_each_word(const std::function<bool(const std::string& word, const uint32_t counter)>& func);
        void do_each_line(const std::function<bool(const std::string& line, const uint32_t counter)>& func);
    protected:
        std::string m_filepath;
        std::fstream m_file;
    };
}

#endif // !FILE_H