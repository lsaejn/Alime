#pragma once
#include "easyTest.h"
#include <Alime/base/json/aJson.h>

TEST_UNIT(AlimeJson_Test)
{
    {
        AlimeJson aj;
        std::string str = R"("hello")";
        aj = aj.Parse(str.c_str());
        auto ty = aj.GetType();
        ASSERT_TRUE(ty==JsonType::JSON_STRING);
    }
    {
        AlimeJson aj;
        std::string str = R"(null)";
        aj = aj.Parse(str.c_str());
        auto ty = aj.GetType();
        ASSERT_TRUE(ty == JsonType::JSON_NULL);
    }
    {
        AlimeJson aj;
        std::string str = R"(false)";
        aj = aj.Parse(str.c_str());
        auto ty = aj.GetType();
        ASSERT_TRUE(ty == JsonType::JSON_FALSE);
    }
    {
        AlimeJson aj;
        std::string str = R"(true)";
        aj = aj.Parse(str.c_str());
        auto ty = aj.GetType();
        ASSERT_TRUE(ty == JsonType::JSON_TRUE);
    }
}