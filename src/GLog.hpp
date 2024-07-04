#pragma once

#include <glog/logging.h>

#include <iomanip>

inline void setPrefix(std::ostream& s, const google::LogMessage& l, void*)
{
    using namespace std;

    switch (l.time().dayOfWeek())
    {
        case 1: s << "[Mon "; break;
        case 2: s << "[Tue "; break;
        case 3: s << "[Wed "; break;
        case 4: s << "[Thu "; break;
        case 5: s << "[Fri "; break;
        case 6: s << "[Sat "; break;
        default: s << "[Sun "; break;
    }
    s << setw(2) << l.time().day() << "." << setw(2) << l.time().month() << "." << setw(4) << 1900 + l.time().year()
      << " " << setw(2) << l.time().hour() << ':' << setw(2) << l.time().min() << ':' << setw(2) << l.time().sec()
      << "." << setw(6) << l.time().usec() << "] [" << getpid() << ":" << l.thread_id() << "] [" << l.severity()
      << "] [" << l.basename() << ":" << l.line() << "]\t";
}
