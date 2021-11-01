#pragma once

#include <experimental/source_location>
#include <fmt/core.h>
#include <string_view>

namespace lg
{

class Logger
{
    struct format_string
    {
        using source_location = std::experimental::source_location;

        std::string_view str;
        source_location loc;

        format_string(const char* str, const source_location& loc = source_location::current()) : str(str), loc(loc) {}
    };

    // friends
    template <typename... Args>
    friend void info(const format_string& format, Args&&...);

    template <typename... Args>
    friend void error(const format_string& format, Args&&...);

  public:
    Logger(Logger const&) = delete;
    void operator=(Logger const&) = delete;

    static Logger& getLogger()
    {
        static Logger instance;
        return instance;
    }

  private:
    enum class LogLevel
    {
        ERROR,
        INFO
    };

    Logger() = default;

    template <typename... Args>
    void log_impl(LogLevel log_level, const format_string& format, Args&&... args)
    {
        if (LogLevel::ERROR == log_level)
        {
            const auto& loc = format.loc;
            fmt::print("\nError: {}:{}: ", loc.file_name(), loc.line());
        }
        fmt::vprint(format.str, fmt::make_format_args(args...));
    }
};

template <typename... Args>
void error(const Logger::format_string& format, Args&&... args)
{
    Logger::getLogger().log_impl(Logger::LogLevel::ERROR, format, std::forward<Args>(args)...);
}

template <typename... Args>
void info(const Logger::format_string& format, Args&&... args)
{
    Logger::getLogger().log_impl(Logger::LogLevel::INFO, format, std::forward<Args>(args)...);
}

}; // namespace lg
