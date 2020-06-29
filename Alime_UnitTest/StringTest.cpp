#include "easyTest.h"
#include <string_view>

#include <Alime/base/strings/string_util.h>

using namespace Alime::base;
TEST_UNIT(testAny1)
{
	auto re=LowerString("fuckPFFÄã");
	H_TEST_EQUAL(re, "fuckpffÄã");
}


TEST_UNIT(test_Join)
{
	auto r=toDelim<std::string>(",", "aa", "bb");
	auto re = Join(",", "hello", "world");
	H_TEST_EQUAL(re, "hello,world");

	auto re1 = Alime::base::Join("", "a", "new", "file");
	H_TEST_EQUAL(re1, "helloworld");
}