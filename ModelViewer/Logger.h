#pragma once


#include <string>
#include <vector>
#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/format.h>
//
//#    if defined(_MSC_VER) && defined(SPDLOG_WCHAR_FILENAMES) // for some reason msvc doesn't allow fmt::runtime(..) with wchar here 
//return fmt::format(fmt_filename, now_tm);
//#    else 
//return fmt::format(SPDLOG_FMT_RUNTIME(fmt_filename), now_tm);
//#    endif 

#define LOG_TRACE(msg)    Logger::get().log(LogLevel::Trace, msg)
#define LOG_DEBUG(msg)    Logger::get().log(LogLevel::Debug, msg)
#define LOG_INFO(msg)     Logger::get().log(LogLevel::Info, msg)
#define LOG_WARNING(msg)     Logger::get().log(LogLevel::Warning, msg)
#define LOG_ERROR(msg)    Logger::get().log(LogLevel::Error, msg)
#define LOG_CRITICAL(msg) Logger::get().log(LogLevel::Critical, msg)


enum class LogLevel {
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Critical
};

class Logger {
public:
    static Logger& get() {
        static Logger instance;
        return instance;
    }

    void log(LogLevel level, const std::string& message) {
        std::string prefix;
        auto color = fmt::fg(fmt::color::white);
        auto bold = fmt::emphasis::bold;
        switch (level) {
        case LogLevel::Trace:    color = fmt::fg(fmt::color::gray);      prefix = "[Trace] "; break;
        case LogLevel::Debug:    color = fmt::fg(fmt::color::blue);      prefix = "[Debug] "; break;
        case LogLevel::Info:     color = fmt::fg(fmt::color::green);     prefix = "[Info ] "; break;
        case LogLevel::Warning:  color = fmt::fg(fmt::color::yellow);    prefix = "[Warn ] "; break;
        case LogLevel::Error:    color = fmt::fg(fmt::color::red);       prefix = "[Error] "; break;
        case LogLevel::Critical: color = fmt::fg(fmt::color::magenta);   prefix = "[CRIT ] "; break;
        }

        fmt::print(bold | color, "{}", prefix);
        fmt::print(color, "{}\n", message );

        messages.push_back(prefix + message);

    }

    const std::vector<std::string>& getMessages() const {
        return messages;
    }

private:
    std::vector<std::string> messages;
};
