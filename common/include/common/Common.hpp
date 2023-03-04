//#ifndef CLIENSERVERECN_COMMON_HPP
//#define CLIENSERVERECN_COMMON_HPP
#pragma once

#include <string>

static short port = 5555;

namespace Requests
{
    static std::string Registration = "Reg";
    static std::string Hello = "Hel";
    static std::string Close = "Close";
    static std::string Transaction = "Tran";
}

//#endif //CLIENSERVERECN_COMMON_HPP
