#include "easyTest.h"

#include <iostream>

#include <Alime/base/fileSystem/filePath.h>
#include <Alime/base/fileSystem/file.h>
#include <Alime/base/strings/string_conversions.h>

TEST_UNIT(test_FilePath)
{
	//Alime::base::FilePath path(L".");
	//auto cwd = path.GetFullPath();
	//cwd = path.GetFullPath();

	//auto component = Alime::base::FilePath::GetPathComponents(path.GetFullPath());

	//auto folder=path.GetFolder();
	//auto folderPath= folder.GetFullPath();


	//auto thisFile = Alime::base::File(L"./fileSystemTest.cpp");
	//std::string re;
	//Alime::base::Encoding encoding;
	//bool containsBom = false;
	////thisFile.ReadAllText(re, encoding, containsBom);
	//auto u16String=Alime::base::UTF8ToUTF16(re);
	//int x = 3;
}

#include <Alime/base/fileSystem/FileStream.h>
TEST_UNIT(test_FileStream)
{
	//Alime::base::FileStream fs(L"./fileSystemTest.cpp",Alime::base::FileMode::OpenOrCreate, Alime::base::FileAccess::ReadWrite, Alime::base::FileShare::None);
	//bool cr=fs.CanRead();
	//abyte buffer[1024 * 4] = { 0 };
	//fs.Read(buffer, 1024 * 4);
	//std::string s = (char*)buffer;
	//fs.Close();
}