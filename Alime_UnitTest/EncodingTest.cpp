#include "easyTest.h"

#include "../Alime/Alime/base/details/CharEncode.h"
#include "../Alime/Alime/base/fileSystem/FileStream.h"

TEST_UNIT(CharEncode_Test)
{

}

TEST_UNIT(CharDecode_Test)
{
	auto fs = new Alime::base::System::IO::FileStream(L"./fuckyouU8.txt");
	fs->SeekFromEnd(0);
	Alime::base::System::IO::Utf8Encoder u8Encoder;
	u8Encoder.Setup(fs);
	u8Encoder.Write(L"ÎÒ°®Äã", 6);
	u8Encoder.Close();
	fs->Close();

	Alime::base::System::IO::Utf8Decoder u8Decoder;
	fs = new Alime::base::System::IO::FileStream(L"./fuckyouU8.txt");
	u8Decoder.Setup(fs);
	wchar_t buf[1024];
	auto nRead=u8Decoder.Read(buf, 1024);
	//buf[nRead] = L'\0';
	std::wstring str = buf;
}