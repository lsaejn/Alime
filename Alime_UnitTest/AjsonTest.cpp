#include "easyTest.h"
#include <Alime/base/json/aJson.h>

//using namespace Alime;

TEST_UNIT(JsonTest)
{
	Ajson value;
	const char* info = { "\"key\":\"null\"" };
	value =Ajson::Parse(info);
	ASSERT_TRUE(value.type_ == JSON_NULL);
}