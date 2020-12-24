#include <stdexcept>
#include <cwctype>

#include <Alime/base/fileSystem/filePath.h>
#include <Alime/base/strings/string_util.h>
#include <Alime/base/fileSystem/file_define.h>

#ifdef OS_WIN
#include "windows.h"
#include "Shlwapi.h"
#pragma comment(lib, "Shlwapi.lib")


Char Alime::base::System::IO::FilePath::Delimiter= kDelimiter;
namespace Alime::base::System::IO
{
	

	FilePath::FilePath()
	{
		Initialize();
	}

	FilePath::FilePath(const String& _filePath)
		:fullPath_(_filePath)
	{
		Initialize();
	}

	FilePath::FilePath(const wchar_t* _filePath)
		: fullPath_(_filePath)
	{
		Initialize();
	}

	FilePath::FilePath(const FilePath& _filePath)
		: fullPath_(_filePath.GetFullPath())
	{
	}

	FilePath& FilePath::operator=(const FilePath& filePath)
	{
		fullPath_ = filePath.fullPath_;
		return *this;
	}

	FilePath::~FilePath()
	{
	}

	void	FilePath::Initialize()
	{

		std::vector<wchar_t> buffer(fullPath_.length() + 1);
		wcscpy_s(&buffer[0], fullPath_.length() + 1, fullPath_.c_str());
		for (size_t i = 0; i < buffer.size(); i++)
		{
			if (buffer[i] == L'\\' || buffer[i] == L'/')
				buffer[i] = Delimiter;
		}
		fullPath_ = &buffer[0];

		if (fullPath_ != L"")
		{
			//Ïà¶ÔÂ·¾¶
			if (fullPath_.length() < 2 || fullPath_[1] != L':')
			{
				wchar_t buffer[MAX_PATH + 1] = { 0 };
				auto result = GetCurrentDirectoryW(sizeof(buffer) / sizeof(*buffer), buffer);
				if (result > MAX_PATH + 1 || result == 0)
				{
					throw std::exception("Failed to call GetCurrentDirectory.");
				}
				fullPath_ = String(buffer) + L"\\" + fullPath_;
			}

			{
				wchar_t buffer[MAX_PATH + 1] = { 0 };
				if (fullPath_.length() == 2 && fullPath_[1] == L':')
				{
					fullPath_ += L"\\";
				}
				auto result = GetFullPathNameW(fullPath_.data(), sizeof(buffer) / sizeof(*buffer), buffer, NULL);
				if (result > MAX_PATH + 1 || result == 0)
				{
					throw std::exception("The path is illegal.");
				}
				{
					wchar_t shortPath[MAX_PATH + 1];
					wchar_t longPath[MAX_PATH + 1];
					if (GetShortPathNameW(buffer, shortPath, MAX_PATH) > 0)
					{
						if (GetLongPathNameW(shortPath, longPath, MAX_PATH) > 0)
						{
							memcpy(buffer, longPath, sizeof(buffer));
						}
					}
				}
				fullPath_ = buffer;
			}
		}
		if (fullPath_ != L"/" && fullPath_.length() > 0 && fullPath_[fullPath_.length() - 1] == Delimiter)
		{
			fullPath_ = fullPath_.substr(0, fullPath_.length() - 1);
		}
		fullPath_[0] = std::towupper(fullPath_[0]);
	}

	bool	FilePath::IsFile()const
	{
		WIN32_FILE_ATTRIBUTE_DATA info;
		BOOL result = GetFileAttributesEx(fullPath_.c_str(), GetFileExInfoStandard, &info);
		if (!result)
			return false;
		return (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
	}

	bool	FilePath::IsFolder()const
	{
		//return PathFileExistsW(fullPath_.data())&&PathIsDirectoryW(fullPath_.data());
		WIN32_FILE_ATTRIBUTE_DATA info;
		BOOL result = GetFileAttributesEx(fullPath_.c_str(), GetFileExInfoStandard, &info);
		if (!result)
			return false;
		return (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
	}

	bool	FilePath::IsRoot()const
	{
		return fullPath_ == L"";
	}

	String  FilePath::GetName()const
	{
		auto index = fullPath_.find_last_of(Delimiter);
		if (index != String::npos)
			return fullPath_.substr(index + 1, fullPath_.length() - index);
		return fullPath_;
	}

	FilePath  FilePath::GetFolder()const
	{
		auto index = fullPath_.find_last_of(Delimiter);
		if (index != String::npos)
			return fullPath_.substr(0, index);
		return FilePath();
	}

	String FilePath::GetFullPath()const
	{
		return fullPath_;
	}


	String FilePath::GetRelativePathFor(const FilePath& _filePath)
	{
		if (fullPath_.length() == 0 || _filePath.fullPath_.length() == 0 || fullPath_[0] != _filePath.fullPath_[0])
		{
			return _filePath.fullPath_;
		}
		wchar_t buffer[MAX_PATH + 1] = { 0 };
		PathRelativePathTo(
			buffer,
			fullPath_.c_str(),
			(IsFolder() ? FILE_ATTRIBUTE_DIRECTORY : 0),
			_filePath.fullPath_.c_str(),
			(_filePath.IsFolder() ? FILE_ATTRIBUTE_DIRECTORY : 0)
		);
		return buffer;
	}

	FilePath  FilePath::operator/(const String& relativePath)const
	{
		if (IsRoot())
		{
			return relativePath;
		}
		else
		{
			return fullPath_ + L"/" + relativePath;
		}
	}

	std::vector<String> FilePath::GetPathComponents(const String& path)
	{
		String pathRemaining = path;
		std::wstring delimiter;
		delimiter += FilePath::Delimiter;
		return Alime::base::Split(path, delimiter);
	}

	int  FilePath::Compare(const FilePath& a, const FilePath& b)
	{
		String strA = a.fullPath_;
		String strB = b.fullPath_;
		const wchar_t* bufA = strA.c_str();
		const wchar_t* bufB = strB.c_str();
		int length = strA.length() < strB.length() ? strA.length() : strB.length();
		while (length--)
		{
			int diff = *bufA++ - *bufB++;
			if (diff != 0)
			{
				return diff;
			}
		};
		return strA.length() - strB.length();
	}
}

#endif