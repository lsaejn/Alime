#include "easyTest.h"
#include <Alime/base/json/aJson.h>

//using namespace Alime;

std::vector<int> rv()
{
	std::vector<int> v{ 1,3,5 };
	return v;
}

class A
{

};

class B : public A
{

};

TEST_UNIT(JsonTest)
{
	{
		A* b = new B();
		auto a = b;
	}
	{
		AlimeJson value;
		const char* info = { "null" };
		value = AlimeJson::Parse(info);
		ASSERT_TRUE(value.GetType() == JSON_NULL);
	}

	{
		AlimeJson value;
		const char* info = { "  true  " };
		value = AlimeJson::Parse(info);
		ASSERT_TRUE(value.GetType() == JSON_TRUE);
	}

	{
		AlimeJson value;
		const char* info = { "  false  " };
		value = AlimeJson::Parse(info);
		ASSERT_TRUE(value.GetType() == JSON_FALSE);
	}

	{
		AlimeJson value;
		const char* info = { "\"fuckkkk\"" };
		value = AlimeJson::Parse(info);
		ASSERT_TRUE(value.GetType() == JSON_STRING);
	}

}