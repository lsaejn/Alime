#include "FileSystemInfo.h"

#include "windows.h"

namespace Alime::base::System::IO
{
	namespace
	{
		constexpr int sz = sizeof(FILETIME);
		static_assert(sizeof(FILETIME) == 8, "filetime could be optimized");

		/// <summary>
		/// 辅助函数，转换一个FILETIME到DateTime。
		/// </summary>
		/// <param name="ft">elapsed since 1601year 1month 1day in the Gregorian calendar.</param>
		/// <returns></returns>
		DateTime FTIMEToDateTime(FILETIME ft)
		{
			auto ticks = ((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
			//elapsed since January 1, 0001 at 00:00 : 00.000 in the Gregorian calendar.
			ticks += 504911520000000000;//
			return DateTime(static_cast<aint64>(ticks));
		}

		DateTime LocalFtimeToUTCDateTime(FILETIME localtime)
		{
			FILETIME utcTime = { 0 };
			BOOL ret = LocalFileTimeToFileTime(&localtime, &utcTime);
			if (!ret)
				throw "failed to switch";
			return FTIMEToDateTime(utcTime);
		}



		WINBASEAPI
			BOOL
			WINAPI
			FileTimeToSystemTime(
				__in  CONST FILETIME* lpFileTime,
				__out LPSYSTEMTIME lpSystemTime
			);

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
		CreationTimeUtc= LocalFtimeToUTCDateTime(_data->ftCreationTime);
		LastWriteTime= FTIMEToDateTime(_data->ftLastWriteTime);
		LastWriteTimeUtc= LocalFtimeToUTCDateTime(_data->ftLastWriteTime);
		LastAccessTime = FTIMEToDateTime(_data->ftLastAccessTime);
		LastAccessTimeUtc= LocalFtimeToUTCDateTime(_data->ftLastAccessTime);
		Attributes=(FileAttributes)_data->dwFileAttributes;
		//我们不取文件大小，是因为文件夹可能很大...
		//return ((long)_data.nFileSizeHigh) << 32 | _data.nFileSizeLow & 0xFFFFFFFFL;
		base_->dataInitialized_= true;
	}

}