#include "easyTest.h"
#include <string_view>
#include "../Alime/Alime/base/strings/string_util.h"

using namespace Alime::base;
TEST_UNIT(testAny1) {
	auto re=LowerString("fuckPFFÄã");
	H_TEST_EQUAL(re, "fuckpffÄã");
}