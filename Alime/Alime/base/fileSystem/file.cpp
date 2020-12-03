#include <Alime/base/fileSystem/file.h>
#include <Alime/base/fileSystem/FileStream.h>
#include <Alime/base/strings/string_conversions.h>

#include "windows.h"

namespace
{
	using namespace Alime::base;
	static void SetFileReadable(const String& file, bool canRead)
	{
		//not support in windows
	}

	static void SetFileWriteable(const String& file, bool canWrite)
	{
		DWORD attr = GetFileAttributesW(file.c_str());
		if (attr == -1)
			throw "can not open file";
		if (canWrite)
			attr &= ~FILE_ATTRIBUTE_READONLY;
		else
			attr |= FILE_ATTRIBUTE_READONLY;
		if (SetFileAttributesW(file.c_str(), attr) == 0)
			throw "set Attribute error";
	}

	static void setFileExecutable(const String& file, bool canExecute)
	{
		//not support in windows
	}

	static void setFileHideable(const String& file, bool canHide)
	{
		//we not implement this kz of a security reason
	}

	void TestEncoding(unsigned char* buffer, int size, bool& containsBom, Encoding& encoding)
	{
		if (size >= 3 && strncmp((char*)buffer, "\xEF\xBB\xBF", 3) == 0)
		{
			encoding = Encoding::Utf8;
			containsBom = true;
		}
		else if (size >= 2 && strncmp((char*)buffer, "\xFF\xFE", 2) == 0)
		{
			encoding = Encoding::Utf16;
			containsBom = true;
		}
		else if (size >= 2 && strncmp((char*)buffer, "\xFE\xFF", 2) == 0)
		{
			encoding = Encoding::Utf16BE;
			containsBom = true;
		}
		else
		{
			encoding = Encoding::Mbcs;
			containsBom = false;
		}
	}

}


namespace Alime::base::System::IO
{
	



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
		::CopyFile(sourceFileName.c_str(), destFileName.c_str(), !overwrite);
	}

	void File::Move(String sourceFileName, String destFileName)
	{
		File::Move(sourceFileName, destFileName, false);
	}

	void File::Move(String sourceFileName, String destFileName, bool overwrite)
	{
		if (!File::Exists(sourceFileName))
			throw "source file balabalaba";
		bool ret = ::CopyFile(sourceFileName.c_str(), destFileName.c_str(), !overwrite);
		if (ret == 0)//failed, fix me
			throw "...";//fix me
		else
		{
			if (!Delete(sourceFileName))
			{
				throw "...";//fix me
			}
		}
	}

	FileStream File::Create(String path)
	{
		//we did not use CreateFile becz we still need to return a fileStream
		FileStream fs(path, FileMode::Create, FileAccess::ReadWrite, FileShare::ReadWrite);
		if (!fs.IsAvailable())
			throw "failed to open";
		return std::move(fs);
	}

	FileStream File::Create(String path, int bufferSize)
	{
		FileStream fs(path, FileMode::Create, FileAccess::ReadWrite, FileShare::ReadWrite);
		if (!fs.IsAvailable())
			throw "failed to open";
		//fix me, set bufferSize here
		return std::move(fs);
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

	void File::SetAttributes(String path, FileAttributes fileAttributes)
	{
		DWORD file_attr = ::GetFileAttributes(path.data());
		if (file_attr != INVALID_FILE_ATTRIBUTES)
		{
			throw "fix me";
		}
		else
		{
			file_attr |= (DWORD)fileAttributes;
			::SetFileAttributes(path.data(), file_attr);
		}
	}

	FileAttributes File::GetAttributes(String path)
	{
		const DWORD file_attr = ::GetFileAttributes(path.data());
		if (file_attr != INVALID_FILE_ATTRIBUTES)
		{
			throw "fix me";
		}
		return (FileAttributes)file_attr;
	}

	//fix me, check here
	FileStream File::Open(String path, FileMode mode)
	{
		FileStream fs(path, mode, FileAccess::ReadWrite, FileShare::ReadWrite);
		if (!fs.IsAvailable())
			throw "failed to open";
		//fix me, set bufferSize here
		return std::move(fs);
	}

	//fix me, check here
	FileStream File::Open(String path, FileMode mode, FileAccess access)
	{
		FileStream fs(path, mode, access, FileShare::ReadWrite);
		if (!fs.IsAvailable())
			throw "failed to open";
		//fix me, set bufferSize here
		return std::move(fs);
	}

	FileStream File::Open(String path, FileMode mode, FileAccess access, FileShare share)
	{
		FileStream fs(path, mode, access, share);
		if (!fs.IsAvailable())
			throw "failed to open";
		//fix me, set bufferSize here
		return std::move(fs);
	}

	FileStream File::OpenRead(String path)
	{
		FileStream fs(path, FileMode::Open, FileAccess::Read, FileShare::ReadWrite);
		if (!fs.IsAvailable())
			throw "failed to open";
		//fix me, set bufferSize here
		return std::move(fs);
	}
	//static StreamReader OpenText(String path);
	FileStream File::OpenWrite(String path)
	{
		FileStream fs(path, FileMode::Open, FileAccess::Write, FileShare::Read);
		if (!fs.IsAvailable())
			throw "failed to open";
		//fix me, set bufferSize here
		return std::move(fs);
	}

	std::vector<abyte> File::ReadAllBytes(String path)
	{
		FileStream fs(path, FileMode::Open, FileAccess::Read, FileShare::ReadWrite);
		if (fs.CanRead())
		{
			auto fileLen=static_cast<size_t>(fs.Size());//fix me, use buffer insteadly?
			std::vector<abyte> result;
			result.reserve(fileLen);
			fs.Read(&result[0], fileLen);
		}
		else
			throw "fuck";
	}
	std::vector<String> File::ReadAllLines(String path)
	{
		//fix me, we will do this in StreamReader
		return {};
	}

	std::vector<String> File::ReadAllLines(String path, Encoding encoding)
	{
		//fix me, we will do this in StreamReader
		return {};
	}

	String File::ReadAllText(String path)
	{
		//fix me, we will do this in StreamReader
		return {};
	}

	String File::ReadAllText(String path, Encoding encoding)
	{
		//fix me, we will implement this in StreamReader
		return {};
	}

	//The name of a file that replaces the file specified by destinationFileName.
	void File::Replace(String sourceFileName, String destinationFileName, String destinationBackupFileName)
	{
		//fix me check destinationFileName
		File::Copy(destinationFileName, destinationBackupFileName, true);
		if (!File::Delete(destinationFileName))
			throw "delete failed";
		File::Copy(sourceFileName, destinationFileName);
	}

	void File::Replace(String sourceFileName, String destinationFileName, String destinationBackupFileName, bool ignoreMetadataErrors)
	{
		//win32 seem not to support ignore fileshare access
	}

	void File::WriteAllBytes(String path, std::vector<abyte> bytes)
	{

	}
	void File::WriteAllLines(String path, std::vector<String> contents)
	{

	}
	void File::WriteAllLines(String path, std::vector<String> contents, Encoding encoding)
	{

	}
	void File::WriteAllText(String path, String contents)
	{

	}
	void File::WriteAllText(String path, String contents, Encoding encoding)
	{

	}
}



