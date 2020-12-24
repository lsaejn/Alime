#include "FileSystemInfo.h"

#include "windows.h"

namespace Alime::base::System::IO
{

	constexpr int sz = sizeof(FILETIME);
	static_assert(sizeof(FILETIME) == 8, "filetime could be optimized");

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

	FileSystemInfo::FileSystemInfo(const String& filename)
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

	String FileSystemInfo::Name()
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
		creationTime_ = FTIMEToDateTime(_data->ftCreationTime);
		creationTimeUtc_= FileTimeToUtcDateTime(_data->ftCreationTime);
		lastWriteTime_ = FTIMEToDateTime(_data->ftLastWriteTime);
		lastWriteTimeUtc_ = FileTimeToUtcDateTime(_data->ftLastWriteTime);
		lastAccessTime_ = FTIMEToDateTime(_data->ftLastAccessTime);
		lastAccessTimeUtc_ = FileTimeToUtcDateTime(_data->ftLastAccessTime);
		attributes_ = (FileAttributes)_data->dwFileAttributes;
		//我们不取文件大小，是因为文件夹可能很大...
		//return ((long)_data.nFileSizeHigh) << 32 | _data.nFileSizeLow & 0xFFFFFFFFL;
		base_->dataInitialized_= true;
	}

}