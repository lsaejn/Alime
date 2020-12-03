#include "FileSystemInfo.h"

#ifdef OS_WIN
#include "windows.h"
#else

#endif // OS_POSIX

namespace Alime::base::System::IO
{
	namespace
	{
		constexpr int sz = sizeof(FILETIME);
		static_assert(sizeof(FILETIME) == 8, "filetime could be optimized");
		DateTime FTIMEToDateTime(FILETIME ft)
		{
			auto ticks = ((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
			return DateTime(static_cast<aint64>(ticks));
		}
	}

	struct FileSystemInfoBase
	{
#ifdef OS_WIN
		WIN32_FILE_ATTRIBUTE_DATA fileAddtribute_ = { 0 };
#else
		
#endif // OS_POSIX
		bool dataInitialized_ = false;
	};

	FileSystemInfo::FileSystemInfo()
		:base_(new FileSystemInfoBase())
	{
		Init();
	}

	FileSystemInfo::FileSystemInfo(String filename)
		:filePath_(filename)
	{

	}

	String FileSystemInfo::FullName()
	{
		return filePath_.GetFullPath();
	}

	bool FileSystemInfo::Exists() const
	{
		return false;
	}

	void FileSystemInfo::Delete()
	{

	}
	//virtual void GetObjectData(SerializationInfo info, StreamingContext context);
	void FileSystemInfo::Refresh()
	{

	}

	String FileSystemInfo::ToString()
	{
		return {};
	}



	void FileSystemInfo::Init()
	{
		auto _data = &base_->fileAddtribute_;
		GetFileAttributesEx(FullName().c_str(), GetFileExInfoStandard, _data);
		FILETIME ft = _data->ftCreationTime;
		auto ticks=((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
		CreationTime= DateTime(static_cast<aint64>(_data->ftCreationTime.dwHighDateTime));
		//LastWriteTimeUtc= _data->;
		Attributes=(FileAttributes)_data->dwFileAttributes;
		//LastWriteTime = _data->ftLastWriteTime;
		//LastAccessTimeUtc= _data->ftLastAccessTime;
		//LastAccessTime= _data->ftLastAccessTime;
		//CreationTimeUtc=;
		//_data->dwFileAttributes = (int)info->FileAttributes;
		//_data->ftCreationTime = *((Interop.Kernel32.FILE_TIME*) & info->CreationTime);
		//_data->ftLastAccessTime = *((Interop.Kernel32.FILE_TIME*) & info->LastAccessTime);
		//_data->ftLastWriteTime = *((Interop.Kernel32.FILE_TIME*) & info->LastWriteTime);
		//_data->nFileSizeHigh = (uint)(info->EndOfFile >> 32);
		//_data->nFileSizeLow = (uint)info->EndOfFile;
		base_->dataInitialized_= 1;
	}

}