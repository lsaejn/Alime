#include "easyTest.h"
#include <Alime/base/fileSystem/FileStream.h>
#include <Alime/base/fileSystem/file.h>
#include <Alime/base/fileSystem/FileInfo.h>
#include <Alime/base/exceptionBase.h>
#include <iostream>


TEST_UNIT(FileStreamAccessTest)
{
	using Alime::base::System::IO::FileStream;
	bool exist = Alime::base::System::IO::File::Exists(L"FileStreamAccess.txt");
	if (exist)
	{
		bool ret = Alime::base::System::IO::File::Delete(L"FileStreamAccess.txt");
		H_TEST_EQUAL(exist, false);
	}
	
	exist = Alime::base::System::IO::File::Exists(L"FileStreamAccess.txt");
	H_TEST_EQUAL(exist, false);
	{
		try {
			FileStream fst(L"FileStreamAccess.txt", Alime::base::FileMode::Open);
			exist = Alime::base::System::IO::File::Exists(L"FileStreamAccess.txt");
			H_TEST_EQUAL(exist, false);
		}
		catch (Alime::base::Error &e)
		{
			auto msg = e.Message();
			H_TEST_NOTEQUAL(msg, nullptr);
		}

	}
	FileStream fs(L"FileStreamAccess.cpp", Alime::base::FileMode::OpenOrCreate, Alime::base::FileAccess::ReadWrite);
	exist = Alime::base::System::IO::File::Exists(L"FileStreamAccess.cpp");
	H_TEST_EQUAL(exist, true);
	H_TEST_EQUAL(fs.CanRead(), true);
	H_TEST_EQUAL(fs.CanWrite(), true);
	H_TEST_EQUAL(fs.CanSeek(), true);
	H_TEST_EQUAL(fs.CanPeek(), true);
	fs.Close();

}

TEST_UNIT(FileStreamRequest)
{
	using Alime::base::System::IO::FileStream;
	//本文件
	FileStream fs(L"FileStreamTest.cpp", Alime::base::FileMode::Open,
		Alime::base::FileAccess::ReadWrite,
		Alime::base::FileShare::Write);
	H_TEST_EQUAL(fs.CanWrite(), 1);
	if (fs.CanRead())
	{
		char buffer[2046] = { 0 };
		auto nRead=fs.Read(buffer, 2046);
		std::string str = buffer;
		std::cout << str;
		auto ret = str.empty();
	}

	try
	{
		//try to open a file not exist
		FileStream fs1(L"FileStreamTest1.cpp");
	}
	catch (Alime::base::Error& e)
	{
		auto msg =  e.Message();
		msg = nullptr;
	}

	try
	{
		//opened by another process
		FileStream fs2(L"FileStreamTest.cpp");
	}
	catch (Alime::base::Error& e)
	{
		std::cout << e.Message();
	}

	try
	{
		FileStream fs3(L"FileStreamTest.cpp", Alime::base::FileMode::OpenOrCreate);
	}
	catch (Alime::base::Error& e)
	{
		std::cout << e.Message();
	}

	try
	{
		FileStream fs4(L"FileStreamTest.cpp", Alime::base::FileMode::Append);
	}
	catch (Alime::base::Error& e)
	{
		std::cout << e.Message();
	}

	try
	{
		//读写权限打开一个被truncated的文件
		FileStream fs4(L"FileStreamTest.cpp", Alime::base::FileMode::Truncate);
	}
	catch (Alime::base::Error& e)
	{
		std::cout << e.Message();
	}
	


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
	FileStream fs(L"fuckyouu16.txt", Alime::base::FileMode::OpenOrCreate, Alime::base::FileAccess::ReadWrite);
	auto sz=fs.Size();
	char buffer[2046] = { 0 };
	auto all = fs.Read(buffer, 2046);
	fs.SeekFromBegin(0);
	all = fs.Read(buffer, 2046);
	fs.SeekFromBegin(0);
	Alime::base::System::IO::StreamReader sr(fs, true);
	auto info=sr.ReadLine();
	
	fs.SeekFromBegin(0);
	info = sr.ReadLine();
}