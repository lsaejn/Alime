#include "FileInfo.h"
#include "DirectoryInfo.h"
#include "windows.h"

namespace Alime::base::System::IO
{
	FileInfo::FileInfo(const FilePath& filePath)
		:filePath_(filePath)
	{

	}

	FileInfo::FileInfo(String fileName)
		: filePath_(fileName)
	{

	}

	bool FileInfo::Rename(const String& newName) const
	{
		String oldFileName = filePath_.GetFullPath();
		String newFileName = (filePath_.GetFolder() / newName).GetFullPath();
		return MoveFile(oldFileName.c_str(), newFileName.c_str()) != 0;
	}

	bool FileInfo::Exists() const
	{
		DWORD attr = GetFileAttributes(filePath_.GetFullPath().c_str());
		if (attr == INVALID_FILE_ATTRIBUTES)
		{
			switch (GetLastError())
			{
			case ERROR_FILE_NOT_FOUND:
			case ERROR_PATH_NOT_FOUND:
			case ERROR_NOT_READY:
			case ERROR_INVALID_DRIVE:
				return false;
			default:
				//fix me, unexpected error
				//handleLastErrorImpl(_path);
				return false;
			}
		}
		return true;
	}

	bool FileInfo::Delete()const
	{
		return DeleteFile(filePath_.GetFullPath().c_str()) != 0;
	}

	String FileInfo::DirectoryName()
	{
		return filePath_.GetFolder().GetFullPath().c_str();
	}

	DirectoryInfo FileInfo::Directory()
	{
		auto folder = filePath_.GetFolder();
		return {};
	}

	aint64 FileInfo::Length()
	{
		WIN32_FILE_ATTRIBUTE_DATA fad;
		if (GetFileAttributesEx(filePath_.GetFullPath().c_str(), GetFileExInfoStandard, &fad) == 0)
		{
			//fix me
			//throw 
		}
		LARGE_INTEGER li;
		li.LowPart = fad.nFileSizeLow;
		li.HighPart = fad.nFileSizeHigh;
		return li.QuadPart;
	}

	String FileInfo::Name()
	{
		return filePath_.GetName();
	}

	String FileInfo::GetFullPath()
	{
		return filePath_.GetFullPath();
	}

	FileInfo FileInfo::CopyTo(String destFilePath)
	{
		return CopyTo(destFilePath, false);
	}

	FileInfo FileInfo::CopyTo(String destFilePath, bool overwrite)
	{
		auto ret = ::CopyFile(GetFullPath().c_str(), destFilePath.c_str(), false) != 0;
		if (!ret)
			return FileInfo{};
		return FileInfo{ destFilePath };
	}

	void FileInfo::MoveTo(String destFileName)
	{
		return MoveTo(destFileName, false);
	}

	void FileInfo::MoveTo(String destFileName, bool overwrite)
	{
		auto newFile=CopyTo(destFileName, overwrite);
		Delete();
		//fix me, rvalue instead
		filePath_ = newFile.filePath_;
	}
}