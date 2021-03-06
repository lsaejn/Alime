#include "easyTest.h"
#include <Alime/base/fileSystem/FileStream.h>
#include <Alime/base/fileSystem/file.h>
#include <iostream>

//巴拉巴拉
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
	ASSERT_TRUE(fs.CanWrite());

}

TEST_UNIT(FileTest)
{
	std::wstring ws = L"你好吗";
	std::vector<std::wstring> lines;
	lines.push_back(L"你好1");
	lines.push_back(L"你好2");
	lines.push_back(L"你好3");
	Alime::base::System::IO::File::AppendAllLines(L"FileStreamTest2.cpp", lines);
}


#include <Alime/base/fileSystem/FileInfo.h>
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