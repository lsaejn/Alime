#include "easyTest.h"
#include <Alime/base/fileSystem/FileStream.h>
#include <Alime/base/fileSystem/file.h>
#include <iostream>
using namespace Alime::base;
//��������
TEST_UNIT(FileStreamRequest)
{
	using System::IO::FileStream;
	FileStream fs(L"FileStreamTest.cpp");
	if (fs.CanRead())
	{
		char buffer[2046] = { 0 };
		auto nRead=fs.Read(buffer, 2046);
		std::string str = buffer;
		std::cout << str;
		auto ret = str.empty();
	}
	ASSERT_TRUE(!fs.CanWrite());

}

TEST_UNIT(FileTest)
{
	std::wstring ws = L"�����";
	std::vector<std::wstring> lines;
	lines.push_back(L"���1");
	lines.push_back(L"���2");
	lines.push_back(L"���3");
	System::IO::File::AppendAllLines(L"FileStreamTest2.cpp", lines);
}