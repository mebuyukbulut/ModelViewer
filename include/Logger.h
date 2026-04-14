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


#define LOG_TRACE(...)    Logger::get().log(LogLevel::Trace, __VA_ARGS__)
#define LOG_DEBUG(...)    Logger::get().log(LogLevel::Debug, __VA_ARGS__)
#define LOG_INFO(...)     Logger::get().log(LogLevel::Info, __VA_ARGS__)
#define LOG_WARNING(...)  Logger::get().log(LogLevel::Warning, __VA_ARGS__)
#define LOG_ERROR(...)    Logger::get().log(LogLevel::Error, __VA_ARGS__)
#define LOG_SUCCESS(...)  Logger::get().log(LogLevel::Success, __VA_ARGS__)
#define LOG_CRITICAL(...) Logger::get().log(LogLevel::Critical, __VA_ARGS__)


enum class LogLevel {
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Success,
    Critical
};

class Logger {
    const int MAX_MESSAGE_COUNT = 250;
    const int MESSAGE_BUFFER_COUNT = 50;
    std::vector<std::string> messages;
    
    Logger(){
        messages.reserve(MAX_MESSAGE_COUNT + MESSAGE_BUFFER_COUNT);
    }
public:
    static Logger& get() {
        static Logger instance;
        return instance;
    }

    // Variadic template ile doğrudan format desteği
    template <typename... Args>
    void log(LogLevel level, fmt::format_string<Args...> format, Args&&... args) {
        std::string message = fmt::format(format, std::forward<Args>(args)...);

        std::string prefix;
        auto color = fmt::fg(fmt::color::white);
        auto bold = fmt::emphasis::bold;

        switch (level) {
        case LogLevel::Trace:    color = fmt::fg(fmt::color::gray);      prefix = "[Trace   ] "; break;
        case LogLevel::Debug:    color = fmt::fg(fmt::color::cyan);      prefix = "[Debug   ] "; break;
        case LogLevel::Info:     color = fmt::fg(fmt::color::white);     prefix = "[Info    ] "; break;
        case LogLevel::Warning:  color = fmt::fg(fmt::color::yellow);    prefix = "[Warning ] "; break;
        case LogLevel::Error:    color = fmt::fg(fmt::color::red);       prefix = "[Error   ] "; break;
        case LogLevel::Success:  color = fmt::fg(fmt::color::lime_green);prefix = "[Success ] "; break;
        case LogLevel::Critical: color = fmt::fg(fmt::color::magenta);   prefix = "[Critical] "; break;
        }

        fmt::print(bold | color, "{}", prefix);
        fmt::print(color, "{}\n", message );
        
        messages.push_back(prefix + message);
        if (messages.size() >= MAX_MESSAGE_COUNT) 
            messages.erase(messages.begin(), messages.begin() + MESSAGE_BUFFER_COUNT);
        
    }

    const std::vector<std::string>& getMessages() const {
        return messages;
    }

};
