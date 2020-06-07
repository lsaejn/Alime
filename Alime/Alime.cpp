// Alime.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "Alime/base/strings/string_util.h"
#include "Alime/base/details/conv.h"
using namespace Alime::base;

int main()
{
    //const char* ptr = nullptr;
    //std::string_view sv(ptr);
    std::wstring_view ws = L"    123ABC      ";
    auto re= Alime::base::StringTrimLeft(ws);
    re= Alime::base::StringTrimRight(re);

    std::wstring_view ws2 = L"123ABC";
    bool eq = re == ws2;
    ws.remove_prefix(3);
    auto v = ws.substr();
    //Alime::base::StringTrim<wchar_t>(L"123ABC");
   // Alime::base::LowerString<wchar_t>(ws.c_str());
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
        std::wstring wresult = Alime::base::Join(L",", L"你", L"就", L"是",-1, L"个", L"白痴");
        std::wcout << wresult << std::endl;
    }
    {

    }
    std::cout << "Hello World!\n";
}
