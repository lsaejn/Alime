// Alime.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "Alime/base/strings/string_util.h"
#include "Alime/base/details/conv.h"
#include "Alime/base/strings/string_conversions.h"
#include "assert.h"
using namespace Alime::base;

int main()
{
    {
        //1
        std::string_view sv = u8"hello你";
        std::wstring re = UTF8ToUTF16(sv);
        assert(re == L"hello你");
        //2
        std::wstring_view wsv = re;
        std::string str = UTF16ToUTF8(wsv);
        assert(str == u8"hello你");
        //3
        std::u32string u32str = UTF16ToUTF32(wsv);
        assert(u32str == U"hello你");
        //4
        std::u32string u32str2 = UTF8ToUTF32(sv);
        assert(u32str2 == U"hello你");
        //5
        std::string u8str = UTF32ToUTF8(u32str2);
        assert(u8str == u8"hello你");
        //6
        std::wstring wstr = UTF32ToUTF16(u32str2);
        assert(wstr == L"hello你");
        //7
        std::wstring wide_str = L"你tmd";
        auto u8_str=SysWideToUTF8(wide_str);
        assert(u8_str == u8"你tmd");
        //8
        std::wstring wide_str2 = SysUTF8ToWide(u8_str);
        assert(wide_str2 == L"你tmd");
        //9
        std::string ansiStr=SysWideToNativeMB(wide_str2);
        assert(ansiStr == "你tmd");
        //10
        std::wstring wide_str3=SysNativeMBToWide(ansiStr);
        assert(wide_str3 == L"你tmd");

    }
    auto result = Alime::base::StringTrimLeft(L"    123ABC      ");
    std::wstring str= L"    123ABC      ";
    auto result2 = Alime::base::StringTrimLeft(str);
    //const char* ptr = nullptr;
    //std::string_view sv(ptr);
    std::wstring_view ws = L"    123ABC      ";
    auto re= Alime::base::StringTrimLeft(ws);
    re= Alime::base::StringTrimRight(re);

    std::wstring_view ws2 = L"123ABC";
    bool eq = re == ws2;
    ws.remove_prefix(3);
    auto v = ws.substr();
    auto afterTrim=Alime::base::StringTrim(L"   123ABC   ");
    afterTrim=Alime::base::LowerString(afterTrim);
    //auto res = ws + ws2;
    {
        std::string_view sv("fuckoff");
        auto result=Alime::base::TrimEnd(sv, "mn");
        std::cout << result << std::endl;

        std::string_view sv1("  fuck  off");
        auto result1 = Alime::base::TrimEnd(sv1, "fof");
        result1 = Alime::base::TrimStart(Alime::base::TrimEnd(result1));
        std::cout << result1 << std::endl;
    }
    {
        std::wstring_view sv(L"你妈了个fuckoff");
        bool re = Alime::base::StartsWith(sv, L"你妈");
        std::cout << re << std::endl;
    }
    {
        std::wstring_view sv(L"你a妈了个fuckoff");
        bool re = Alime::base::StartsWith(sv, L"你A妈",false);
        std::cout << re << std::endl;
        re = Alime::base::StartsWith(sv, L"你A妈", true);
        std::cout << re << std::endl;
    }
    {//EndsWith
        std::wstring_view sv(L"你a妈了个fuckoff哦");
        bool re = Alime::base::EndsWith(sv, L"F哦", false);
        std::cout << re << std::endl;
        re = Alime::base::EndsWith(sv, L"F哦", true);
        std::cout << re << std::endl;
    }
    {
        std::string_view sv1 = "fuckoff";
        std::string_view sv2 = "fuck";
        int re = sv1.compare(sv2);
        re += 1;
    }
    {
        auto ret=Alime::base::Compare(L"你好", L"你坏",1);
        std::cout << ret << std::endl;
    }
    {
        std::string result= Alime::base::Join(",", "你", "就", "是", "个", "白痴");
        result = toDelim<std::string>(",", "你", "就", "是", -1, "个", "白痴");
        result = toDelim<std::string>(",", "你", "就", "是", 2.3456789, "个", "白痴");
        result = toDelim<std::string>(",", "you", "like", "her");
        std::cout << result << std::endl;
    }
    {
        std::wstring wresult = Alime::base::Join(L",", L"你", L"就", L"是", -2.3456789, L"个", L"白痴");
        wresult = toDelim<std::wstring>(L",", L"你", L"就", L"是", 3.45, 1, L"个", L"白痴");
    }
    {
        //std::u16string u16 = L"fuck";
    }
    std::cout << "Hello World!\n";
}
