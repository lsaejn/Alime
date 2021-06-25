#include "easyTest.h"

#include <utility>   
#include <iostream>

#include "../Alime/Alime/base/details/conv.h"

TEST_UNIT(Alime_Conv_Test)
{
	//getLastElement
	{
		auto should_be_a_wchar_t
			= getLastElement(1, 2, "aaa", 4.0, u8"hello", L"oh,no");

		auto ptr = L"oh,no";
		H_TEST_EQUAL(should_be_a_wchar_t, L"oh,no");
		H_TEST_EQUAL(should_be_a_wchar_t, ptr);
	}

	//LastElement
	{
		static_assert(std::is_same<decltype(
			LastElementType<5, int, double, float, char*, wchar_t>::type{}), wchar_t >::value, "both wchar_t");

		LastElement<int, double, float, char*, wchar_t, const char*>::type tp;

	}

	{
		//typedef std::pair<int, double> MyPair;
		//MyPair c0(0, 1.333);
		//std::tuple_element<0, MyPair>::type val = std::get<0>(c0);
		//std::tuple_element<0, MyPair>::
	}
}