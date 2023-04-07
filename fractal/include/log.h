#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <map>
#include <vector>
#include <cstdarg>
#include <sstream>

#include "file.h"

namespace Fractal {
	class LogCommand {
	public:
		LogCommand() : m_command_name("") { }
		virtual void run_command(va_list& args, const char* input) = 0;
		virtual void process_args(va_list& args) = 0;

		inline const std::string get_command() const { return m_command_name; }
		inline const std::string get_command_output() const { return m_command_output; }

		inline void set_command(const std::string& command_name) { m_command_name = command_name; };
		inline void set_command_output(const std::string& command_output) { m_command_output = command_output; };
	protected:
		std::string m_command_name;
		std::string m_command_output;
	};

	typedef LogCommand* (__stdcall* CreateLogCommandFn)(void);

	class TimestampLogCommand : public LogCommand {
	public:
		TimestampLogCommand() { }
		void run_command(va_list& args, const char* input) override;
		void process_args(va_list& args) override;
		static LogCommand* __stdcall create() { return new TimestampLogCommand(); }
	};

	class UserLogCommand : public LogCommand {
	public:
		UserLogCommand() { }
		void run_command(va_list& args, const char* input) override;
		void process_args(va_list& args) override;
		static LogCommand* __stdcall create() { return new UserLogCommand(); }
	};

	class LeftLogCommand : public LogCommand {
	public:
		LeftLogCommand() { }
		void run_command(va_list& args, const char* input) override;
		void process_args(va_list& args) override;
		static LogCommand* __stdcall create() { return new LeftLogCommand(); }
	};

	enum ColorCode {
		FG_RED = 31,
		FG_GREEN = 32,
		FG_BLUE = 34,
		FG_YELLOW = 33,
		FG_DEFAULT = 37,

		BG_RED = 41,
		BG_GREEN = 42,
		BG_BLUE = 44,
		BG_DEFAULT = 49
	};

	class ColorController {
	public:
		ColorController() : m_code(FG_DEFAULT) { }

		inline void set_color(ColorCode code) { m_code = code; }

		friend std::ostream& operator<<(std::ostream& os, const ColorController& controller) {
			return os << "\033[" << controller.m_code << "m";
		}
	private:
		ColorCode m_code;
	};

	class ColorLogCommand : public LogCommand {  
	public:
		ColorLogCommand() { }
		void run_command(va_list& args, const char* input) override;
		void process_args(va_list& args) override;
		static LogCommand* __stdcall create() { return new ColorLogCommand(); }
	private:
		ColorController m_color_controller;
	};

	class LogFormat {
	public:
		void initialize(const char* format, ...);
		std::vector<LogCommand*>& get_commands() { return m_commands; }
	private:
		std::string look_for_command(const std::string& input, size_t index);
		void new_command(const std::string& name, va_list& args);

		std::vector<LogCommand*> m_commands;
	};

	class Logger {
	public:
		void set_log_format(LogFormat* log_format);
		void log(const char* fmt, ...); 
	private:
		LogFormat* m_formatter;
	};

	using LogEntries = std::vector<std::string>;
	void initialize_logging_system();
	LogEntries* get_log_entries();

	class Logs {
	public:
		static void intialize_loggers();

		static Logger& get_log_errors();
		static Logger& get_log_warnings();
		static Logger& get_log_def();
		static Logger& get_log_def_good();
	};
}

#define FRACTAL_LOG_ERROR(...) Fractal::Logs::get_log_errors().log(__VA_ARGS__)
#define FRACTAL_LOG_WARNING(...) Fractal::Logs::get_log_warnings().log(__VA_ARGS__)
#define FRACTAL_LOG(...) Fractal::Logs::get_log_def().log(__VA_ARGS__)
#define FRACTAL_LOG_GOOD(...) Fractal::Logs::get_log_def_good().log(__VA_ARGS__)

#endif // !LOGGER_H