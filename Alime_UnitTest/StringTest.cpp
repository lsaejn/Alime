#include "easyTest.h"
#include <string_view>

#include <Alime/base/strings/string_util.h>
#include <Alime/base/strings/string_util.cpp>

using namespace Alime::base;

TEST_UNIT(test_CharCompareNoCase)
{
	H_TEST_ASSERT(Alime::base::details::CharCompareNoCase('a', 'b')<0);
	H_TEST_ASSERT(Alime::base::details::CharCompareNoCase('A', 'B')<0);
	H_TEST_ASSERT(Alime::base::details::CharCompareNoCase('a', 'B')<0);
	H_TEST_ASSERT(Alime::base::details::CharCompareNoCase('c', 'b')>0);
	H_TEST_ASSERT(Alime::base::details::CharCompareNoCase('c', 'B')>0);
	H_TEST_ASSERT(Alime::base::details::CharCompareNoCase('C', 'B')>0);
	H_TEST_ASSERT(Alime::base::details::CharCompareNoCase('C', 'b')>0);
	H_TEST_ASSERT(Alime::base::details::CharCompareNoCase('Z', 'B')>0);
	H_TEST_ASSERT(Alime::base::details::CharCompareNoCase('Z', 'z')==0);
	H_TEST_ASSERT(Alime::base::details::CharCompareNoCase('Z', 'Z')==0);
}

TEST_UNIT(test_StringCompareNoCase)
{
	std::string_view sv1("fuck");
	std::string_view sv2("Fuck");
	H_TEST_ASSERT(0 == Alime::base::details::StringCompareNoCase(sv1, sv2));
	std::string_view sv3("FUCK");
	H_TEST_ASSERT(0 == Alime::base::details::StringCompareNoCase(sv1, sv3));
	std::string_view sv4("EggPains");
	H_TEST_ASSERT(1 == Alime::base::details::StringCompareNoCase(sv1, sv4));
	std::string_view sv5("WJY is a Genius");
	H_TEST_ASSERT(-1 == Alime::base::details::StringCompareNoCase(sv1, sv5));

	std::string_view sv6("WJY IS A GENIUS");
	H_TEST_ASSERT(0 == Alime::base::details::StringCompareNoCase(sv6, sv5));
	//we uses string_view, and it cause some imperceptible error
	std::string_view sv7("en..., WJY IS A GENIUS.....");
	H_TEST_ASSERT(-1== Alime::base::details::StringCompareNoCase(sv7, sv6));
	std::string_view sv8=sv7.substr(7, sv6.length());
	H_TEST_ASSERT(0 == Alime::base::details::StringCompareNoCase(sv8, sv6));
	//我们截取sv7后面一段
	std::string_view sv9 = sv7.substr(7);
	H_TEST_ASSERT(1 == Alime::base::details::StringCompareNoCase(sv9, sv6));
	std::string_view sv10 = sv7.substr(0, sv7.length()-7);
	H_TEST_ASSERT(-1 == Alime::base::details::StringCompareNoCase(sv10, sv7));

}

TEST_UNIT(testAny1)
{
	auto re=LowerString("fuckPFF你");
	H_TEST_EQUAL(re, "fuckpff你");
}


TEST_UNIT(test_Join)
{
	auto b = to<std::string>("hello");
	auto r=toDelim<std::string>(",", "aa", "bb");
	auto re = Join(",", "hello", "world");
	H_TEST_EQUAL(re, "hello,world");

	auto re1 = Alime::base::Join("", "a", "new", "file");
	H_TEST_EQUAL(re1, "helloworld");
}

/// <summary>
/// need to be very careful about string_view::data()
/// </summary>
TEST_UNIT(test_string_view)
{
	std::string_view sv = "fuck off";
	auto subSv = sv.substr(0,5);
	std::string str1(sv);
	std::string str2(subSv);
	H_EXPECT_TRUE(str1!=str2);

	std::string str3(sv.data());
	std::string str4(subSv.data());
	H_TEST_EQUAL(str3, str4);
}

