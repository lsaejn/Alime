#include "easyTest.h"

#include <iostream>

#include <Alime/base/fileSystem/filePath.h>
#include <Alime/base/fileSystem/fileinfo.h>
#include <Alime/base/strings/string_conversions.h>
#include <Alime/base/fileSystem/FileStream.h>
#include <Alime/base/fileSystem/file.h>

TEST_UNIT(test_FilePath)
{
	//test logic disk
	{
		Alime::base::System::IO::FilePath path(L"c:");
		auto cwd = path.GetFullPath();
		cwd = path.GetFullPath();
		bool ret = path.IsRoot();
		ASSERT_TRUE(ret);
		ret = path.IsFolder();
		ASSERT_TRUE(ret);
	}
	{
		Alime::base::System::IO::FilePath bad_path(L"d:\\\\");
		auto cwd = bad_path.GetFullPath();
		cwd = bad_path.GetFullPath();
		bool ret = bad_path.IsRoot();
		ASSERT_TRUE(ret);
		ret = bad_path.IsFolder();
		ASSERT_TRUE(ret);
	}
	//file path
	{
		Alime::base::System::IO::FilePath path_not_exist(L"k:\\\\");
		auto cwd = path_not_exist.GetFullPath();
		cwd = path_not_exist.GetFullPath();
		bool ret = path_not_exist.IsRoot();
		ASSERT_TRUE(ret);
		ret = path_not_exist.IsFolder();
		ASSERT_TRUE(!ret);
		ret = path_not_exist.IsFile();
		ASSERT_TRUE(!ret);
		ret = false;
	}

	Alime::base::System::IO::FilePath path(L".");
	auto cwd = path.GetFullPath();
	cwd = path.GetFullPath();

	auto component = Alime::base::System::IO::FilePath::GetPathComponents(path.GetFullPath());

	auto folder=path.GetFolder();
	auto folderPath= folder.GetFullPath();
	bool isDir=path.IsFolder();
	bool isFile = path.IsFile();
	bool isRoot =path.IsRoot();
	


	//auto thisFile = Alime::base::System::IO::FileInfo(L"./fileSystemTest.cpp");
	//std::string re;
	//Alime::base::Encoding encoding;
	//bool containsBom = false;
	//Alime::base::System::IO::File::ReadAllText(re, encoding, containsBom);
	//auto u16String=Alime::base::UTF8ToUTF16(re);
	int x = 3;
}


TEST_UNIT(test_FileStream)
{
	Alime::base::System::IO::FileStream fs(L"./fuckyou.txt",Alime::base::FileMode::OpenOrCreate, Alime::base::FileAccess::ReadWrite, Alime::base::FileShare::None);
	bool cr=fs.CanRead();
	abyte buffer[1024 * 4] = { 0 };
	fs.Read(buffer, 1024 * 4);
	std::string s = (char*)buffer;
	fs.Close();

	auto path = L"./fuckyou.txt";
	auto str=Alime::base::System::IO::File::ReadAllText(path);
	str == str;
}

#include <thread>
TEST_UNIT(test_File)
{
	std::thread t;

	Alime::base::System::IO::File::Delete(L"fuckyou.txt");
	auto fs=Alime::base::System::IO::File::Create(L"fuckyou.txt");
	fs.Close();

	fs=Alime::base::System::IO::FileStream(L"fuckyou.txt", Alime::base::FileMode::Open, Alime::base::FileAccess::ReadWrite);
	if (fs.CanWrite())
	{
		char buffer[] = "this is a test to write ÄãºÃ in u8";
		auto ret=fs.Write(buffer, sizeof(buffer));
		ret = sizeof(buffer);
	}
}