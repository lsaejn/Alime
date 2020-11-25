#include "easyTest.h"
#include <string_view>

#include <Alime/base/strings/string_util.h>
#include <Alime/base/strings/string_util.cpp>

using namespace Alime::base;

/*
Each test can be divided into three parts: astring, wstring, string_view 
*/


TEST_UNIT(test_Format)
{
	{
		auto re = Format("你就是%d个%s东西", 1, "狗");
		H_TEST_EQUAL(re, "你就是1个狗东西");
		auto re2 = Format("你就是%d个%s东西", 30000222, "SB");
		H_TEST_EQUAL(re2, "你就是30000222个SB东西");
		//so, how to deal longlong now? %ld or %lld. fix me
		auto re3 = Format("你会不会算%d + %f= %d", 1, 2.2, 3);
		H_TEST_EQUAL(re3, "你会不会算1 + 2.200000= 3");
	}
	{
		auto re = Format(L"你就是%d个%s东西", 1, L"狗");
		H_TEST_EQUAL(re, L"你就是1个狗东西");
		auto re2 = Format(L"你就是%d个%s东西", 30000222, L"SB");
		H_TEST_EQUAL(re2, L"你就是30000222个SB东西");
		//so, how to deal longlong now? %ld or %lld. fix me
		auto re3 = Format(L"你会不会算%d + %f= %d", 1, 2.2, 3);
		H_TEST_EQUAL(re3, L"你会不会算1 + 2.200000= 3");
	}
}

TEST_UNIT(test_lowerString)
{
	{
		std::wstring_view sv = L"fuckPFF你";
		H_TEST_EQUAL(LowerString(sv), L"fuckpff你");
		H_TEST_EQUAL(LowerString(L""), L"");
		H_TEST_ASSERT(LowerString(L"") != L"fuck");
		H_TEST_EQUAL(LowerString(L"aabb"), L"aabb");
		H_TEST_EQUAL(LowerString(L"你好吗"), L"你好吗");
	}
	{
		std::string_view sv = "fuckPFF你";
		H_TEST_EQUAL(LowerString(sv), "fuckpff你");
		H_TEST_EQUAL(LowerString(""), "");
		H_TEST_ASSERT(LowerString("") != "fuck");
		H_TEST_EQUAL(LowerString("aabb"), "aabb");
		H_TEST_EQUAL(LowerString("你好吗"), "你好吗");
	}
	{
		std::string_view sv = "fuckPFF你";
		H_TEST_EQUAL(LowerString(sv), "fuckpff你");
		auto sv2 = sv.substr(0, 5);
		H_TEST_ASSERT(LowerString(sv2) != "fuckpff你");
		H_TEST_EQUAL(LowerString(sv2), "fuckp");
		auto sv3 = sv.substr(1, 5);
		H_TEST_EQUAL(LowerString(sv3), "uckpf");
	}
}

TEST_UNIT(test_UpperString)
{
	{
		std::wstring_view sv = L"fuckPFF你";
		H_TEST_EQUAL(UpperString(sv), L"FUCKPFF你");
		H_TEST_EQUAL(UpperString(L""), L"");
		H_TEST_ASSERT(UpperString(L"") != L"FUCK");
		H_TEST_EQUAL(UpperString(L"aabb"), L"AABB");
		H_TEST_EQUAL(UpperString(L"你好吗"), L"你好吗");
	}
	{
		std::string_view sv = "fuckPFF你";
		H_TEST_EQUAL(UpperString(sv), "FUCKPFF你");
		H_TEST_EQUAL(UpperString(""), "");
		H_TEST_ASSERT(UpperString("") != "FUCK");
		H_TEST_EQUAL(UpperString("aabb"), "AABB");
		H_TEST_EQUAL(UpperString("你好吗"), "你好吗");
	}
	{
		std::string_view sv = "fuckPFF你";
		H_TEST_EQUAL(UpperString(sv), "FUCKPFF你");
		auto sv2 = sv.substr(0, 5);
		H_TEST_ASSERT(UpperString(sv2) != "FUCKPFF你");
		H_TEST_EQUAL(UpperString(sv2), "FUCKP");
		auto sv3 = sv.substr(1, 5);
		H_TEST_EQUAL(UpperString(sv3), "UCKPF");
	}
}

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

TEST_UNIT(test_StringTrimLeft)
{
	{
		H_TEST_EQUAL(StringTrimLeft("   你好"), "你好");
		H_TEST_EQUAL(StringTrimLeft("   你好	吗"), "你好	吗");
		H_TEST_EQUAL(StringTrimLeft("   你 好"), "你 好");
		H_TEST_EQUAL(StringTrimLeft("\r\naabbzd你 好"), "aabbzd你 好");
		H_TEST_EQUAL(StringTrimLeft("\r\n   aabbzd你 好"), "aabbzd你 好");
		H_TEST_EQUAL(StringTrimLeft("\r\n   \taabbzd你 好"), "aabbzd你 好");
		H_TEST_EQUAL(StringTrimLeft("\r   \taabbzd你 好"), "aabbzd你 好");
		H_TEST_EQUAL(StringTrimLeft("\n   \taabbzd你 好"), "aabbzd你 好");
	}
	{
		H_TEST_EQUAL(StringTrimLeft("   你好"), "你好");
		H_TEST_EQUAL(StringTrimLeft("   你好	吗"), "你好	吗");
		H_TEST_EQUAL(StringTrimLeft("   你 好"), "你 好");
		H_TEST_EQUAL(StringTrimLeft("\r\naabbzd你 好"), "aabbzd你 好");
		H_TEST_EQUAL(StringTrimLeft("\r\n   aabbzd你 好"), "aabbzd你 好");
		H_TEST_EQUAL(StringTrimLeft("\r\n   \taabbzd你 好"), "aabbzd你 好");
		H_TEST_EQUAL(StringTrimLeft("\r   \taabbzd你 好"), "aabbzd你 好");
		H_TEST_EQUAL(StringTrimLeft("\n   \taabbzd你 好"), "aabbzd你 好");
	}
	{
		std::string_view sv1("Fuck   你好");
		std::string_view sv2 = sv1.substr(4);
		H_TEST_EQUAL(StringTrimLeft(sv2), "你好");	
	}
	{
		std::wstring_view sv1(L"Fuck   你好");
		std::wstring_view sv2 = sv1.substr(4);
		H_TEST_EQUAL(StringTrimLeft(sv2), L"你好");
	}
}

