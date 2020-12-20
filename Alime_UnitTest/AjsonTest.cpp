#include "easyTest.h"
#include <Alime/base/json/aJson.h>

//using namespace Alime;

TEST_UNIT(JsonTest)
{
	AlimeJson value;
	const char* info = { "{\"key\":  null  }" };
	value = AlimeJson::Parse(info);
	ASSERT_TRUE(value.type_ == JSON_NULL);
}