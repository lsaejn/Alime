#include "easyTest.h"
#include <Alime/base/fileSystem/FileStream.h>
#include <Alime/base/fileSystem/file.h>
#include <Alime/base/fileSystem/FileInfo.h>
#include <Alime/base/exceptionBase.h>
#include <iostream>

TEST_UNIT(FileTest)
{
	std::wstring ws = L"你好吗";
	std::vector<std::wstring> lines;
	lines.push_back(L"你好1");
	lines.push_back(L"你好2");
	lines.push_back(L"你好3");
	Alime::base::System::IO::File::AppendAllLines(L"FileTest.text", lines);
}