TEST_UNIT(test_StringTrimRight)
{
	{
		H_TEST_EQUAL(StringTrimRight("你好   "), "你好");
		H_TEST_EQUAL(StringTrimRight("你好	吗   "), "你好	吗");
		H_TEST_EQUAL(StringTrimRight("你 好   "), "你 好");
		H_TEST_EQUAL(StringTrimRight("aabbzd你 好\r\n"), "aabbzd你 好");
		H_TEST_EQUAL(StringTrimRight("aabbzd你 好\r\n   "), "aabbzd你 好");
		H_TEST_EQUAL(StringTrimRight("aabbzd你 好\r\n   \t"), "aabbzd你 好");
		H_TEST_EQUAL(StringTrimRight("aabbzd你 好\r   \t"), "aabbzd你 好");
		H_TEST_EQUAL(StringTrimRight("aabbzd你 好\n   \t"), "aabbzd你 好");
	}							 
	{							 
		H_TEST_EQUAL(StringTrimRight("你好   "), "你好");
		H_TEST_EQUAL(StringTrimRight("你好	吗   "), "你好	吗");
		H_TEST_EQUAL(StringTrimRight("你 好   "), "你 好");
		H_TEST_EQUAL(StringTrimRight("aabbzd你 好\r\n"), "aabbzd你 好");
		H_TEST_EQUAL(StringTrimRight("aabbzd你 好\r\n   "), "aabbzd你 好");
		H_TEST_EQUAL(StringTrimRight("aabbzd你 好\r\n   \t"), "aabbzd你 好");
		H_TEST_EQUAL(StringTrimRight("aabbzd你 好\r   \t"), "aabbzd你 好");
		H_TEST_EQUAL(StringTrimRight("aabbzd你 好\n   \t"), "aabbzd你 好");
	}
	{
		H_TEST_EQUAL(StringTrimRight(L"你好   "), L"你好");
		H_TEST_EQUAL(StringTrimRight(L"你好	吗   "), L"你好	吗");
		H_TEST_EQUAL(StringTrimRight(L"你 好   "), L"你 好");
		H_TEST_EQUAL(StringTrimRight(L"aabbzd你 好\r\n"), L"aabbzd你 好");
		H_TEST_EQUAL(StringTrimRight(L"aabbzd你 好\r\n   "), L"aabbzd你 好");
		H_TEST_EQUAL(StringTrimRight(L"aabbzd你 好\r\n   \t"), L"aabbzd你 好");
		H_TEST_EQUAL(StringTrimRight(L"aabbzd你 好\r   \t"), L"aabbzd你 好");
		H_TEST_EQUAL(StringTrimRight(L"aabbzd你 好\n   \t"), L"aabbzd你 好");
	}
	{
		H_TEST_EQUAL(StringTrimRight(L"你好   "), L"你好");
		H_TEST_EQUAL(StringTrimRight(L"你好	吗   "), L"你好	吗");
		H_TEST_EQUAL(StringTrimRight(L"你 好   "), L"你 好");
		H_TEST_EQUAL(StringTrimRight(L"aabbzd你 好\r\n"), L"aabbzd你 好");
		H_TEST_EQUAL(StringTrimRight(L"aabbzd你 好\r\n   "), L"aabbzd你 好");
		H_TEST_EQUAL(StringTrimRight(L"aabbzd你 好\r\n   \t"), L"aabbzd你 好");
		H_TEST_EQUAL(StringTrimRight(L"aabbzd你 好\r   \t"), L"aabbzd你 好");
		H_TEST_EQUAL(StringTrimRight(L"aabbzd你 好\n   \t"), L"aabbzd你 好");
	}
	{
		std::string_view sv1("F 你好    ");
		std::string_view sv2 = sv1.substr(1);
		H_TEST_EQUAL(StringTrimRight(sv2), " 你好");
	}
	{
		std::wstring_view sv1(L"F 你好    ");
		std::wstring_view sv2 = sv1.substr(1);
		H_TEST_EQUAL(StringTrimRight(sv2), L" 你好");
	}
}

