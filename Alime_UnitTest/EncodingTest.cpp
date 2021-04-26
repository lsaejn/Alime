#include "easyTest.h"

#include "../Alime/Alime/base/details/CharEncode.h"
#include "../Alime/Alime/base/fileSystem/FileStream.h"

TEST_UNIT(CharEncode_Test)
{

}

TEST_UNIT(CharDecode_Test)
{
	auto fs = new Alime::base::System::IO::FileStream(L"./fuckyouU8.txt", Alime::base::FileMode::Truncate);
	fs->SeekFromEnd(0);
	Alime::base::System::IO::Utf8Encoder u8Encoder;
	u8Encoder.Setup(fs);
	int nWrited = u8Encoder.Write(L"ÎÒ°®Äã", 6);
	H_TEST_EQUAL(nWrited, 6);
	u8Encoder.Close();
	fs->Close();

	Alime::base::System::IO::Utf8Decoder u8Decoder;
	fs = new Alime::base::System::IO::FileStream(L"./fuckyouU8.txt");
	u8Decoder.Setup(fs);
	wchar_t buf[1024];
	auto nRead=u8Decoder.Read(buf, 1024);
	//buf[nRead] = L'\0';
	std::wstring str;
	str.append(buf, nRead/2);
	H_TEST_EQUAL(str, L"ÎÒ°®Äã");
	H_TEST_EQUAL(nRead, 6);
}