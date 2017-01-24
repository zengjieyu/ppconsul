//  Copyright (c) 2014-2017 Andrey Upadyshev <oliora@gmail.com>
//
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "ppconsul/parameters.h"
#include <catch/catch.hpp>


namespace {
    enum class Enum { Default, Val1, Val2 };
}

namespace test_params {
    PPCONSUL_PARAM(p1, std::string)
    PPCONSUL_PARAM(p2, uint64_t)
    PPCONSUL_PARAM_NAMED(p3, bool, "boolean")
    PPCONSUL_PARAM_NO_NAME(p4, Enum)

    inline void printParameter(std::ostream& os, Enum v, KWARGS_KW_TAG(p4))
    {
        switch (v)
        {
            case Enum::Val1: os << "val1"; break;
            case Enum::Val2: os << "val2"; break;
            default: break;
        }
    }
}


TEST_CASE("makeQuery", "[parameters]")
{
    using ppconsul::parameters::makeQuery;
    using ppconsul::parameters::makeUrl;

    using namespace test_params;

    CHECK(makeQuery() == "");

    CHECK(makeUrl("http://www.example.com", "/something/interesting") ==
        "http://www.example.com/something/interesting");

    CHECK(makeUrl("http://www.example.com", "/something/interesting", p1 = "") ==
        "http://www.example.com/something/interesting");

    CHECK(makeUrl("http://www.example.com", "/something/interesting", p1 = "bla", p2 = 99, p1 = "wow") ==
        "http://www.example.com/something/interesting?p2=99&p1=wow");

    CHECK(makeUrl("http://www.example.com", "/something/interesting", p1 = "bla", p2 = 99, p1 = "") ==
        "http://www.example.com/something/interesting?p2=99");

    CHECK(makeUrl("http://www.example.com", "/something/interesting", p3 = true) ==
        "http://www.example.com/something/interesting?boolean=1");

    CHECK(makeUrl("http://www.example.com", "/something/interesting", p3 = false) ==
        "http://www.example.com/something/interesting?boolean=0");

    CHECK(makeUrl("http://www.example.com", "/something/interesting", p3 = false, p3 = true) ==
        "http://www.example.com/something/interesting?boolean=1");

    CHECK(makeUrl("http://www.example.com", "/something/interesting", p4 = Enum::Val1) ==
        "http://www.example.com/something/interesting?val1");

    CHECK(makeUrl("http://www.example.com", "/something/interesting", p4 = Enum::Default) ==
        "http://www.example.com/something/interesting");

    CHECK(makeUrl("http://www.example.com", "/something/interesting", p4 = Enum::Default, p1 = "bla") ==
        "http://www.example.com/something/interesting?p1=bla");

    CHECK(makeUrl("http://www.example.com", "/something/interesting", p2=2, p4 = Enum::Val1, p3 = true, p1 = "bla") ==
        "http://www.example.com/something/interesting?p2=2&val1&boolean=1&p1=bla");
}