TEST_UNIT(test_TrimEnd)
{
	{
		std::string_view sv1("F 你好    ");	
		H_TEST_EQUAL(TrimEnd(sv1, " ") , "F 你好");
		std::string_view sv2("F 你好好好好");
		H_TEST_EQUAL(TrimEnd(sv2, "好"), "F 你");
		std::string_view sv3("F 你好    llllkkk");
		H_TEST_EQUAL(TrimEnd(sv3, "k"), "F 你好    llll");
		std::string_view sv4("F 你好    llllkkk");
		H_TEST_EQUAL(TrimEnd(sv1, " kl"), "F 你好");
	}
	{
		std::wstring_view sv1(L"F 你好    ");
		H_TEST_EQUAL(TrimEnd(sv1, L" "), L"F 你好");
		std::wstring_view sv2(L"F 你好好好好");
		H_TEST_EQUAL(TrimEnd(sv2, L"好"), L"F 你");
		std::wstring_view sv3(L"F 你好    llllkkk");
		H_TEST_EQUAL(TrimEnd(sv3, L"k"), L"F 你好    llll");
		H_TEST_EQUAL(TrimEnd(sv3, L"kl"), L"F 你好    ");
		H_TEST_EQUAL(TrimEnd(sv3, L" kl"), L"F 你好");
		H_TEST_EQUAL(TrimEnd(sv3, L" kl你好"), L"F");
	}
	{
		std::wstring_view sv1(L"FFFF 你好    llllkkk");
		H_TEST_EQUAL(TrimEnd(sv1, L" "), sv1);
		std::wstring_view sv2 = sv1.substr(0,sv1.length()-7);
		H_TEST_EQUAL(TrimEnd(sv2, L" "), L"FFFF 你好");
		std::wstring_view sv3 = sv1.substr(3);		
		H_TEST_EQUAL(TrimEnd(sv3, L"k"), L"F 你好    llll");
		H_TEST_EQUAL(TrimEnd(sv3, L"kl"), L"F 你好    ");
		H_TEST_EQUAL(TrimEnd(sv3, L" kl"), L"F 你好");
		H_TEST_EQUAL(TrimEnd(sv3, L" kl你好"), L"F");
	}
}

TEST_UNIT(test_TrimStart)
{
	{
		std::string_view sv1("    F 你好");
		H_TEST_EQUAL(TrimStart(sv1, " "), "F 你好");
		std::string_view sv2("好好好好F 你");
		H_TEST_EQUAL(TrimStart(sv2, "好"), "F 你");
		std::string_view sv3("gfyusF 你好    llllkkk");
		H_TEST_EQUAL(TrimStart(sv3, "gfyusgyf"), "F 你好    llllkkk");//a trap
		std::string_view sv4("kllk    llllkkkF 你好");
		H_TEST_EQUAL(TrimStart(sv4, " kl"), "F 你好");
	}
	{
		std::wstring_view sv1(L"    F 你好");
		H_TEST_EQUAL(TrimStart(sv1, L" "), L"F 你好");
		std::wstring_view sv2(L"好好好好F 你");
		H_TEST_EQUAL(TrimStart(sv2, L"好"), L"F 你");
		std::wstring_view sv3(L"gf    llllkkkyusF 你好B");
		H_TEST_EQUAL(TrimStart(sv3, L"g"), L"f    llllkkkyusF 你好B");
		H_TEST_EQUAL(TrimStart(sv3, L"gf "), L"llllkkkyusF 你好B");
		H_TEST_EQUAL(TrimStart(sv3, L" klfg"), L"yusF 你好B");
		H_TEST_EQUAL(TrimStart(sv3, L" gflkyusF你好"), L"B");
	}
	{
		std::wstring_view sv1(L"llllkk  kFFFF 你好");
		H_TEST_EQUAL(TrimStart(sv1, L" "), sv1);
		std::wstring_view sv2 = sv1.substr(7);
		H_TEST_EQUAL(TrimStart(sv2, L" lk"), L"FFFF 你好");
		std::wstring_view sv3 = sv1.substr(3);
		H_TEST_EQUAL(TrimStart(sv3, L"lk"), L"  kFFFF 你好");
		H_TEST_EQUAL(TrimStart(sv3, L"kFl "), L"你好");
	}
}

TEST_UNIT(test_TrimStart_SizeN)
{
	{
		std::string_view sv1("FFFF你好");
		H_TEST_EQUAL(TrimStart(sv1, 4), "你好");
		std::string_view sv2("好好好好F 你");
		H_TEST_EQUAL(TrimStart(sv2, 2), "好好好F 你");
		std::string_view sv3("gfyusF 你好    llllkkk");
		H_TEST_EQUAL(TrimStart(sv3, 100), "");//a trap
		std::string_view sv4("");
		H_TEST_EQUAL(TrimStart(sv4, 30), "");
	}
	{
		std::wstring_view sv1(L"FFFF你好");
		H_TEST_EQUAL(TrimStart(sv1, 4), L"你好");
		H_TEST_EQUAL(TrimStart(sv1, 5), L"好");
		H_TEST_EQUAL(TrimStart(sv1, 6), L"");
		H_TEST_EQUAL(TrimStart(sv1, 7), L"");
		H_TEST_EQUAL(TrimStart(sv1, 8887), L"");
		H_TEST_EQUAL(TrimStart(sv1, -1), L"");
		H_TEST_EQUAL(TrimStart(sv1, 0), sv1);
	}
	{
		std::wstring_view sv1(L"FFFF你好");
		H_TEST_EQUAL(TrimStart(sv1, 0), sv1);
		std::wstring_view sv2 = sv1.substr(2);
		H_TEST_EQUAL(TrimStart(sv2, 2), L"你好");
		std::wstring_view sv3 = sv1.substr(5);
		H_TEST_EQUAL(TrimStart(sv3, 1), L"");
	}
}

