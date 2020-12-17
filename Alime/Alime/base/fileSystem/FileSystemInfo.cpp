#include "FileSystemInfo.h"

#include "windows.h"

namespace Alime::base::System::IO
{

		constexpr int sz = sizeof(FILETIME);
		static_assert(sizeof(FILETIME) == 8, "filetime could be optimized");

		/// <summary>
		/// caculate a FileTime to DateTime
		/// </summary>
		/// <param name="ft"> FILETIME struct</param>
		/// <returns>an utc datetime</returns>
		DateTime FileTimeToUtcDateTime(FILETIME ft)
		{
			auto ticks = ((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
			//SYSTEMTIME st;
			//FileTimeToSystemTime(&ft, &st);
			//return DateTime(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
			ticks += 504911232000000000;//
			return DateTime(ticks, DateTimeKind::Utc);
		}

		DateTime FTIMEToDateTime(FILETIME ft)
		{
			return FileTimeToUtcDateTime(ft).ToLocalTime();
		}

	FileSystemInfo::FileSystemInfo()
		:base_(new FileSystemInfoBase())
	{
		Init();
	}

	FileSystemInfo::FileSystemInfo(String filename)
		:filePath_(filename),
		base_(new FileSystemInfoBase())
	{
		Init();
	}

	FileSystemInfo::~FileSystemInfo()
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

	void FileSystemInfo::Delete() const
	{
		//fix me
	}
	//virtual void GetObjectData(SerializationInfo info, StreamingContext context);
	// This should not throw, instead we store the result so that we can throw it
	// when someone actually accesses a property
	void FileSystemInfo::Refresh()
	{
		if(Exists())
			Init();
	}

	String FileSystemInfo::ToString()
	{
		return L"Class FileSystemInfo";
	}



	void FileSystemInfo::Init()
	{
		//assert base
		auto _data = &base_->fileAddtribute_;
		GetFileAttributesEx(FullName().c_str(), GetFileExInfoStandard, _data);
		CreationTime = FTIMEToDateTime(_data->ftCreationTime);
		CreationTimeUtc= FileTimeToUtcDateTime(_data->ftCreationTime);
		LastWriteTime= FTIMEToDateTime(_data->ftLastWriteTime);
		LastWriteTimeUtc= FileTimeToUtcDateTime(_data->ftLastWriteTime);
		LastAccessTime = FTIMEToDateTime(_data->ftLastAccessTime);
		LastAccessTimeUtc= FileTimeToUtcDateTime(_data->ftLastAccessTime);
		Attributes=(FileAttributes)_data->dwFileAttributes;
		//我们不取文件大小，是因为文件夹可能很大...
		//return ((long)_data.nFileSizeHigh) << 32 | _data.nFileSizeLow & 0xFFFFFFFFL;
		base_->dataInitialized_= true;
	}

}