#include "easyTest.h"
#include <string_view>
#include "../Alime/Alime/base/strings/string_util.h"

using namespace Alime::base;
TEST_UNIT(testAny1) {
	auto re=LowerString("fuckPFF��");
	H_TEST_EQUAL(re, "fuckpff��");
}