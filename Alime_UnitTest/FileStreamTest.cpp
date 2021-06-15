#include "easyTest.h"
#include <Alime/base/fileSystem/FileStream.h>
#include <Alime/base/fileSystem/file.h>
#include <Alime/base/fileSystem/FileInfo.h>
#include <Alime/base/exceptionBase.h>
#include <iostream>


TEST_UNIT(FileStreamAccessTest)
{
	using Alime::base::System::IO::FileStream;
	Alime::base::System::IO::File::Delete(L"FileStreamAccess.cpp");
	bool exist = Alime::base::System::IO::File::Exists(L"FileStreamAccess.cpp");
	H_TEST_EQUAL(exist, false);
	{
		try {
			FileStream fst(L"FileStreamAccess.cpp", Alime::base::FileMode::Open);
			exist = Alime::base::System::IO::File::Exists(L"FileStreamAccess.cpp");
			H_TEST_EQUAL(exist, false);
		}
		catch (Alime::base::Error &e)
		{
			auto msg = e.Message();
			auto copy = msg;
		}

	}
	FileStream fs(L"FileStreamAccess.cpp", Alime::base::FileMode::OpenOrCreate);
	exist = Alime::base::System::IO::File::Exists(L"FileStreamAccess.cpp");
	H_TEST_EQUAL(exist, true);
	H_TEST_EQUAL(fs.CanRead(), true);
	H_TEST_EQUAL(fs.CanWrite(), true);
	H_TEST_EQUAL(fs.CanSeek(), true);
	H_TEST_EQUAL(fs.CanPeek(), true);
	fs.Close();

	FileStream fs2(L"FileStreamAccess.cpp", Alime::base::FileMode::Create);
}

TEST_UNIT(FileStreamRequest)
{
	using Alime::base::System::IO::FileStream;
	FileStream fs(L"FileStreamTest.cpp");
	if (fs.CanRead())
	{
		char buffer[2046] = { 0 };
		auto nRead=fs.Read(buffer, 2046);
		std::string str = buffer;
		std::cout << str;
		auto ret = str.empty();
	}
	H_TEST_EQUAL(fs.CanWrite(), 1);

}

TEST_UNIT(FileTest)
{
	std::wstring ws = L"你好吗";
	std::vector<std::wstring> lines;
	lines.push_back(L"你好1");
	lines.push_back(L"你好2");
	lines.push_back(L"你好3");
	Alime::base::System::IO::File::AppendAllLines(L"FileTest.text", lines);
}



//we have to compare with C# now due to reason that we modify Datetime class totally
TEST_UNIT(FileInfo)
{
	auto filename = L"D:\\wpfAutoUpdate\\LoveCSharp\\bin\\Debug\\netcoreapp3.1\\2.txt";
	Alime::base::System::IO::FileInfo fi(filename);
	auto fc=fi.creationTime_;
	fc.Ticks();
	fi.creationTimeUtc_;
	auto len=fi.Length();


	//f_in_cSharp.CreationTime();
}


#include "../Alime/Alime/base/fileSystem/StreamReader.h"
TEST_UNIT(StreamReader)
{
	using Alime::base::System::IO::FileStream;
	FileStream fs(L"fuckyou-u16.txt");
	auto sz=fs.Size();
	char buffer[2046] = { 0 };
	auto all = fs.Read(buffer, 2046);
	fs.SeekFromBegin(0);
	all = fs.Read(buffer, 2046);
	fs.SeekFromBegin(0);
	Alime::base::System::IO::StreamReader sr(fs);
	auto info=sr.ReadLine();
	
	fs.SeekFromBegin(0);
	info = sr.ReadLine();
}