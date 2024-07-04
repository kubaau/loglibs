#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include <memory>

#include "Thread.hpp"

#define TRACE_LOG SPDLOG_TRACE
#define DEBUG_LOG SPDLOG_DEBUG
#define INFO_LOG SPDLOG_INFO
#define WARN_LOG SPDLOG_WARN
#define ERROR_LOG SPDLOG_ERROR
#define CRITICAL_LOG SPDLOG_CRITICAL

#define FUNCTION_LOG DEBUG_LOG("");
#define DEBUG_VAR(var) DEBUG_LOG(#var "={}", var);

namespace SpdLog
{
    constexpr auto full_pattern = "[%a %d.%m.%Y %T.%F UTC%z] [%P%(:%t%]%)] [%^%l%$] [%s:%#:%!]\t%v";
    constexpr auto short_pattern = "%T.%f %s:%#:%! %^%l%$\t%v";
    constexpr auto tsl_pattern = "%T.%F/%^%l%$/%)/%s:%#/%!:\t%v";
    constexpr auto default_spdlog_pattern = "%+";
    constexpr auto default_pattern = short_pattern;

    class ProcessThreadNameFormatterFlag : public spdlog::custom_flag_formatter
    {
        using F = std::function<std::string(std::string)>;

    public:
        ProcessThreadNameFormatterFlag(F f) : f(std::move(f)) {}

        void format(const spdlog::details::log_msg&, const tm&, spdlog::memory_buf_t& dest) override
        {
            auto txt = f(std::string{});
            if (not txt.empty())
                txt.insert(0, 1, '/');
            dest.append(txt.data(), txt.data() + txt.size());
        }

        std::unique_ptr<custom_flag_formatter> clone() const override
        {
            return std::make_unique<ProcessThreadNameFormatterFlag>(f);
        }

    private:
        F f;
    };

    inline void logToConsoleAndFile(const std::string& filename)
    {
        using namespace spdlog;
        using namespace sinks;
        using namespace std;

        constexpr auto truncate = true;
        set_default_logger(make_unique<logger>(
            "multi_sink",
            sinks_init_list{make_shared<stderr_color_sink_mt>(), make_shared<basic_file_sink_mt>(filename, truncate)}));
    }

    inline void setPattern(const std::string& pattern)
    {
        using namespace spdlog;
        using namespace std;

        auto formatter = make_unique<pattern_formatter>();
        formatter->add_flag<ProcessThreadNameFormatterFlag>('(', processName).
                   add_flag<ProcessThreadNameFormatterFlag>(')', threadName).
                   add_flag<ProcessThreadNameFormatterFlag>(']', threadHexId).
                   set_pattern(pattern);
        set_formatter(move(formatter));
    }

    inline void initLogging(const std::string& filename = "", const std::string& pattern = default_pattern)
    {
        using namespace spdlog;
        
        if (not filename.empty())
            logToConsoleAndFile("log.txt");
#ifdef _DEBUG
        set_level(level::debug);
#endif
        setPattern(pattern);
        threadName("main");
    }
}
