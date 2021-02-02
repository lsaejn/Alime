#pragma once
#include "easyTest.h"
#include <Alime/base/json/aJson.h>
#include <Alime/base/strings/string_conversions.h>

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

    {
        wchar_t* unicode =L"\u941c";
        char* u8 = "\xe9\x90\x9c";//
        char* gbk = "\xe7\x8e";//这个字符不能保存
        auto wString=Alime::base::SysUTF8ToWide(u8);
        auto gbkString = Alime::base::SysWideToNativeMB(wString);
        auto u8String = Alime::base::UTF16ToUTF8(unicode);

        bool ret=gbkString == (std::string)gbk;
        ret=u8String == (std::string)u8;
        ret=ret == true;
    }   
    {
        //wchar_t* unicode = L"\uD834\uDD1E";// we can previews is 𝄞, which but can not speci here
        char *gbk = "𝄞";//
        wchar_t* wTest1 = L"𝄞";
        char* utf8char=u8"𝄞";
        //char* utf8 = "\xD8\x34\xDD\x1E";//F0 9D 84 9E
        char* utf8 = "\xF0\x9D\x84\x9E";

        auto u8String = Alime::base::SysWideToUTF8(wTest1);
        auto wString = Alime::base::SysUTF8ToWide(utf8char);
        auto str = Alime::base::SysWideToNativeMB(wString);
        //auto str2 = Alime::base::UTF8ToUTF16(test1);

        //wchar_t *test2w=L"\uD834\uDD1E";
        const char* test4 = "𝄞";

        {
            const char* you = "\u4f60\u597d";//\u一个码点
            const char* y = "\u00A2";//¢
            const char* y2 = "\u5784";
        }
        {
            const char* u8Str = "\x4F\x60\x59\x7D";
            auto wstr= Alime::base::UTF8ToUTF16(u8Str);
            auto wstr2 = Alime::base::UTF8ToUTF16(u8Str);
        }
        {
            

            //unsigned unicode = 0;
            //func("0024", &unicode);
            //unicode = 0;

        }

    }
}

TEST_UNIT(AlimeJson_Test_Unicode)
{
    //概念
    {
        wchar_t* w_hello = L"你好";
        wchar_t* w_hello2 = L"\u4f60\u597d";//一个字符的unicode编码是确定的

        char* hello = "\xC4\xE3\xBA\xC3";
        char hello2[5] = { 0 }; hello2[0] = 0xC4; hello2[1] = 0xE3; hello2[2] = 0xBA; hello2[3] = 0xC3;
        bool eq = strcmp(hello, hello2);
        //auto u8Hello = Alime::base::UTF16ToUTF8(hello);
        //auto u8Ansi = Alime::base::SysWideToNativeMB(hello);
    }
}