TEST_UNIT(test_Concat)
{
	{
		std::string_view sv1("F 你好");
		H_TEST_EQUAL(Concat(sv1, "GGGGGgg"), "F 你好GGGGGgg");
		H_TEST_EQUAL(Concat(sv1, ""), sv1);
		H_TEST_EQUAL(Concat("", sv1), sv1);
		H_TEST_EQUAL(Concat(sv1, sv1), std::string(sv1)+ std::string(sv1));
	}
	{
		std::wstring_view sv1(L"FFFF你好");
		H_TEST_EQUAL(Concat(sv1, L"GGGGGgg"), L"FFFF你好GGGGGgg");
		H_TEST_EQUAL(Concat(sv1, L""), sv1);
		H_TEST_EQUAL(Concat(L"", sv1), sv1);
		H_TEST_EQUAL(Concat(sv1, sv1), std::wstring(sv1) + std::wstring(sv1));
	}
	{
		std::wstring_view sv(L"FFFF你好");
		std::wstring_view sv1 = sv.substr(2);
		H_TEST_EQUAL(Concat(sv1, L"GGGGGgg"), L"FF你好GGGGGgg");
		H_TEST_EQUAL(Concat(sv1, L""), sv1);
		H_TEST_EQUAL(Concat(L"", sv1), sv1);
		H_TEST_EQUAL(Concat(sv1, sv1), std::wstring(sv1) + std::wstring(sv1));
		std::wstring_view sv2 = sv.substr(2,3);
		H_TEST_EQUAL(Concat(sv2, L"GGGGGgg"), L"FF你GGGGGgg");
		H_TEST_EQUAL(Concat(sv2, L""), sv2);
		H_TEST_EQUAL(Concat(L"", sv2), sv2);
		H_TEST_EQUAL(Concat(sv2, sv2), std::wstring(sv2) + std::wstring(sv2));
		H_TEST_NOTEQUAL(Concat(sv2, sv2), std::wstring(sv2.data()) + std::wstring(sv2));
	}
}

TEST_UNIT(test_StartsWith)
{
	{
		std::string_view sv1("F 你好");
		H_EXPECT_TRUE(StartsWith(sv1, ""));
		H_EXPECT_TRUE(StartsWith(sv1, "F"));
		H_EXPECT_TRUE(StartsWith(sv1, "F "));
		H_EXPECT_TRUE(StartsWith(sv1, "F 你"));
		H_EXPECT_TRUE(StartsWith(sv1, "F 你好"));
		H_EXPECT_TRUE(StartsWith(sv1, "", true));
		H_EXPECT_TRUE(StartsWith(sv1, "F", true));
		H_EXPECT_TRUE(StartsWith(sv1, "F ", true));
		H_EXPECT_TRUE(StartsWith(sv1, "F 你", true));
		H_EXPECT_TRUE(StartsWith(sv1, "F 你好", true));
		H_EXPECT_TRUE(StartsWith(sv1, "", true));
		H_EXPECT_TRUE(StartsWith(sv1, "f", true));
		H_EXPECT_TRUE(StartsWith(sv1, "f ", true));
		H_EXPECT_TRUE(StartsWith(sv1, "f 你", true));
		H_EXPECT_TRUE(StartsWith(sv1, "f 你好", true));

		H_EXPECT_TRUE(StartsWith("", ""));
		H_EXPECT_FALSE(StartsWith("", "aa"));
		H_EXPECT_FALSE(StartsWith("", "aabb"));
	}
	{
		std::wstring_view sv1(L"F 你好");
		H_EXPECT_TRUE(StartsWith(sv1, L""));
		H_EXPECT_TRUE(StartsWith(sv1, L"F"));
		H_EXPECT_TRUE(StartsWith(sv1, L"F "));
		H_EXPECT_TRUE(StartsWith(sv1, L"F 你"));
		H_EXPECT_TRUE(StartsWith(sv1, L"F 你好"));
		H_EXPECT_TRUE(StartsWith(sv1, L"", true));
		H_EXPECT_TRUE(StartsWith(sv1, L"F", true));
		H_EXPECT_TRUE(StartsWith(sv1, L"F ", true));
		H_EXPECT_TRUE(StartsWith(sv1, L"F 你", true));
		H_EXPECT_TRUE(StartsWith(sv1, L"F 你好", true));
		H_EXPECT_TRUE(StartsWith(sv1, L"", true));
		H_EXPECT_TRUE(StartsWith(sv1, L"f", true));
		H_EXPECT_TRUE(StartsWith(sv1, L"f ", true));
		H_EXPECT_TRUE(StartsWith(sv1, L"f 你", true));
		H_EXPECT_TRUE(StartsWith(sv1, L"f 你好", true));

		H_EXPECT_TRUE(StartsWith("", ""));
		H_EXPECT_FALSE(StartsWith("", "aa"));
		H_EXPECT_FALSE(StartsWith("", "aabb"));
	}
	{
		std::string_view sv("FFFUUUCCCKKK");
		std::string_view sv1=sv.substr(3,6);
		H_EXPECT_TRUE(StartsWith(sv1, "U"));
		H_EXPECT_TRUE(StartsWith(sv1, "UU"));
		H_EXPECT_TRUE(StartsWith(sv1, "UUU"));
		H_EXPECT_TRUE(StartsWith(sv1, "UUUC"));
		H_EXPECT_TRUE(StartsWith(sv1, "UUUCC"));
		H_EXPECT_TRUE(StartsWith(sv1, "UUUCCC"));
		H_EXPECT_FALSE(StartsWith(sv1, "UUUCCCK"));
		H_EXPECT_FALSE(StartsWith(sv1, "UUUCCCKK"));
		H_EXPECT_FALSE(StartsWith(sv1, "UUUCCCKKK"));
		H_EXPECT_FALSE(StartsWith(sv1, "UUUCCCKKKK"));
	}
}

