#include <Alime/base/fileSystem/file.h>
#include <Alime/base/fileSystem/FileStream.h>
#include <Alime/base/strings/string_conversions.h>

#include "windows.h"


namespace Alime::base::System::IO
{
	//void File::TestEncoding(unsigned char* buffer, int size, Encoding& encoding, bool& containsBom)
	//{
	//	if (size >= 3 && strncmp((char*)buffer, "\xEF\xBB\xBF", 3) == 0)
	//	{
	//		encoding = Encoding::Utf8;
	//		containsBom = true;
	//	}
	//	else if (size >= 2 && strncmp((char*)buffer, "\xFF\xFE", 2) == 0)
	//	{
	//		encoding = Encoding::Utf16;
	//		containsBom = true;
	//	}
	//	else if (size >= 2 && strncmp((char*)buffer, "\xFE\xFF", 2) == 0)
	//	{
	//		encoding = Encoding::Utf16BE;
	//		containsBom = true;
	//	}
	//	else
	//	{
	//		encoding = Encoding::Mbcs;
	//		containsBom = false;
	//	}
	//}



	//File::File(const FilePath& filePath)
	//	:filePath_(filePath)
	//{
	//}

	//const FilePath& File::GetFilePath()const
	//{
	//	return filePath_;
	//}

	/*bool File::ReadAllText(String& text, Encoding& encoding, bool& containsBom)
	{
		std::vector<unsigned char> buffer;

		FileStream fileStream(filePath_.GetFullPath(),FileMode::Open,FileAccess::Read,FileShare::ReadWrite);
		if (!fileStream.IsAvailable())
			return false;
		if (fileStream.Size() == 0)
		{
			text = L"";
			encoding = Encoding::Mbcs;
			containsBom = false;
			return true;
		}
		buffer.resize((int)fileStream.Size());
		int count = fileStream.Read(&buffer[0], buffer.size());

		TestEncoding(&buffer[0], buffer.size(), encoding, containsBom);

		int bomSize = 0;
		if (containsBom)
		{
			if (encoding == Encoding::Utf8)
				bomSize = 3;
			else if(encoding== Encoding::Utf16 || encoding == Encoding::Utf16BE)
				bomSize = 2;
			text.reserve(buffer.size()- bomSize);
		}
		for (int i = bomSize; i != buffer.size(); ++i)
		{
			text.push_back(buffer[i]);
		}
		return true;
	}*/

	//u8÷±Ω”–¥»Î
	void File::AppendAllLines(String path, std::vector<String> contents)
	{
		FileStream fs(path, FileMode::OpenOrCreate);
		for (auto& line : contents)
		{
			auto u8Str=UTF16ToUTF8(line);
			fs.Write((void*)u8Str.c_str(), u8Str.length());
		}	
	}

	void File::AppendAllLines(String path, std::vector<String> contents, Encoding encoding)
	{
		FileStream fs(path, FileMode::OpenOrCreate);
		for (auto& line : contents)
		{
			if (encoding == Encoding::Utf8)
			{
				auto u8Str = UTF16ToUTF8(line);
				fs.Write((void*)u8Str.c_str(), u8Str.length());
				fs.Write("\r\n", 2);
			}
			//fix me
			else if (encoding == Encoding::Unicode)
			{
				fs.Write((void*)line.c_str(), line.length()*2);
				fs.Write(L"\r\n", 4);
			}
			else if (encoding == Encoding::Mbcs)
			{
				auto defaultStr = SysWideToNativeMB(line);
				fs.Write((void*)defaultStr.c_str(), defaultStr.length());
				fs.Write("\r\n", 2);
			}
		}
	}

	void File::AppendAllText(String path, String contents)
	{
		FileStream fs(path, FileMode::OpenOrCreate);
		auto u8Str = UTF16ToUTF8(contents);
		fs.Write((void*)u8Str.c_str(), u8Str.length());
	}

	void File::AppendAllText(String path, String contents, Encoding encoding)
	{

	}

	void File::Copy(String sourceFileName, String destFileName)
	{
		File::Copy(sourceFileName.c_str(), destFileName.c_str(), false);
	}

	void File::Copy(String sourceFileName, String destFileName, bool overwrite)
	{
		::CopyFile(sourceFileName.c_str(), destFileName.c_str(), overwrite);
	}

	FileStream File::Create(String path)
	{
		return {L""};
	}

	FileStream File::Create(String path, int bufferSize)
	{
		return { L"" };
	}

	void File::Decrypt(String path)
	{

	}

	bool File::Delete(String path)
	{
		if (::DeleteFile(path.c_str()) != 0)
			return true;
		return false;
	}

	void File::Encrypt(String path)
	{

	}

	bool File::Exists(String path)
	{
		const DWORD file_attr = ::GetFileAttributes(path.data());
		if (file_attr != INVALID_FILE_ATTRIBUTES)
		{
			return true;
		}
		return false;
	}

}



