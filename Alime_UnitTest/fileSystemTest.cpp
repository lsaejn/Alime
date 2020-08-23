#include "easyTest.h"

#include <iostream>

#include <Alime/base/fileSystem/filePath.h>
#include <Alime/base/fileSystem/file.h>
#include <Alime/base/strings/string_conversions.h>

TEST_UNIT(test_FilePath)
{
	Alime::base::FilePath path(L".");
	auto cwd = path.GetFullPath();
	cwd = path.GetFullPath();

	auto component = Alime::base::FilePath::GetPathComponents(path.GetFullPath());

	auto folder=path.GetFolder();
	auto folderPath= folder.GetFullPath();


	auto thisFile = Alime::base::File(L"D:\\公司文档\\pgcell - 副本.txt");
	std::string re;
	Alime::base::Encoding encoding;
	bool containsBom = false;
	thisFile.ReadAllText(re, encoding, containsBom);
	auto u16String=Alime::base::UTF8ToUTF16(re);
	int x = 3;
}