TEST_UNIT(test_EndsWith)
{
	{
		H_EXPECT_TRUE(EndsWith("HHHEEELLLLLLOOO", "O"));
		H_EXPECT_TRUE(EndsWith("HHHEEELLLLLLOOO", "OO"));
		H_EXPECT_TRUE(EndsWith("HHHEEELLLLLLOOO", "OOO"));
		H_EXPECT_TRUE(EndsWith("HHHEEELLLLLLOOO", "LLOOO"));
		H_EXPECT_TRUE(EndsWith("HHHEEELLLLLLOOO", ""));
		H_EXPECT_TRUE(EndsWith("HHHEEELLLLLLOOO", "HHHEEELLLLLLOOO"));
		H_EXPECT_FALSE(EndsWith("HHHEEELLLLLLOOO", "HHHHEEELLLLLLOOOO"));
	}
	{
		H_EXPECT_TRUE(EndsWith(L"HHHEEELLLLLLOOO", L"O"));
		H_EXPECT_TRUE(EndsWith(L"HHHEEELLLLLLOOO", L"OO"));
		H_EXPECT_TRUE(EndsWith(L"HHHEEELLLLLLOOO", L"OOO"));
		H_EXPECT_TRUE(EndsWith(L"HHHEEELLLLLLOOO", L"LLOOO"));
		H_EXPECT_TRUE(EndsWith(L"HHHEEELLLLLLOOO", L""));
		H_EXPECT_TRUE(EndsWith(L"HHHEEELLLLLLOOO", L"HHHEEELLLLLLOOO"));
		H_EXPECT_FALSE(EndsWith(L"HHHEEELLLLLLOOO", L"HHHHEEELLLLLLOOOO"));
	}
	{
		std::wstring_view sv = L"HHHEEELLLLLLOOO";
		H_EXPECT_TRUE(EndsWith(sv , L"O"));
		H_EXPECT_TRUE(EndsWith(sv, L"OO"));
		H_EXPECT_TRUE(EndsWith(sv, L"OOO"));
		H_EXPECT_TRUE(EndsWith(sv, L"LLOOO"));
		H_EXPECT_TRUE(EndsWith(sv, L""));
		H_EXPECT_TRUE(EndsWith(sv, L"HHHEEELLLLLLOOO"));
		H_EXPECT_FALSE(EndsWith(sv, L"HHHHEEELLLLLLOOOO"));
	}
	{
		std::wstring_view sv = L"HHHEEELLLLLLOOO";
		sv=sv.substr(3, 9);
		H_EXPECT_TRUE(EndsWith(sv, L"L"));
		H_EXPECT_TRUE(EndsWith(sv, L"LLL"));
		H_EXPECT_TRUE(EndsWith(sv, L"ELLLLLL"));
		H_EXPECT_TRUE(EndsWith(sv, L""));
		H_EXPECT_TRUE(EndsWith(sv, L"") && StartsWith(sv,L"EE") );
	}
}

TEST_UNIT(test_IsWhiteSpace)
{
	{
		std::string s = R"( )";
		H_EXPECT_TRUE(IsWhiteSpace(s));
		s = R"(	 )";
		H_EXPECT_TRUE(IsWhiteSpace(s));
		s = R"(	 
)";
		H_EXPECT_TRUE(IsWhiteSpace(s));
		s = R"(	 
						)";
		H_EXPECT_TRUE(IsWhiteSpace(s));
		s = R"(	 
						  )";
		H_EXPECT_TRUE(IsWhiteSpace(s));
		s = R"(	 

						  )";
		H_EXPECT_TRUE(IsWhiteSpace(s));
		s = R"(	 

										)";
		H_EXPECT_TRUE(IsWhiteSpace(s));
	}
	{
		std::wstring s = LR"( )";
		H_EXPECT_TRUE(IsWhiteSpace(s));
		s = LR"(	 )";
		H_EXPECT_TRUE(IsWhiteSpace(s));
		s = LR"(	 
)";
		H_EXPECT_TRUE(IsWhiteSpace(s));
		s = LR"(	 
						)";
		H_EXPECT_TRUE(IsWhiteSpace(s));
		s = LR"(	 
						  )";
		H_EXPECT_TRUE(IsWhiteSpace(s));
		s = LR"(	 

						  )";
		H_EXPECT_TRUE(IsWhiteSpace(s));
		s = LR"(	 

										)";
		H_EXPECT_TRUE(IsWhiteSpace(s));
	}
	{
		std::string_view sv = "你好吗		\n\r\n   测试";
		H_EXPECT_TRUE(!IsWhiteSpace(sv));
		sv=sv.substr(7, sv.length() - 4-7);
		H_EXPECT_TRUE(IsWhiteSpace(sv));
	}
	{
		std::wstring_view sv = L"你好吗		\n\r\n   测试";
		H_EXPECT_TRUE(!IsWhiteSpace(sv));
		sv = sv.substr(4, sv.length() - 4 - 2);
		H_EXPECT_TRUE(IsWhiteSpace(sv));
	}
}

