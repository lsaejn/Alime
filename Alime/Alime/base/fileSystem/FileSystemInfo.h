#pragma once
#include <Alime/base/base_define.h>
#include <Alime/base/fileSystem/file_define.h>
#include <Alime/base/fileSystem/filePath.h>
#include <Alime/base/time/Timestamp.h>

namespace Alime::base::System::IO
{
	struct FileSystemInfoBase;

	class FileSystemInfo
	{
	protected:
		

		FileSystemInfo();
		FileSystemInfo(String filename);
		~FileSystemInfo();
		//FileSystemInfo(SerializationInfo info, StreamingContext context);
	public:
		DateTime LastWriteTime;
		DateTime LastAccessTimeUtc;
		DateTime LastAccessTime;
		virtual String FullName()=0;
		String Extension;
		bool Exists() const;
		DateTime CreationTime;
		DateTime LastWriteTimeUtc;
		FileAttributes Attributes;
		DateTime CreationTimeUtc;
		String Name;

		virtual void Delete()=0;
		//virtual void GetObjectData(SerializationInfo info, StreamingContext context);
		void Refresh();
		String ToString();

	protected:
		//String fullPath_;
		String originalPath_;
		FilePath filePath_;
	private:
		FileSystemInfoBase* base_;
		
		void Init();
	};
}