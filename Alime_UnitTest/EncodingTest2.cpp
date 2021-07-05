#include "easyTest.h"
#include "../Alime/Alime/base/Encoding/NEWEncoding.h"

TEST_UNIT(newUtf8decoder_Test)
{
	{
		auto u8Decoder = NEWUTF8Encoding().GetDecoder();
		std::string str_in_u8 = u8"ÄãºÃÂð";
		wchar_t buffer[8] = { 0 };
		int bytesUsed = 0;
		int charsUsed = 0;
		bool completed = false;
		auto CharCount=u8Decoder->GetCharCount((abyte*)str_in_u8.c_str(), 0, str_in_u8.size(), 1);
		u8Decoder->Convert((abyte*)str_in_u8.c_str(), 0, str_in_u8.size(), buffer, 0, 8, true, bytesUsed, charsUsed, completed);
		H_TEST_EQUAL(str_in_u8.size(), 9);
		H_TEST_EQUAL(bytesUsed,9);
		H_TEST_EQUAL(charsUsed, 3);
		H_TEST_EQUAL(completed, true);
	}

	{
		auto u8Decoder = NEWUTF8Encoding().GetDecoder();
		std::string str_in_u8 = u8"²âÊÔÍêÕû×Ö·û";
		bool completed = false;
		int bytesUsed = 0;
		int charsUsed = 0;
		wchar_t buffer[8] = { 0 };
		auto byteArray01 = (abyte*)str_in_u8.c_str();
		u8Decoder->Convert(byteArray01, 0, 17, buffer, 0, 1, false, bytesUsed, charsUsed, completed);
		H_TEST_EQUAL(str_in_u8.size(), 18);
		H_TEST_EQUAL(bytesUsed, 3);
		H_TEST_EQUAL(charsUsed, 1);
		H_TEST_EQUAL(completed, false);
		u8Decoder->Convert(byteArray01, bytesUsed, 18 - bytesUsed, buffer, 1, 7, false, bytesUsed, charsUsed,completed);
		H_TEST_EQUAL(str_in_u8.size(), 9);
		H_TEST_EQUAL(bytesUsed, 15);
		H_TEST_EQUAL(charsUsed, 5);
		H_TEST_EQUAL(completed, true);

		H_EXPECT_ANY_THROW(
			u8Decoder->Convert(byteArray01, 18, 0, buffer, 1, 7, false, bytesUsed, charsUsed, completed)
		);
	}
	{
		auto u8Decoder = NEWUTF8Encoding().GetDecoder();
		std::string str_in_u8 = u8"²âÊÔÍêÕû×Ö·û";
		bool completed = false;
		int bytesUsed = 0;
		int charsUsed = 0;
		wchar_t buffer[8] = { 0 };
		auto byteArray01 = (abyte*)str_in_u8.c_str();
		u8Decoder->Convert(byteArray01, 0, 5, buffer, 0, 1, false, bytesUsed, charsUsed, completed);
		completed = false;
		u8Decoder->Convert(byteArray01, bytesUsed, 18 - bytesUsed, buffer, charsUsed, 7, false, bytesUsed, charsUsed, completed);
		completed = false;
	}

}