// the first test running when which  i found the results of the two cases were inconsistent
// because of two function are written at different time. so, be careful here
TEST_UNIT(test_Compare)
{
	{
		std::string_view sv("hello world");
		H_EXPECT_TRUE(Compare(sv, "hello world")==0);
		H_EXPECT_TRUE(Compare(sv, "aello world") > 0);
		H_EXPECT_TRUE(Compare(sv, "aello world") ==1);
		H_EXPECT_TRUE(Compare(sv, "iello world") ==-1);

		H_EXPECT_TRUE(Compare(sv, "Hello world", true) == 0);
		H_EXPECT_TRUE(Compare(sv, "HEllo world", true) == 0);
		H_EXPECT_TRUE(Compare(sv, "HEllO world", true) == 0);
		H_EXPECT_TRUE(Compare(sv, "AELLO WorLD", true) == 1);
		H_EXPECT_TRUE(Compare(sv, "AellO WORLD", true) ==1);
		H_EXPECT_TRUE(Compare(sv, "aello WORld", true) > 0);
		H_EXPECT_TRUE(Compare(sv, "iello wORld", true) < 0);

		H_EXPECT_TRUE(Compare(sv, "") > 0);
		H_EXPECT_TRUE(Compare("", sv) < 0);
		H_EXPECT_TRUE(Compare("", "") == 0);
	}
	{
		std::wstring_view sv(L"hello world");
		H_EXPECT_TRUE(Compare(sv, L"hello world")==0);
		H_EXPECT_TRUE(Compare(sv, L"aello world") > 0);
		H_EXPECT_TRUE(Compare(sv, L"iello world") < 0);

		H_EXPECT_TRUE(Compare(sv, L"Hello world", true) == 0);
		H_EXPECT_TRUE(Compare(sv, L"HEllo world", true) == 0);
		H_EXPECT_TRUE(Compare(sv, L"HEllO world", true) == 0);
		H_EXPECT_TRUE(Compare(sv, L"AELLO WorLD", true) > 0);
		H_EXPECT_TRUE(Compare(sv, L"AellO WORLD", true) > 0);
		H_EXPECT_TRUE(Compare(sv, L"aello WORld", true) > 0);
		H_EXPECT_TRUE(Compare(sv, L"iello wORld", true) < 0);

		H_EXPECT_TRUE(Compare(sv, L"") > 0);
		H_EXPECT_TRUE(Compare(L"", sv) < 0);
		H_EXPECT_TRUE(Compare(L"", L"") == 0);

		sv = L"abc测试中文";
		H_EXPECT_TRUE(Compare(sv, L"abc测试中文")==0);
		H_EXPECT_TRUE(Compare(sv, L"ABC测试中文", true) == 0);
		H_EXPECT_TRUE(Compare(sv, L"Abc测试中文") > 0);
		H_EXPECT_TRUE(Compare(sv, L"Abc测试中文", true) == 0);
		H_EXPECT_TRUE(Compare(sv, L"C测试中文") > 0);
		H_EXPECT_TRUE(Compare(sv, L"C测试中文",true) < 0);

		sv = L"测试中文";
		H_EXPECT_TRUE(Compare(sv, L"测试中文")==0);
	}
	{
		std::string_view sv_("hello world");
		auto sv = sv_.substr(6);//"world"
		H_EXPECT_TRUE(Compare(sv, "world")==0);
		H_EXPECT_TRUE(Compare(sv, "World") >0);
		H_EXPECT_TRUE(Compare(sv, "World") == 1);
		H_EXPECT_TRUE(Compare(sv, "Aorld") > 0);
		H_EXPECT_TRUE(Compare(sv, "") > 0);
		H_EXPECT_TRUE(Compare("", sv) < 0);
		H_EXPECT_TRUE(Compare("", "") == 0);

		H_EXPECT_TRUE(Compare(sv, "WOrld", true) == 0);
		H_EXPECT_TRUE(Compare(sv, "World", true) == 0);
		H_EXPECT_TRUE(Compare(sv, "WorlD", true) == 0);
		H_EXPECT_TRUE(Compare(sv, "Aorld") > 0);
		H_EXPECT_TRUE(Compare(sv, "aorld") > 0);
		H_EXPECT_TRUE(Compare(sv, "Aorld", true) > 0);
		H_EXPECT_TRUE(Compare(sv, "aorld", true) > 0);
		H_EXPECT_TRUE(Compare(sv, "zorld") < 0);
		H_EXPECT_TRUE(Compare(sv, "Zorld") > 0);
		H_EXPECT_TRUE(Compare(sv, "zorld", true) < 0);
		H_EXPECT_TRUE(Compare(sv, "Zorld", true) < 0);
		H_EXPECT_TRUE(Compare(sv, "") > 0);
		H_EXPECT_TRUE(Compare("", sv) < 0);
		H_EXPECT_TRUE(Compare("", "") == 0);
	}
}

