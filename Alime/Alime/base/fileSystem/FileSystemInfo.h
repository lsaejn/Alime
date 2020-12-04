#pragma once
#include <Alime/base/base_define.h>
#include <Alime/base/fileSystem/file_define.h>
#include <Alime/base/fileSystem/filePath.h>
#include <Alime/base/time/DateTime.h>

#ifdef OS_WIN
#include "windows.h"
#else

#endif // OS_POSIX
namespace Alime::base::System::IO
{
	struct FileSystemInfoBase
	{
		WIN32_FILE_ATTRIBUTE_DATA fileAddtribute_ ;
		bool dataInitialized_ = false;
	};

	class FileSystemInfo
	{
	protected:
		

		FileSystemInfo();
		FileSystemInfo(String filename);
		virtual ~FileSystemInfo();
		//FileSystemInfo(SerializationInfo info, StreamingContext context);
	public:
		DateTime LastWriteTime;
		DateTime LastAccessTimeUtc;
		DateTime LastAccessTime;
		virtual String FullName();
		String Extension;
		bool Exists() const;
		DateTime CreationTime;
		DateTime LastWriteTimeUtc;
		FileAttributes Attributes;
		DateTime CreationTimeUtc;
		String Name;

		virtual void Delete() const;
		//virtual void GetObjectData(SerializationInfo info, StreamingContext context);
		void Refresh();
		String ToString();

	protected:
		//String fullPath_;
		String originalPath_;
		FilePath filePath_;
	protected:
		FileSystemInfoBase* base_;
		
		void Init();
	};
}