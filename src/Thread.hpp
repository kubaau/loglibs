#pragma once

#include <sstream>
#include <thread>

inline auto threadHexId(std::string = {})
{
    using namespace std;
    thread_local string ret;
    if (ret.empty())
    {
        ostringstream oss;
        oss << "0x" << hex << this_thread::get_id();
        ret = oss.str();
    }
    return ret;
}

inline auto& replaceIfNotEmpty(std::string& str, std::string opt_str)
{
    if (not opt_str.empty())
        str = opt_str;
    return str;
}

inline auto threadName(std::string name = {})
{
    thread_local std::string ret;
    return replaceIfNotEmpty(ret, name);
}

inline auto processName(std::string name = {})
{
    static std::string ret;
    return replaceIfNotEmpty(ret, name);
}
