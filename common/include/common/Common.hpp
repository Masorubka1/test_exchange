//#ifndef CLIENSERVERECN_COMMON_HPP
//#define CLIENSERVERECN_COMMON_HPP
#pragma once

#include <string>
#include <cppkafka/cppkafka.h>

static short port = 5555;

namespace Requests
{
    static std::string Registration = "Reg";
    static std::string Hello = "Hel";
    static std::string Close = "Close";
    static std::string Transaction = "Tran";
}

namespace conf {
    static cppkafka::Configuration config = {
        { "metadata.broker.list", "127.0.0.1:9092" }
    };
}

//#endif //CLIENSERVERECN_COMMON_HPP
