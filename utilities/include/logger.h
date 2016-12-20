#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include <memory>
#include <sstream>
#include <map>

#define LOG(level) graphix::utilities::Logger::log(level, "")

#define TRACE graphix::utilities::LoggingLevel::trace
#define DEBUG graphix::utilities::LoggingLevel::debug
#define INFO graphix::utilities::LoggingLevel::info
#define WARNING graphix::utilities::LoggingLevel::warning
#define ERROR graphix::utilities::LoggingLevel::error
#define FATAL graphix::utilities::LoggingLevel::fatal

namespace graphix {
namespace utilities {

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

} /* namespace utilities */
} /* namespace graphix */

#endif /* LOGGER_H */
