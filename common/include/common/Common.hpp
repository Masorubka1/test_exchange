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
    static cppkafka::Configuration config_order = {
        { "metadata.broker.list", "127.0.0.1:9092" }, {"group.id","order"}, {"topic.metadata.refresh.sparse", true}
    };
    static cppkafka::Configuration config_user = {
        { "metadata.broker.list", "127.0.0.1:9092" }, {"group.id","user"}
    };
}

//#endif //CLIENSERVERECN_COMMON_HPP
