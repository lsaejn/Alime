#include "easyTest.h"
#include <Alime/base/json/aJson.h>

//using namespace Alime;

std::vector<int> rv()
{
	std::vector<int> v{ 1,3,5 };
	return v;
}

TEST_UNIT(JsonTest)
{
	{
		std::vector<int> vi=rv();
		vi.size();
	}
	AlimeJson value;
	const char* info = { "null" };
	value = AlimeJson::Parse(info);
	ASSERT_TRUE(value.GetType() == JSON_NULL);
}