#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include <memory>
#include <sstream>
#include <map>

#define LOG(level) graphix::engine::utilities::Logger::log(level, "")

#define TRACE graphix::engine::utilities::LoggingLevel::trace
#define DEBUG graphix::engine::utilities::LoggingLevel::debug
#define INFO graphix::engine::utilities::LoggingLevel::info
#define WARNING graphix::engine::utilities::LoggingLevel::warning
#define ERROR graphix::engine::utilities::LoggingLevel::error
#define FATAL graphix::engine::utilities::LoggingLevel::fatal

namespace graphix {
namespace engine {
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
} /* namespace engine */
} /* namespace graphix */

#endif /* LOGGER_H */
