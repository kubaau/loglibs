#include <signal.h>
#include <spdlog/fmt/chrono.h>
#include <spdlog/stopwatch.h>

#include "BoostLog.hpp"
#include "GLog.hpp"

//#define _DEBUG
#include "SpdLog.hpp"

using namespace std;

namespace
{
    string foo()
    {
        FUNCTION_LOG;
        return __FUNCTION__;
    }
}

void testSpdLog()
{
    using namespace SpdLog;
    using namespace spdlog;

    initLogging("log.txt");
    INFO_LOG("Welcome to spdlog!");
    setPattern(full_pattern);
    ERROR_LOG("Some error message with arg: {}", 1);
    setPattern(tsl_pattern);
    WARN_LOG("Easy padding in numbers like {:08d}", 12);
    setPattern(short_pattern);
    CRITICAL_LOG("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    INFO_LOG("Support for floats {:03.2f}", 1.23456);
    INFO_LOG("Support for floats {}", 1.23456);
    INFO_LOG("Positional args are {1} {0}..", "too", "supported");
    INFO_LOG("{:<30}", "left aligned");

    DEBUG_LOG("This message may not be displayed..");
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    DEBUG_LOG("This message should be displayed..");

    // change log pattern
    // spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");
    // DEBUG_LOG("This message should be displayed..");
    // 23:01:10.8332988/DBG/eNB2/NetworkTask.cpp:59/networkTask:       Ending task
    // spdlog::set_pattern("[%a %d.%m.%Y %T.%F UTC%z] [%P:%t] [%^%l%$] [%s:%#:%!]\t%v");
    // DEBUG_LOG("This message should be displayed..");

    // Compile time log levels
    // define SPDLOG_ACTIVE_LEVEL to desired level
    SPDLOG_TRACE("Some trace message with param {}", 42);
    SPDLOG_DEBUG("Some debug message");

    setPattern(full_pattern);
    vector<thread> ts(10);
    for (auto& t : ts)
        t = thread(
            [&]
            {
                for (int j = 0; j < 10; ++j)
                    DEBUG_LOG("{}", j);
            });

    for (auto& t : ts)
        t.join();

    setPattern(default_spdlog_pattern);

    DEBUG_LOG("{}", foo());

    auto xyz = 123;
    DEBUG_VAR(xyz);

    bool b = true;
    if (b)
        DEBUG_LOG("aaa");
    else
        b = false;
    assert(b);

    setPattern(full_pattern);
    spdlog::debug("lol");

    stopwatch sw;
    std::this_thread::sleep_for(std::chrono::milliseconds(123));
    INFO_LOG("Stopwatch: {} seconds", sw);

    processName("main");
    const auto fork_result = fork();
    if (fork_result)
        DEBUG_LOG("parent");
    else
    {
        DEBUG_LOG("child");
        processName("child");
    }
    DEBUG_LOG("both");
    if (fork_result)
        kill(fork_result, SIGKILL);
    
    DEBUG_LOG("{}", false);
    DEBUG_LOG("{:d}", false);
}

void testGLog([[maybe_unused]] char* argv[])
{
    FLAGS_log_dir = "./logs";
    google::InitGoogleLogging(argv[0]);
    google::InstallPrefixFormatter(setPrefix);
    FLAGS_stderrthreshold = 0;
    LOG(INFO) << "Found " << 2 << " cookies";
    LOG(ERROR) << "err";
}

void testBoostLog()
{
    boost::log::add_console_log(cerr);
    boost::log::add_file_log("boost.log");
    boost::log::add_common_attributes();
    BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
    BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
    BOOST_LOG_TRIVIAL(info) << "An informational severity message";
    BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
    BOOST_LOG_TRIVIAL(error) << "An error severity message";
    BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";
}
