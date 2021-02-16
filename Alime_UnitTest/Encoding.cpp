#include "easyTest.h"

#include "../Alime/Alime/base/details/CharEncode.h"
#include "../Alime/Alime/base/fileSystem/FileStream.h"

TEST_UNIT(CharEncode_Test)
{

}

TEST_UNIT(CharDecode_Test)
{
	auto fs = new Alime::base::System::IO::FileStream(L"D:\\MyProject\\Alime\\Alime_UnitTest\\fuckyouU8.txt");
	fs->SeekFromEnd(0);
	Alime::base::System::IO::Utf8Encoder u8Encoder;
	u8Encoder.Setup(fs);
	u8Encoder.Write(L"ÎÒ°®Äã", 6);
	u8Encoder.Close();
	fs->Close();
}