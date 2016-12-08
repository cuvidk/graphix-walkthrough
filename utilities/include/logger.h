#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include <memory>
#include <sstream>
#include <map>

#define LOG(level) Logger::log(level, "")

#define TRACE LoggingLevel::trace
#define DEBUG LoggingLevel::debug
#define INFO LoggingLevel::info
#define WARNING LoggingLevel::warning
#define ERROR LoggingLevel::error
#define FATAL LoggingLevel::fatal

enum class LoggingLevel : int {
    trace,
    debug,
    info,
    warning,
    error,
    fatal
};

static std::map<LoggingLevel, std::string> level_to_string_mapper = {
    { LoggingLevel::trace, "TRACE  " },
    { LoggingLevel::debug, "DEBUG  " },
    { LoggingLevel::info, "INFO   " },
    { LoggingLevel::warning, "WARNING" },
    { LoggingLevel::error, "ERROR  " },
    { LoggingLevel::fatal, "FATAL  " },
};

class Logger : public std::ofstream {
public:
    Logger(const Logger& other) = delete;
    Logger& operator=(const Logger& other) = delete;
    Logger(Logger&& other) = delete;
    Logger& operator=(Logger&& other) = delete;

    static void initialize(const std::string& file_name);
    static Logger& log(const LoggingLevel& level, const std::string& message);

    ~Logger();

private:
    Logger(const std::string& file_name);

private:
    static std::unique_ptr<Logger> logger_;
};

#endif /* LOGGER_H */
