//#ifndef TIMER_HPP
//#define TIMER_HPP
#pragma once

#include <chrono>

namespace server_common {
    
class Timer {
public:
    Timer() {};
    static int64_t now() {
        using namespace std::chrono;
        return duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
    }
};

}

//#endif