TEST_UNIT(test_Contains)
{
	{
		std::string_view sv("hello world");
		H_EXPECT_TRUE(Contains(sv, "ello"));
		H_EXPECT_TRUE(Contains(sv, "orld"));
		H_EXPECT_TRUE(Contains(sv, "world"));
		H_EXPECT_TRUE(Contains(sv, "o world"));
		std::string_view sv1("少玩嫩模");
		H_EXPECT_TRUE(Contains(sv1, "少"));
		H_EXPECT_TRUE(Contains(sv1, "少玩"));
		H_EXPECT_TRUE(Contains(sv1, "少玩嫩"));
		H_EXPECT_TRUE(Contains(sv1, "少玩嫩模"));
		H_EXPECT_TRUE(Contains(sv1, "玩"));
		H_EXPECT_TRUE(Contains(sv1, "玩嫩"));
		H_EXPECT_TRUE(Contains(sv1, "玩嫩模"));
		H_EXPECT_TRUE(Contains(sv1, "嫩"));
		H_EXPECT_TRUE(Contains(sv1, "嫩模"));
		H_EXPECT_TRUE(Contains(sv1, "模"));
	}
	{
		std::wstring_view sv(L"hello world");
		H_EXPECT_TRUE(Contains(sv, L"ello"));
		H_EXPECT_TRUE(Contains(sv, L"orld"));
		H_EXPECT_TRUE(Contains(sv, L"world"));
		H_EXPECT_TRUE(Contains(sv, L"o world"));
		std::wstring_view sv1(L"少玩嫩模");
		H_EXPECT_TRUE(Contains(sv1, L"少"));
		H_EXPECT_TRUE(Contains(sv1, L"少玩"));
		H_EXPECT_TRUE(Contains(sv1, L"少玩嫩"));
		H_EXPECT_TRUE(Contains(sv1, L"少玩嫩模"));
		H_EXPECT_TRUE(Contains(sv1, L"玩"));
		H_EXPECT_TRUE(Contains(sv1, L"玩嫩"));
		H_EXPECT_TRUE(Contains(sv1, L"玩嫩模"));
		H_EXPECT_TRUE(Contains(sv1, L"嫩"));
		H_EXPECT_TRUE(Contains(sv1, L"嫩模"));
		H_EXPECT_TRUE(Contains(sv1, L"模"));
	}
	{
		std::wstring_view sv_(L"hello world");
		auto sv = sv_.substr(6);// "world"
		H_EXPECT_TRUE(!Contains(sv, L"ello"));
		H_EXPECT_TRUE(Contains(sv, L"orld"));
		H_EXPECT_TRUE(Contains(sv, L"world"));
		H_EXPECT_TRUE(!Contains(sv, L"o world"));
		H_EXPECT_TRUE(Contains(sv, L"orld"));

		std::wstring_view sv1_(L"少玩嫩模");
		auto sv1 = sv1_.substr(1);// "world"
		H_EXPECT_TRUE(Contains(sv1, L"玩"));
		H_EXPECT_TRUE(Contains(sv1, L"玩嫩"));
		H_EXPECT_TRUE(Contains(sv1, L"玩嫩模"));
		H_EXPECT_TRUE(Contains(sv1, L"嫩"));
		H_EXPECT_TRUE(Contains(sv1, L"嫩模"));
		H_EXPECT_TRUE(Contains(sv1, L"模"));
	}
}

TEST_UNIT(test_Split)
{
	{
		std::string_view sv("hello,world,is,the,first,program,that,everyone,write,in,this,world");
		H_EXPECT_TRUE(Split(sv, ",").size() == 12);
		H_EXPECT_TRUE(Split(sv, ",,").size() == 12);
		H_EXPECT_TRUE(Split(sv, ".,").size() == 12);
		H_EXPECT_TRUE(Split(sv, ".").size() == 1);
		H_EXPECT_TRUE(Split(sv, "...").size() == 1);
	}
	{
		std::wstring_view wsv(L"年  轻,人.少,玩'嫩''模");
		H_EXPECT_TRUE(Split(wsv, L" ,.'").size() == 7);
		H_EXPECT_TRUE(Split(wsv, L" ").size() == 2);
		H_EXPECT_TRUE(Split(wsv, L",").size() == 3);
		H_EXPECT_TRUE(Split(wsv, L".").size() == 2);
		H_EXPECT_TRUE(Split(wsv, L"'").size() == 3);
	}
	{
		std::wstring_view wsv(L" 年  ,轻 ,  人,少  ,玩,嫩,模  ");
		auto vec=Split(wsv, L",");
		H_EXPECT_TRUE(vec.size() == 7);
	}
	{
		std::wstring_view wsv(L"年 轻,人.少,玩'嫩''模");
		H_EXPECT_TRUE(Split(wsv, L" ,.'").size() == 7);
		H_EXPECT_TRUE(Split(wsv, L" ").size() == 2);
		H_EXPECT_TRUE(Split(wsv, L",").size() == 3);
		H_EXPECT_TRUE(Split(wsv, L".").size() == 2);
		H_EXPECT_TRUE(Split(wsv, L"'").size() == 3);
	}
	// with quoter
	{
		std::string_view path = R"("C:\\Program Files\\Baidu is a rogue", "C:\\Program Files\\Tencent is a rogue too")";
		H_EXPECT_TRUE(Split(path, ",", "\"").size() == 2);
		H_EXPECT_TRUE(Split(path, " ", "\"").size() == 2);
		H_EXPECT_TRUE(Split(path, ", ", "\"").size() == 2);
	}
	//StringSplitOptions
	{
		std::string_view path = " hello,world,,by, lee  ";
		auto vec=Split(path, ",", StringSplitOptions::None);
		H_EXPECT_TRUE(Split(path, ", ", "\"").size() == 4);

		std::string_view path1 = " hello,world,,by, lee  ";
		auto vec1 = Split(path1, ",", StringSplitOptions::RemoveEmptyEntries);
		H_EXPECT_TRUE(Split(path1, ", ", "\"").size() == 4);

		std::string_view path2 = " fuck , , ,off ";
		auto vec2 = Split(path2, ", ", StringSplitOptions::None);
		H_EXPECT_TRUE(vec2.size() == 9);
	}
}

