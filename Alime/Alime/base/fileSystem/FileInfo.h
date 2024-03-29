#pragma once

#include "filePath.h"
#include "Alime/base/base_define.h"
#include <Alime/base/fileSystem/FileSystemInfo.h>
#include "Alime/base/fileSystem/FileStream.h"


namespace Alime::base::System::IO
{
	class DirectoryInfo;

	class FileInfo: public FileSystemInfo
	{
	public:
		FileInfo() = default;
		FileInfo(const String& fileName);
		~FileInfo()=default;

		bool Exists() const;
		void Delete();
		bool Rename(const String& newName);//��C#�ӿ�

		bool IsReadOnly();
		String DirectoryName();
		DirectoryInfo Directory();
		aint64 Length();
		String Name();
		String GetFullPath();
		//StreamWriter AppendText();
		FileInfo CopyTo(const String& destFileName);
		FileInfo CopyTo(const String& destFileName, bool overwrite);
		FileStream Create();
		//StreamWriter CreateText();
		//void Decrypt();
		//void Encrypt();
		void MoveTo(const String& destFileName);
		void MoveTo(const String& destFileName, bool overwrite);
		//FileStream Open(FileMode mode, FileAccess access);
		//FileStream Open(FileMode mode, FileAccess access, FileShare share);
		//FileStream Open(FileMode mode);
		//FileStream OpenRead();
		//StreamReader OpenText();
		//FileStream OpenWrite();
		//FileInfo Replace(String destinationFileName, String destinationBackupFileName, bool ignoreMetadataErrors);
		//FileInfo Replace(String destinationFileName, String destinationBackupFileName);
		String ToString();
	};
}