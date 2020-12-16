#include "windows.h"

#include <Alime/base/fileSystem/file.h>
#include <Alime/base/fileSystem/FileStream.h>
#include <Alime/base/strings/string_conversions.h>


/*
* windows 时间相关
* 
* FILETIME是基于UTC的结构体，严格说，不一定和时区相关
* 
GetSystemTime(&st);
GetSystemTimeAsFileTime
SystemTimeToFileTime(&st, &ft)
FileTimeToSystemTime(&ft,&st)
GetLocalTime;
GetSystemTime
SystemTimeToTzSpecificLocalTime
LocalFileTimeToFileTime
*/

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
			encoding = Encoding::Unknown;
			containsBom = false;
		}
	}

}


namespace Alime::base::System::IO
{
	extern DateTime FTIMEToDateTime(FILETIME ft);

	//u8直接写入
	void File::AppendAllLines(const String& path, std::vector<String>& contents)
	{
		FileStream fs(path, FileMode::OpenOrCreate);
		for (auto& line : contents)
		{
			auto u8Str=UTF16ToUTF8(line);
			fs.Write((void*)u8Str.c_str(), u8Str.length());
		}	
	}

	void File::AppendAllLines(const String& path, std::vector<String>& contents, Encoding encoding)
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
				fs.Write(L"\r\n", (sizeof(L"\r\n")-1)* sizeof (wchar_t));
			}
			else if (encoding == Encoding::Mbcs)
			{
				auto defaultStr = SysWideToNativeMB(line);
				fs.Write((void*)defaultStr.c_str(), defaultStr.length());
				fs.Write("\r\n", 2);
			}
			else
			{
				throw "encode is not supported now";
			}
		}
	}

	void File::AppendAllText(const String& path, const String& contents)
	{
		FileStream fs(path, FileMode::OpenOrCreate);
		auto u8Str = UTF16ToUTF8(contents);
		fs.Write((void*)u8Str.c_str(), u8Str.length());
	}

	void File::AppendAllText(const String& path, const String& content, Encoding encoding)
	{
		FileStream fs(path, FileMode::OpenOrCreate);
		if (encoding == Encoding::Utf8)
		{
			auto u8Str = UTF16ToUTF8(content);
			fs.Write((void*)u8Str.c_str(), u8Str.length());
		}
		else if (encoding == Encoding::Unicode)
		{
			fs.Write((void*)content.c_str(), content.length() * 2);
		}
		else if(encoding == Encoding::Mbcs)
		{
			auto ansiStr = SysWideToNativeMB(content);
			fs.Write((void*)ansiStr.c_str(), ansiStr.length());
		}
		else
		{
		throw "encode is not supported now";
		}
	}

	StreamWriter File::AppendText(const String& path)
	{
		//fix me
		return {};
	}

	void File::Copy(const String& sourceFileName, const String& destFileName)
	{
		File::Copy(sourceFileName.c_str(), destFileName.c_str(), false);
	}

	void File::Copy(const String& sourceFileName, const String& destFileName, bool overwrite)
	{
		::CopyFile(sourceFileName.c_str(), destFileName.c_str(), !overwrite);
	}

	void File::Move(const String& sourceFileName, const String& destFileName)
	{
		File::Move(sourceFileName, destFileName, false);
	}

	void File::Move(const String& sourceFileName, const String& destFileName, bool overwrite)
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

	FileStream File::Create(const String& path)
	{
		//we did not use CreateFile becz we still need to return a fileStream
		FileStream fs(path, FileMode::Create, FileAccess::ReadWrite, FileShare::ReadWrite);
		if (!fs.IsAvailable())
			throw "failed to open";
		return std::move(fs);
	}

	FileStream File::Create(const String& path, int bufferSize)
	{
		FileStream fs(path, FileMode::Create, FileAccess::ReadWrite, FileShare::ReadWrite);
		if (!fs.IsAvailable())
			throw "failed to open";
		//fix me, set bufferSize here
		return std::move(fs);
	}

	StreamWriter File::CreateText(const String& path)
	{
		return {};
	}

	void File::Decrypt(const String& path)
	{

	}

	bool File::Delete(const String& path)
	{
		if (::DeleteFile(path.c_str()) != 0)
			return true;
		return false;
	}

	void File::Encrypt(const String& path)
	{

	}

	bool File::Exists(const String& path)
	{
		const DWORD file_attr = ::GetFileAttributes(path.data());
		if (file_attr != INVALID_FILE_ATTRIBUTES)
		{
			return true;
		}
		return false;
	}

	void File::SetAttributes(const String& path, FileAttributes fileAttributes)
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

	FileAttributes File::GetAttributes(const String& path)
	{
		const DWORD file_attr = ::GetFileAttributes(path.data());
		if (file_attr != INVALID_FILE_ATTRIBUTES)
		{
			throw "fix me";
		}
		return (FileAttributes)file_attr;
	}

	DateTime File::GetCreationTime(const String& path)
	{
		WIN32_FILE_ATTRIBUTE_DATA info;
		BOOL result = GetFileAttributesEx(path.c_str(), GetFileExInfoStandard, &info);
		FILETIME createFileTime = info.ftCreationTime;
		return FTIMEToDateTime(createFileTime);
		
	}

	DateTime File::GetCreationTimeUtc(const String& path)
	{
		return GetCreationTime(path).ToUniversalTime();
	}

	DateTime File::GetLastAccessTime(const String& path)
	{
		WIN32_FILE_ATTRIBUTE_DATA info;
		BOOL result = GetFileAttributesEx(path.c_str(), GetFileExInfoStandard, &info);
		FILETIME createFileTime = info.ftLastAccessTime;
		return FTIMEToDateTime(createFileTime);
	}

	DateTime File::GetLastAccessTimeUtc(const String& path)
	{
		return GetLastAccessTime(path).ToUniversalTime();
	}

	DateTime File::GetLastWriteTime(const String& path)
	{
		WIN32_FILE_ATTRIBUTE_DATA info;
		BOOL result = GetFileAttributesEx(path.c_str(), GetFileExInfoStandard, &info);
		FILETIME createFileTime = info.ftLastWriteTime;
		return FTIMEToDateTime(createFileTime);
	}

	DateTime File::GetLastWriteTimeUtc(const String& path)
	{
		return GetLastWriteTime(path).ToUniversalTime();
	}

	void SetFileTimeImpl(const String& path,
		const DateTime* creationTime,
		const DateTime* lastAccessTime,
		const DateTime* lastWriteTime
		)
	{
		HANDLE hFile = CreateFile(path.c_str(),
			GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
		FILETIME ft1;
		FILETIME ft2;
		FILETIME ft3;

		if (creationTime)
		{
			//fix me
			ULARGE_INTEGER largeInteger;
			largeInteger.QuadPart = creationTime->ToFileTimeUtc();
			auto ticks = creationTime->ToFileTimeUtc();
			ft1.dwHighDateTime = largeInteger.HighPart;
			ft1.dwLowDateTime = largeInteger.LowPart;
		}
		if (lastAccessTime)
		{
			auto ticks = lastAccessTime->ToFileTimeUtc();
			ft2.dwHighDateTime = ticks >> 32;
			ft2.dwLowDateTime = static_cast<int>(ticks);
		}
		if (lastWriteTime)
		{
			auto ticks = lastWriteTime->ToFileTimeUtc();
			ft3.dwHighDateTime = ticks >> 32;
			ft3.dwLowDateTime = static_cast<int>(ticks);
		}
		SetFileTime(hFile, creationTime? &ft1 : nullptr, lastAccessTime? &ft2 : nullptr, lastWriteTime ? &ft3: nullptr);
	}


	void File::SetCreationTime(const String& path, DateTime creationTime)
	{
		creationTime = creationTime.ToUniversalTime();
		SetFileTimeImpl(path, &creationTime, nullptr, nullptr);
	}

	void File::SetCreationTimeUtc(const String& path, DateTime creationTimeUtc)
	{
		SetFileTimeImpl(path, &creationTimeUtc, nullptr, nullptr);
	}

	void File::SetLastAccessTime(const String& path, DateTime lastAccessTime)
	{
		lastAccessTime = lastAccessTime.ToUniversalTime();
		SetFileTimeImpl(path, nullptr, &lastAccessTime, nullptr);
	}

	void File::SetLastAccessTimeUtc(const String& path, DateTime lastAccessTimeUtc)
	{
		SetFileTimeImpl(path, nullptr, &lastAccessTimeUtc, nullptr);
	}

	void File::SetLastWriteTime(const String& path, DateTime lastWriteTime)
	{
		lastWriteTime = lastWriteTime.ToUniversalTime();
		SetFileTimeImpl(path, nullptr, nullptr, &lastWriteTime);
	}

	void File::SetLastWriteTimeUtc(const String& path, DateTime lastWriteTimeUtc)
	{
		SetFileTimeImpl(path, nullptr, nullptr, &lastWriteTimeUtc);
	}

	//fix me, check here
	FileStream File::Open(const String& path, FileMode mode)
	{
		FileStream fs(path, mode, FileAccess::ReadWrite, FileShare::ReadWrite);
		if (!fs.IsAvailable())
			throw "failed to open";
		//fix me, set bufferSize here
		return std::move(fs);
	}

	//fix me, check here
	FileStream File::Open(const String& path, FileMode mode, FileAccess access)
	{
		FileStream fs(path, mode, access, FileShare::ReadWrite);
		if (!fs.IsAvailable())
			throw "failed to open";
		//fix me, set bufferSize here
		return std::move(fs);
	}

	FileStream File::Open(const String& path, FileMode mode, FileAccess access, FileShare share)
	{
		FileStream fs(path, mode, access, share);
		if (!fs.IsAvailable())
			throw "failed to open";
		//fix me, set bufferSize here
		return std::move(fs);
	}

	FileStream File::OpenRead(const String& path)
	{
		FileStream fs(path, FileMode::Open, FileAccess::Read, FileShare::ReadWrite);
		if (!fs.IsAvailable())
			throw "failed to open";
		//fix me, set bufferSize here
		return std::move(fs);
	}

	//StreamReader File::OpenText(const String& path)
	//{
	//	return { path };
	//}

	//static StreamReader OpenText(String path);
	FileStream File::OpenWrite(const String& path)
	{
		FileStream fs(path, FileMode::Open, FileAccess::Write, FileShare::Read);
		if (!fs.IsAvailable())
			throw "failed to open";
		//fix me, set bufferSize here
		return std::move(fs);
	}

	std::vector<abyte> File::ReadAllBytes(const String& path)
	{
		FileStream fs(path, FileMode::Open, FileAccess::Read, FileShare::ReadWrite);
		if (fs.CanRead())
		{
			auto fileLen=static_cast<size_t>(fs.Size());//fix me, use buffer insteadly?
			std::vector<abyte> result;
			result.reserve(fileLen);
			fs.Read(&result[0], fileLen);
			return result;
		}
		else
			throw "fuck";
	}
	std::vector<String> File::ReadAllLines(const String& path)
	{
		//fix me, we will do this in StreamReader
		return {};
	}

	std::vector<String> File::ReadAllLines(const String& path, Encoding encoding)
	{
		//fix me, we will do this in StreamReader
		return {};
	}

	String File::ReadAllText(const String& path)
	{
		//fix me, we will do this in StreamReader
		return {};
	}

	String File::ReadAllText(const String& path, Encoding encoding)
	{
		//fix me, we will implement this in StreamReader
		return {};
	}

	//The name of a file that replaces the file specified by destinationFileName.
	void File::Replace(const String& sourceFileName, const String& destinationFileName, const String& destinationBackupFileName)
	{
		//fix me check destinationFileName
		File::Copy(destinationFileName, destinationBackupFileName, true);
		if (!File::Delete(destinationFileName))
			throw "delete failed";
		File::Copy(sourceFileName, destinationFileName);
	}

	void File::Replace(const String& sourceFileName, const String& destinationFileName, const String& destinationBackupFileName, bool ignoreMetadataErrors)
	{
		//win32 seem not to support ignore fileshare access
	}

	void File::WriteAllBytes(const String& path, std::vector<abyte>& bytes)
	{

	}
	void File::WriteAllLines(const String& path, std::vector<String>& contents)
	{

	}
	void File::WriteAllLines(const String& path, std::vector<String>& contents, Encoding encoding)
	{

	}
	void File::WriteAllText(const String& path, const String& contents)
	{

	}
	void File::WriteAllText(const String& path, const String& contents, Encoding encoding)
	{

	}
}