TEST_UNIT(test_Insert)
{
	//string
	{
		H_TEST_EQUAL(Insert("fuck .", 5, "off"), "fuck off.");
		H_TEST_EQUAL(Insert("fuck .", 15, "off"), "fuck .off");
		H_TEST_EQUAL(Insert("fuck .", 6, "off"), "fuck .off");
		H_TEST_EQUAL(Insert("fuck .", 0, "off"), "offfuck .");
		H_TEST_EQUAL(Insert("fuck .", 1, "off"), "foffuck .");
		H_TEST_EQUAL(Insert("fuck .", 2, "off"), "fuoffck .");
	}
	//wstring
	{
		H_TEST_EQUAL(Insert(L"警察，你好吗", 5, L"梁朝伟"), L"警察，你好梁朝伟吗");
		H_TEST_EQUAL(Insert(L"警察，你好吗", -1, L"梁朝伟"), L"警察，你好吗梁朝伟");
		H_TEST_EQUAL(Insert(L"警察，你好吗", 0, L"梁朝伟"), L"梁朝伟警察，你好吗");
		H_TEST_EQUAL(Insert(L"警察，你好吗", 100, L"梁朝伟"), L"警察，你好吗梁朝伟");
		H_TEST_EQUAL(Insert(L"警察，你好吗", 0, L""), L"警察，你好吗");
		H_TEST_EQUAL(Insert(L"警察，你好吗", 3, L"梁朝伟"), L"警察，梁朝伟你好吗");
	}
	//string_view
	{
		std::string str = " hello world,,by, lee  ";
		std::string_view path1 = str;
		std::string_view path2 = path1.substr(1,11);//"hello world"
		H_TEST_EQUAL(Insert(path2, 5, "off"), "hellooff world");
		H_TEST_EQUAL(Insert(path2, 15, "off"),"hello worldoff");
		H_TEST_EQUAL(Insert(path2, 6, "off"), "hello offworld");
		H_TEST_EQUAL(Insert(path2, 0, "off"), "offhello world");
		H_TEST_EQUAL(Insert(path2, 1, "off"), "hoffello world");
		H_TEST_EQUAL(Insert(path2, 2, "off"), "heoffllo world");
	}
}

TEST_UNIT(test_Replace)
{
	{
		H_TEST_EQUAL(Replace("李明同学，你同学是失了智吗", "同学", "小姐"), "李明小姐，你小姐是失了智吗");
		H_TEST_EQUAL(Replace("李明同学，你同学是失了智吗", "", "lee"), "李明同学，你同学是失了智吗");
		H_TEST_EQUAL(Replace("李明同学，你同学是失了智吗", "学", "志"), "李明同志，你同志是失了智吗");
		H_TEST_EQUAL(Replace("李明同学，你同学是失了智吗", "，", ","), "李明同学,你同学是失了智吗");
	}
}

TEST_UNIT(test_Join)
{
	auto b = to<std::string>("hello");
	auto r=toDelim<std::string>(",", "aa", "bb");
	auto re = Join(",", "hello", "world");
	H_TEST_EQUAL(re, "hello,world");

	auto re1 = Alime::base::Join("", "a", "new", "file");
	H_TEST_EQUAL(re1, "anewfile");
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

	std::string str5(sv);
	std::string str6(subSv);
	H_EXPECT_TRUE(str5!=str6);
}


TEST_UNIT(test_toString)
{
	auto re = to<std::string>(3.1415);
	std::string test="fuck";
	//re = to<std::string>(test);
	re = to<std::string>("hello");
	re = to<std::string>("hello",3.1415,test);



}

//Concat
TEST_UNIT(test_ConcatTemplate)
{
	std::string test1 = "what";
	std::string test2 = "the";
	std::string test3 = "fuck";
	auto result1= Alime::base::Concat<std::string>(test1, test2, test3);
	H_EXPECT_TRUE(result1 == "whatthefuck");

	std::wstring wtest1 = L"你";
	std::wstring wtest2 = L"他";
	std::wstring wtest3 = L"妈";
	auto result2 = Alime::base::Concat<std::wstring>(wtest1, wtest2, wtest3);
	H_EXPECT_TRUE(result2 == L"你他妈");
	const std::string testString = u8"尼玛了个";

	wchar_t* wch = L"测试而已";
	auto result3 = Alime::base::Concat<std::wstring>(wtest1, wtest2, wtest3, wch);

	int i4 = 4;
	auto result4 = Alime::base::Concat<std::wstring>(wtest1, wtest2, wtest3, wch, i4);

	double d5 = 5.0001;
	auto result5 = Alime::base::Concat<std::wstring>(wtest1, wtest2, wtest3, wch, i4, d5);
}
