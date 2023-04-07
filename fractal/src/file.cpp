#include "file.h"
#include <vector>
#include <iostream>
#include "log.h"

namespace Fractal {
    File::File(const std::string& filepath) : m_filepath(filepath) {
        open(m_filepath);
    }

    File::File() : m_filepath() { }

    File::~File() {
        if (is_open())
            close();
    }

    void File::empty() {
        m_file.flush();
        m_file.close();
        /* Setting it to the trunc mode will clear the m_file's contents. */
        m_file.open(m_filepath, std::fstream::out | std::fstream::trunc);
    }

    void File::close() {
        m_file.flush();
        m_file.close();
        FRACTAL_LOG("Closed file '%s'", m_filepath.c_str());
    }

    void File::destroy() {
        close();

        remove(m_filepath.c_str());
    }

    void File::open(const std::string& filepath) {
        m_file = std::fstream(filepath, std::fstream::in | std::fstream::out | std::fstream::app);
        m_filepath = filepath;

        FRACTAL_LOG("Opened file '%s'", m_filepath.c_str());
    }

    bool File::is_open() {
        return m_file.is_open();
    }

    bool File::eof() {
        return m_file.eof();
    }

    std::streampos File::location() {
        return m_file.tellg();
    }

    void File::reset() {
        m_file.clear();
        m_file.seekg(0);
    }

    void File::location(const uint32_t read_location) {
        if (read_location > -1)
            m_file.seekg(read_location, std::ios::beg);
    }

    std::streamoff File::size() {
        std::streampos beg, end;

        m_file.seekg(0, std::ios::beg);
        beg = m_file.tellg();
        m_file.seekg(0, std::ios::end);
        end = m_file.tellg();

        return end - beg;
    }

    bool File::is_empty() {
        return (size() == 0) ? true : false;
    }

    std::string File::path() {
        return m_filepath;
    }

    uint64_t File::line_count() {
        reset();
        int num_lines = 0;
        std::string unused;
        while (std::getline(m_file, unused))
            ++num_lines;

        return num_lines;
    }

    uint64_t File::character_count() {
        reset();

        char active_char;
        unsigned long number_chars = 0;

        while (m_file.get(active_char))
            if (active_char != '\n')
                number_chars++;

        return number_chars;
    }

    uint64_t File::word_count() {
        reset();

        std::string search_word;
        uint64_t word_counter = 0;
        while (m_file >> search_word)
            word_counter++;

        return word_counter;
    }

    std::string File::read() {
        reset();

        std::string buffer((std::istreambuf_iterator<char>(m_file)),
            std::istreambuf_iterator<char>());

        return buffer;
    }

    std::string File::read_line(const uint32_t line_number) {
        reset();

        std::string line_search;
        uint32_t line_counter = 0;

        while (getline(m_file, line_search)) {
            if (line_counter == line_number)
                return line_search;
            line_counter++;
        }

        return "";
    }

    std::string File::read_word(const uint32_t location) {
        reset();

        std::string word_search;
        uint32_t word_counter = 0;
        while (m_file >> word_search) {
            if (word_counter == location)
                return word_search;
            word_counter++;
        }
        return "";
    }

    void File::delete_line(const uint32_t line_number) {
        std::string m_file_data = read();

        std::string line_search;
        unsigned int line_counter = 0;

        std::vector<size_t> line_sizes;

        while (getline(m_file, line_search)) {
            line_sizes.push_back(line_search.length());

            if (line_counter == line_number) {
                size_t location = 0;
                for (size_t i = 0; i < line_sizes.size() - 1; i++)
                    location += line_sizes[i];
                m_file_data.erase(location, line_sizes.back() + 1);
                break;
            }
            line_counter++;
        }

        line_sizes.clear();

        empty();
        m_file << m_file_data;
    }

    void File::do_each_word(const std::function<bool(const std::string& word, const uint32_t counter)>& func) {
        std::string word_search;
        uint32_t word_counter = 0;
        while (m_file >> word_search) {
            if (!func(word_search, word_counter))
                break;
            word_counter++;
        }

        reset();
    }

    void File::do_each_line(const std::function<bool(const std::string& line, const uint32_t counter)>& func) {
        std::string line;
        uint32_t line_counter = 0;
        while (getline(m_file, line)) {
            if (!func(line, line_counter))
                break;
            line_counter++;
        }

        reset();
    }
}