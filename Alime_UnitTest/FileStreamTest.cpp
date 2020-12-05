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
	auto fc=fi.CreationTime;
	fc.Ticks();
	fi.CreationTimeUtc;
	auto len=fi.Length();


	//f_in_cSharp.CreationTime();
}