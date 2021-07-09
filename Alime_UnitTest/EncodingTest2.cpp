#include "easyTest.h"
#include "../Alime/Alime/base/Encoding/NEWEncoding.h"

TEST_UNIT(newUtf8decoder_Test)
{
	{
		auto u8Decoder = NEWUTF8Encoding().GetDecoder();
		std::string str_in_u8 = u8"�����";
		wchar_t buffer[8] = { 0 };
		int bytesUsed = 0;
		int charsUsed = 0;
		bool completed = false;

		auto begin = (abyte*)str_in_u8.c_str();

		auto CharCount=u8Decoder->GetCharCount(begin, 0, 1, false);
		H_TEST_EQUAL(CharCount, 0);
		CharCount = u8Decoder->GetCharCount(begin, 1, 1, false);
		H_TEST_EQUAL(CharCount, 1);
		CharCount = u8Decoder->GetCharCount(begin, 2, 1, false);
		H_TEST_EQUAL(CharCount, 1);

		CharCount = u8Decoder->GetCharCount(begin, 0, 9, false);
		H_TEST_EQUAL(CharCount, 3);

		CharCount = u8Decoder->GetCharCount(begin, 0, 1, true);
		H_TEST_EQUAL(CharCount, 1);
		CharCount = u8Decoder->GetCharCount(begin, 1, 1, true);
		H_TEST_EQUAL(CharCount, 1);
		CharCount = u8Decoder->GetCharCount(begin, 2, 1, true);
		H_TEST_EQUAL(CharCount, 1);

		CharCount = u8Decoder->GetCharCount(begin, 0, 3, false);
		H_TEST_EQUAL(CharCount, 1);
		CharCount = u8Decoder->GetCharCount(begin, 1, 3, false);
		H_TEST_EQUAL(CharCount, 2);
		CharCount = u8Decoder->GetCharCount(begin, 2, 3, false);
		H_TEST_EQUAL(CharCount, 1);

		CharCount = u8Decoder->GetCharCount(begin, 0, 1, true);
		H_TEST_EQUAL(CharCount, 1);
		CharCount = u8Decoder->GetCharCount(begin, 1, 1, true);
		H_TEST_EQUAL(CharCount, 1);
		CharCount = u8Decoder->GetCharCount(begin, 2, 1, true);
		H_TEST_EQUAL(CharCount, 1);
		CharCount = u8Decoder->GetCharCount(begin, 0, 4, true);
		H_TEST_EQUAL(CharCount, 2);

		{
			char buf[1024];
			buf[0] = 0b11110000;//1
			buf[1] = 0b11110000;//2
			buf[2] = 0b0100001;//3
			buf[3] = 0b0100011;//4
			buf[4] = 0b1100000;//5
			buf[5] = 0b01110000;//6
			buf[6] = 0b01110000;//7
			buf[7] = 0b01110000;//8
			auto CharCount2= u8Decoder->GetCharCount((abyte*)buf, 0, 8, 1);
			H_TEST_EQUAL(CharCount2, 8);
		}
		{
			char buf[1024];
			buf[0] = 0b11110000;//���Ǹ�˫�ֽ�u8
			auto CharCount = u8Decoder->GetCharCount((abyte*)buf, 0, 1, 1);
			H_TEST_EQUAL(CharCount, 1);

			buf[1] = 0b10110000;//1
			CharCount = u8Decoder->GetCharCount((abyte*)buf, 0, 2, 0);
			H_TEST_EQUAL(CharCount, 0);
			CharCount = u8Decoder->GetCharCount((abyte*)buf, 0, 2, 1);
			H_TEST_EQUAL(CharCount, 1);

			buf[2] = 0b10110000;//1
			CharCount = u8Decoder->GetCharCount((abyte*)buf, 0, 3, 1);
			H_TEST_EQUAL(CharCount, 1);
			CharCount = u8Decoder->GetCharCount((abyte*)buf, 0, 3, 0);
			H_TEST_EQUAL(CharCount, 0);

			buf[3] = 0b10110000;//1
			CharCount = u8Decoder->GetCharCount((abyte*)buf, 0, 4, 1);
			H_TEST_EQUAL(CharCount, 2);//
			CharCount = u8Decoder->GetCharCount((abyte*)buf, 0, 4, 0);
			H_TEST_EQUAL(CharCount, 2);

			buf[4] = 0b10110000;//3
			CharCount = u8Decoder->GetCharCount((abyte*)buf, 0, 5, 1);
			H_TEST_EQUAL(CharCount, 3);
			CharCount = u8Decoder->GetCharCount((abyte*)buf, 0, 5, 0);
			H_TEST_EQUAL(CharCount, 3);

			buf[5] = buf[0];//4
			CharCount = u8Decoder->GetCharCount((abyte*)buf, 0, 6, 1);
			H_TEST_EQUAL(CharCount, 4);
			CharCount = u8Decoder->GetCharCount((abyte*)buf, 0, 6, 0);
			H_TEST_EQUAL(CharCount, 3);

			buf[6] = 0b01000000;//5
			CharCount = u8Decoder->GetCharCount((abyte*)buf, 0, 7, 1);
			H_TEST_EQUAL(CharCount, 5);
			CharCount = u8Decoder->GetCharCount((abyte*)buf, 0, 7, 0);
			H_TEST_EQUAL(CharCount, 5);
		}
	}


	{
		auto u8Decoder = NEWUTF8Encoding().GetDecoder();
		std::string str_in_u8 = u8"�����";
		wchar_t buffer[8] = { 0 };
		int bytesUsed = 0;
		int charsUsed = 0;
		bool completed = false;
		auto begin = (abyte*)str_in_u8.c_str();

		u8Decoder->Convert((abyte*)begin, 0, str_in_u8.size(), buffer, 0, 8, true, bytesUsed, charsUsed, completed);
		H_TEST_EQUAL(str_in_u8.size(), 9);
		H_TEST_EQUAL(bytesUsed, 9);
		H_TEST_EQUAL(charsUsed, 3);
		H_TEST_EQUAL(completed, true);

		u8Decoder->Convert((abyte*)begin, 0, 1, buffer, 0, 8, false, bytesUsed, charsUsed, completed);
		H_TEST_EQUAL(bytesUsed, 1);
		H_TEST_EQUAL(charsUsed, 0);
		H_TEST_EQUAL(completed, true);
		u8Decoder->Convert((abyte*)begin, 1, 8, buffer, 0, 8, false, bytesUsed, charsUsed, completed);
		H_TEST_EQUAL(bytesUsed, 8);
		H_TEST_EQUAL(charsUsed, 3);
		H_TEST_EQUAL(completed, true);


	}

	{
		auto u8Decoder = NEWUTF8Encoding().GetDecoder();
		std::string str_in_u8 = u8"���������ַ�";
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
		H_TEST_EQUAL(str_in_u8.size(), 18);
		H_TEST_EQUAL(bytesUsed, 15);
		H_TEST_EQUAL(charsUsed, 5);
		H_TEST_EQUAL(completed, true);

		//H_EXPECT_ANY_THROW(
		//	u8Decoder->Convert(byteArray01, 18, 0, buffer, 1, 7, false, bytesUsed, charsUsed, completed)
		//);
	}
	{
		auto u8Decoder = NEWUTF8Encoding().GetDecoder();
		std::string str_in_u8 = u8"���������ַ�";
		bool completed = false;
		int bytesUsed = 0;
		int charsUsed = 0;
		wchar_t buffer[8] = { 0 };
		auto byteArray01 = (abyte*)str_in_u8.c_str();
		u8Decoder->Convert(byteArray01, 0, 5, buffer, 0, 1, false, bytesUsed, charsUsed, completed);

		u8Decoder->Convert(byteArray01, bytesUsed+1, 18 - bytesUsed, buffer, charsUsed, 7, false, bytesUsed, charsUsed, completed);

	}

}