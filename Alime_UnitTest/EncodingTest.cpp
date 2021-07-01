#include "easyTest.h"

#include "../Alime/Alime/base/details/Encoding.h"
#include "../Alime/Alime/base/fileSystem/FileStream.h"

TEST_UNIT(CharEncode_Test)
{

}

TEST_UNIT(Utf8_Encoding_Test)
{
	auto fs = new Alime::base::System::IO::FileStream(L"./fuckyouU8.txt", Alime::base::FileMode::Create,
		Alime::base::FileAccess::ReadWrite,
		Alime::base::FileShare::None);
	fs->SeekFromEnd(0);
	Alime::base::System::IO::Utf8Encoder u8Encoder;
	u8Encoder.Setup(fs);
	int nWrited = u8Encoder.Write(L"我爱你󰀋", 10);
	H_TEST_EQUAL(nWrited, 10);
	u8Encoder.Close();
	fs->Close();
	delete fs;

	Alime::base::System::IO::Utf8Decoder u8Decoder;
	fs = new Alime::base::System::IO::FileStream(L"./fuckyouU8.txt");
	u8Decoder.Setup(fs);
	wchar_t buf[1024];
	auto nRead=u8Decoder.Read(buf, 1024);
	//buf[nRead] = L'\0';
	std::wstring str;
	str.append(buf, nRead/2);
	H_TEST_EQUAL(str, L"我爱你󰀋");
	H_TEST_EQUAL(nRead, 10);
}

TEST_UNIT(Utf16_Encoding_Test)
{
	{
		auto fs = new Alime::base::System::IO::FileStream(L"./fuckyouU16.txt", Alime::base::FileMode::Truncate, Alime::base::FileAccess::ReadWrite);
		Alime::base::System::IO::Utf16Encoder encoder;
		encoder.Setup(fs);
		encoder.Write("\xFF\xFE", 2);
		int nWrited = encoder.Write(L"a test我爱你󰀋\n", 12 * 2);
		H_TEST_EQUAL(nWrited, 24);
		fs->Close();
		delete fs;

		Alime::base::System::IO::Utf16Decoder decoder;
		auto fs2 = new Alime::base::System::IO::FileStream(L"./fuckyouU16.txt");
		decoder.Setup(fs2);
		wchar_t buf[1024];
		auto nRead = decoder.Read(buf, 1024);
		//buf[nRead] = L'\0';
		std::wstring str;
		str.append(buf + 1, (nRead - 2) / 2);
		H_TEST_EQUAL(str, L"a test我爱你󰀋\n");//字符长度12
		H_TEST_EQUAL(nRead, 24 + 2);
		delete fs2;
	}

	{
		/// <summary>测试二进制</summary>
		auto fs = new Alime::base::System::IO::FileStream(L"./fuckyouU16.txt", Alime::base::FileMode::Append);
		Alime::base::System::IO::Utf16Encoder encoder;
		encoder.Setup(fs);
		std::wstring ctn = LR"(你是千堆雪 我是长街
怕日出一到
彼此瓦解
看着蝴蝶扑不过天涯 谁又有权不理解
你是一封信 我是邮差
最后一双脚 惹尽尘埃
忙着去护送
来不及拆开 里面完美的世界)";
		int nWrited = encoder.Write(ctn.data(), ctn.size()*sizeof(wchar_t));
		H_TEST_EQUAL(nWrited, ctn.size() * sizeof(wchar_t));
		fs->Close();
		delete fs;

		Alime::base::System::IO::Utf16Decoder decoder;
		auto fs2 = new Alime::base::System::IO::FileStream(L"./fuckyouU16.txt");
		decoder.Setup(fs2);
		wchar_t buf[1024];
		auto nRead = decoder.Read(buf, 1024);
		//buf[nRead] = L'\0';
		std::wstring str;
		str.append(buf + 13, (nRead - 13* sizeof(wchar_t)) / sizeof(wchar_t));//违反测试标准。跳过上一个内容
		H_TEST_EQUAL(str, ctn);
		H_TEST_EQUAL(nRead-13* sizeof(wchar_t), ctn.size()* sizeof(wchar_t));
		delete fs2;
	}
	//在C#里 如果一个 FileStream移动了指针，那么StreamRead是不会去检测bom的

}


/// <summary>
/// encoder和decoder只支持wchar_t，保持和C#的一致
/// </summary>
/// <param name=""></param>
TEST_UNIT(Mbcs_Encoding_Test)
{
	{
		auto fs = new Alime::base::System::IO::FileStream(L"./fuckyouAnsi.txt", Alime::base::FileMode::Create, Alime::base::FileAccess::ReadWrite);
		Alime::base::System::IO::MbcsEncoder encoder;
		encoder.Setup(fs);

		int nWrited = encoder.Write(L"a test我爱你\n第二行", 13*sizeof(wchar_t));
		H_TEST_EQUAL(nWrited, 13 * sizeof(wchar_t));
		fs->Close();
		delete fs;

		Alime::base::System::IO::MbcsDecoder decoder;
		auto fs2 = new Alime::base::System::IO::FileStream(L"./fuckyouAnsi.txt");
		decoder.Setup(fs2);
		wchar_t buf[1024];
		auto nRead = decoder.Read(buf, 1024);
		//buf[nRead] = L'\0';
		std::wstring str;
		str.append(buf, nRead/ sizeof(wchar_t));
		H_TEST_EQUAL(str, L"a test我爱你\n第二行");//字符长度12
		H_TEST_EQUAL(nRead, 26);
		delete fs2;
	}
	{
		auto fs = new Alime::base::System::IO::FileStream(L"./fuckyouAnsi.txt", Alime::base::FileMode::Create);
		Alime::base::System::IO::MbcsDecoder decoder;
		decoder.Setup(fs);

		char buf[1024] = { 0 };
		int n = decoder.Read(buf, 1024);
		n = 0;


	}
}