#pragma once
#include "easyTest.h"
#include <Alime/base/json/aJson.h>

TEST_UNIT(AlimeJson_Test_AlimeJsonValue)
{
    AlimeJsonValue jsonValue;
    jsonValue.value_.string_ = new std::string("hello");
    jsonValue.type_ = JsonType::JSON_STRING;
    AlimeJsonValue copy = jsonValue;
    ASSERT_TRUE(copy.type_ == JsonType::JSON_STRING);
    ASSERT_TRUE(*copy.value_.string_ == *jsonValue.value_.string_);
    ASSERT_TRUE(*copy.value_.string_ == "hello");
}

TEST_UNIT(AlimeJson_Test_JSON_STRING)
{
    {
        bool isSpace=Alime::base::details::IsWhitespace('\0');
        isSpace = Alime::base::details::IsWhitespace('\r');
        isSpace = Alime::base::details::IsWhitespace('\n');
    }
    {
        AlimeJson aj;
        std::string str = R"("hello world")";
        aj = AlimeJson::Parse(str.c_str());
        ASSERT_TRUE(aj.GetType() == JsonType::JSON_STRING);
    }
    {
        AlimeJson aj;
        std::string str = R"("hello")";
        aj = AlimeJson::Parse(str.c_str());
        ASSERT_TRUE(aj.GetType() == JsonType::JSON_STRING);
    }
    {
        AlimeJson aj;
        std::string str = R"("hello world")";
        aj = AlimeJson::Parse(str.c_str());
        ASSERT_TRUE(aj.GetType() == JsonType::JSON_STRING);
    }
    {
        AlimeJson aj;
        std::string str = R"("   hello    ")";
        aj = AlimeJson::Parse(str.c_str());
        ASSERT_TRUE(aj.GetType() == JsonType::JSON_STRING);
    }
    {
        AlimeJson aj;
        std::string str = R"("hello 
 world
")";
        aj = AlimeJson::Parse(str.c_str());
        ASSERT_TRUE(aj.GetType() == JsonType::JSON_STRING);
    }
}

TEST_UNIT(AlimeJson_Test_NULL)
{
    {
        AlimeJson aj;
        std::string str = R"(null)";
        aj = aj.Parse(str.c_str());
        auto ty = aj.GetType();
        ASSERT_TRUE(ty == JsonType::JSON_NULL);
    }
    {
        AlimeJson aj;
        std::string str = R"( null)";
        aj = aj.Parse(str.c_str());
        auto ty = aj.GetType();
        ASSERT_TRUE(ty == JsonType::JSON_NULL);
    }
    {
        AlimeJson aj;
        std::string str = R"(   null)";
        aj = aj.Parse(str.c_str());
        auto ty = aj.GetType();
        ASSERT_TRUE(ty == JsonType::JSON_NULL);
    }
    {
        AlimeJson aj;
        std::string str = R"(     null      )";
        aj = aj.Parse(str.c_str());
        auto ty = aj.GetType();
        ASSERT_TRUE(ty == JsonType::JSON_NULL);
    }
    {
        AlimeJson aj;
        try {
            std::string str = R"(     null     g )";
            aj = AlimeJson::Parse(str.c_str());
        }
        catch (...)
        {
            ASSERT_TRUE(aj.GetType() == JsonType::JSON_UNKNOW);
        }
        

    }
    {
        AlimeJson aj;
        try {
            std::string str = R"(     n u l l )";
            aj = aj.Parse(str.c_str());
        }
        catch (...)
        {
            auto ty = aj.GetType();
            ASSERT_TRUE(ty == JsonType::JSON_UNKNOW);
        }

    }
}

TEST_UNIT(AlimeJson_Test_BOOL)
{
    {
        AlimeJson aj;
        std::string str = R"(true)";
        aj = aj.Parse(str.c_str());
        auto ty = aj.GetType();
        ASSERT_TRUE(ty == JsonType::JSON_TRUE);
    }
    {
        AlimeJson aj;
        std::string str = R"(           true )";
        aj = aj.Parse(str.c_str());
        auto ty = aj.GetType();
        ASSERT_TRUE(ty == JsonType::JSON_TRUE);
    }
    {
        AlimeJson aj;
        std::string str = R"(           true        )";
        aj = aj.Parse(str.c_str());
        auto ty = aj.GetType();
        ASSERT_TRUE(ty == JsonType::JSON_TRUE);
    }
    {
        AlimeJson aj;
        std::string str = R"(           true       a )";
        try {
            aj = aj.Parse(str.c_str());
        }
        catch (...) {

        }

        auto ty = aj.GetType();
        ASSERT_FALSE(ty == JsonType::JSON_TRUE);
        ASSERT_TRUE(ty == JsonType::JSON_UNKNOW);
    }
    {
        AlimeJson aj;
        std::string str = R"(    b       true        )";
        try {
            aj = aj.Parse(str.c_str());
        }
        catch (...) {

        }
        auto ty = aj.GetType();
        ASSERT_FALSE(ty == JsonType::JSON_TRUE);
        ASSERT_TRUE(ty == JsonType::JSON_UNKNOW);
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
        std::string str = R"( false )";
        aj = aj.Parse(str.c_str());
        auto ty = aj.GetType();
        ASSERT_TRUE(ty == JsonType::JSON_FALSE);
    }

    {
        AlimeJson aj;
        std::string str = R"( false   b )";
        
        //H_EXPECT_ANY_THROW(aj = aj.Parse(str.c_str()));
        try {
            aj = aj.Parse(str.c_str());
        }
        catch (...) {

        }
        auto ty = aj.GetType();
        //ASSERT_TRUE(ty == JsonType::JSON_FALSE);
        ASSERT_FALSE(ty == JsonType::JSON_FALSE);
        ASSERT_TRUE(ty == JsonType::JSON_UNKNOW);
    }
}

TEST_UNIT(AlimeJson_Test_NUMBER)
{
    {
        AlimeJson aj;
        std::string str = R"(3.14125)";
        aj = aj.Parse(str.c_str());
        auto ty = aj.GetType();
        ASSERT_TRUE(ty == JsonType::JSON_NUMBER);
    }
    {
        AlimeJson aj;
        std::string str = R"([3,4,5,6,7])";
        aj = aj.Parse(str.c_str());
        auto ty = aj.GetType();
        ASSERT_TRUE(ty == JsonType::JSON_ARRAY);
    }
    {
        AlimeJson aj;
        std::string str = R"([3,[4,5,6,7],6,7])";
        aj = aj.Parse(str.c_str());
        auto ty = aj.GetType();
        ASSERT_TRUE(ty == JsonType::JSON_ARRAY);
    }
}