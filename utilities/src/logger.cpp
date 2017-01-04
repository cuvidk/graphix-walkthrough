#include "logger.h"

namespace graphix {
namespace engine {
namespace utilities {

std::unique_ptr<Logger> Logger::logger_ = nullptr;

Logger::Logger(const std::string& file_name) : std::ofstream(file_name) {
    if (!*this) {
        throw std::runtime_error{"Failed to create logger. "
                                 "Failed to open output file: " + file_name + "."};
    }
}

void Logger::initialize(const std::string& file_name) {
    if (logger_) {
        throw std::runtime_error{"Multiple initialization of the logger service encountered."};
    }
    logger_.reset(new Logger{file_name});
}

Logger& Logger::log(const LoggingLevel& level, const std::string& message) {
    if (!logger_) {
        throw std::runtime_error{"Failed to log. Logger initialization omitted."};
    }

    auto crt_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm* parts = std::localtime(&crt_time);

    std::stringstream date_ss;

    if (parts->tm_hour < 10) date_ss << 0;
    date_ss << parts->tm_hour << ":";
    if (parts->tm_min < 10) date_ss << 0;
    date_ss << parts->tm_min << ":";
    if (parts->tm_sec < 10) date_ss << 0;
    date_ss << parts->tm_sec << " ";
    if (parts->tm_mday < 10) date_ss << 0;
    date_ss << parts->tm_mday << "/";
    if (parts->tm_mon < 9) date_ss << 0;
    date_ss << (parts->tm_mon + 1) << "/";
    date_ss << (parts->tm_year + 1900);

    *logger_ << "[ " << level_to_string_mapper[level] << " ]\t"
             << "[ " << date_ss.str() << " ]\t" << message;

    return *logger_;
}

Logger::~Logger() {
    this->flush();
}

} /* namespace utilities */
} /* namespace engine */
} /* namespace graphix */
