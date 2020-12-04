#pragma once

#include "filePath.h"
#include "Alime/base/base_define.h"
#include <Alime/base/fileSystem/FileSystemInfo.h>


namespace Alime::base::System::IO
{
	class DirectoryInfo;

	class FileInfo: public FileSystemInfo
	{
	private:
		

	public:
		FileInfo() = default;
		//FileInfo(const FilePath& _filePath);
		FileInfo(String fileName);
		~FileInfo()=default;

		bool Exists() const;
		void Delete() const;
		bool Rename(const String& newName) const;//·ÇC#½Ó¿Ú

		//bool IsReadOnly{ get; set; }
		String DirectoryName();
		DirectoryInfo Directory();
		aint64 Length();
		String Name();
		String GetFullPath();
		//StreamWriter AppendText();
		FileInfo CopyTo(String destFileName);
		FileInfo CopyTo(String destFileName, bool overwrite);
		//FileStream Create();
		//StreamWriter CreateText();
		//void Decrypt();
		//override void Delete();
		//void Encrypt();
		void MoveTo(String destFileName);
		void MoveTo(String destFileName, bool overwrite);
		//FileStream Open(FileMode mode, FileAccess access);
		//FileStream Open(FileMode mode, FileAccess access, FileShare share);
		//FileStream Open(FileMode mode);
		//FileStream OpenRead();
		//StreamReader OpenText();
		//FileStream OpenWrite();
		//FileInfo Replace(String destinationFileName, String destinationBackupFileName, bool ignoreMetadataErrors);
		//FileInfo Replace(String destinationFileName, String destinationBackupFileName);
		//override String ToString();

	};
}