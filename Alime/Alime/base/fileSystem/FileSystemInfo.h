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
		FileSystemInfo(const String& filename);
		virtual ~FileSystemInfo();
		//FileSystemInfo(SerializationInfo info, StreamingContext context);
		void Init();
		void Refresh();
		String ToString();
		virtual bool Exists() const;
		virtual String FullName();
		virtual String Name();
		virtual void Delete() const;
		//virtual void GetObjectData(SerializationInfo info, StreamingContext context);
		
	public:
		String name_;
		String extension_;
		FilePath filePath_;
		String originalPath_;
		FileAttributes attributes_;
		FileSystemInfoBase* base_;
		
		DateTime lastWriteTime_;
		DateTime lastAccessTime_;
		DateTime lastAccessTimeUtc_;
		DateTime creationTime_;
		DateTime creationTimeUtc_;
		DateTime lastWriteTimeUtc_;
	};
}