/**
 * @file log.cpp
 * @author strah19
 * @date May 30 2024
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the MIT License as
 * published by the Free Software Foundation.
 *
 * @section DESCRIPTION
 *
 * This file contains code for a logger system.
 */

#include "log.h"
#include <ctime>

#define MAX_INPUT_SIZE 512

namespace Fractal {
    void TimestampLogCommand::run_command(va_list& args, const char* input) {
        const std::time_t now = std::time(nullptr); 
        const std::tm calendar_time = *std::localtime(std::addressof(now));

        m_command_output = std::to_string(calendar_time.tm_hour) + ":" + std::to_string(calendar_time.tm_min) + ":" + std::to_string(calendar_time.tm_sec);
    }

    void TimestampLogCommand::process_args(va_list& args) { }

    void UserLogCommand::run_command(va_list& args, const char* input) {
        char buffer[MAX_INPUT_SIZE];
        vsnprintf(buffer, MAX_INPUT_SIZE, input, args);
        va_end(args);

        set_command_output(buffer);
    }

    void UserLogCommand::process_args(va_list& args) { }
    void LeftLogCommand::run_command(va_list& args, const char* input) { }
    void LeftLogCommand::process_args(va_list& args) { }

    const std::pair<std::string, ColorCode> COLORS[] = {
        std::make_pair("{cR}", ColorCode::FG_RED),
        std::make_pair("{cDef}", ColorCode::FG_DEFAULT),
        std::make_pair("{cB}", ColorCode::FG_BLUE),
        std::make_pair("{cG}", ColorCode::FG_GREEN),
        std::make_pair("{cY}", ColorCode::FG_YELLOW),
    };

    void ColorLogCommand::run_command(va_list& args, const char* input) {
        for (auto& color : COLORS) {
            if (color.first == m_command_name) {
                m_color_controller.set_color(color.second);
                break;
            }
        }

        std::stringstream ss;
        ss << m_color_controller;
        m_command_output = ss.str();
    }

    void ColorLogCommand::process_args(va_list& args) { }

    static std::map<std::string, CreateLogCommandFn> COMMANDS;

    void init_commands();

    static LogEntries log_entries;
    void initialize_logging_system() {
        init_commands();
    }

    void init_commands() {
        COMMANDS["{ts}"] = TimestampLogCommand::create;
        COMMANDS["{l}"] = UserLogCommand::create;
        COMMANDS["{L}"] = LeftLogCommand::create;

        for(auto& color : COLORS)
            COMMANDS[color.first] = ColorLogCommand::create;
    }

    void LogFormat::initialize(const char* format, ...) {
        va_list args;
        va_start(args, format);
 
        std::string input = format;

        size_t start = 0;
        bool in_command = false;
        for (size_t i = 0; i < input.size(); i++) {
            if (input[i] == '{') {
                start = i;
                in_command = true;
            }
            else if (input[i] == '}') {
                std::string name = input.substr(start, i - start + 1);
                
                new_command(name, args);
                in_command = false;
            }
            else if (!in_command) {
                new_command("{L}", args);
                std::string output;
                for (size_t j = i; j < input.size(); j++) {
                    if (input[j] == '{') {
                        std::string name = look_for_command(input, j);
                        if (name != "") 
                            break;
                    }
                    else
                        output.push_back(input[j]);
                }
                i += output.size() - 1;
                m_commands.back()->set_command_output(output);
            }
        }

        va_end(args);
	}

    void LogFormat::new_command(const std::string& name, va_list& args) {
        m_commands.push_back(COMMANDS.find(name)->second());
        m_commands.back()->set_command(name);
        m_commands.back()->process_args(args);
    }

    std::string LogFormat::look_for_command(const std::string& input, size_t index) {
        size_t start = 0;
        for (size_t i = index; i < input.size(); i++) {
            if (input[i] == '{')
                start = i;
            else if (input[i] == '}') {
                std::string name = input.substr(start, i - start + 1);
                if (COMMANDS.find(name) != COMMANDS.end())
                    return name;
                break;
            }
        }
        return "";
    }

    void Logger::set_log_format(LogFormat* log_format) {
        m_formatter = log_format;
    }

    void Logger::log(const char* fmt, ...) {
        if (m_formatter) {
            va_list args;
            va_start(args, fmt);
            std::string output;
            std::string log_output;

            for (auto& command : m_formatter->get_commands()) {
                command->run_command(args, fmt);
                output += command->get_command_output();
                if (command->get_command()[1] != 'c')
                    log_output += command->get_command_output();
            }

            printf("%s", output.c_str());
            log_entries.push_back(log_output);
            va_end(args);
        }
    }

    static Logger error_log;
    static LogFormat error_format;

    static Logger warning_log;
    static LogFormat warning_format;

    static Logger def_log;
    static LogFormat def_format;

    static Logger def_log_good;
    static LogFormat def_format_good;

    void Logs::intialize_loggers() {
        error_format.initialize("{cR}Fractal error[{ts}]: {l}{cDef}.\n");
        error_log.set_log_format(&error_format);

        warning_format.initialize("{cY}Fractal warning[{ts}]: {l}{cDef}.\n");
        warning_log.set_log_format(&warning_format);

        def_format.initialize("{cDef}[{ts}]: {l}{cDef}.\n");
        def_log.set_log_format(&def_format);

        def_format_good.initialize("{cG}[{ts}]: {l}{cDef}.\n");
        def_log_good.set_log_format(&def_format_good);
    }

	LogEntries* GetLogEntries() {
        return &log_entries;
    }

    Logger& Logs::get_log_errors() { return error_log; }

    Logger& Logs::get_log_warnings() { return warning_log; }

    Logger& Logs::get_log_def() { return def_log; }

    Logger& Logs::get_log_def_good() { return def_log_good; }
}