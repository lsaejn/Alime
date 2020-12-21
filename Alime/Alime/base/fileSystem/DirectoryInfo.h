#pragma once
#include <Alime/base/fileSystem/FileSystemInfo.h>
#include <Alime/base/fileSystem/FileInfo.h>

namespace Alime::base::System::IO
{

	class DirectoryInfo : public FileSystemInfo
	{
	public:
		DirectoryInfo(String path);

		DirectoryInfo Parent();
		String FullName() override;
		virtual String Name() override;
		bool Exists() const override;
		DirectoryInfo Root();

		void Create();
		//void Create(DirectorySecurity directorySecurity);
		DirectoryInfo CreateSubdirectory(String path);
		//DirectoryInfo CreateSubdirectory(String path, DirectorySecurity directorySecurity);
		void Delete(bool recursive);
		void Delete();
		//IEnumerable<DirectoryInfo> EnumerateDirectories();
		//IEnumerable<DirectoryInfo> EnumerateDirectories(String searchPattern, SearchOption searchOption);
		//IEnumerable<DirectoryInfo> EnumerateDirectories(String searchPattern);
		//IEnumerable<FileInfo> EnumerateFiles(String searchPattern, SearchOption searchOption);
		//IEnumerable<FileInfo> EnumerateFiles(String searchPattern);
		//IEnumerable<FileInfo> EnumerateFiles();
		//IEnumerable<FileSystemInfo> EnumerateFileSystemInfos(String searchPattern, SearchOption searchOption);
		//IEnumerable<FileSystemInfo> EnumerateFileSystemInfos();
		//IEnumerable<FileSystemInfo> EnumerateFileSystemInfos(String searchPattern);
		//DirectorySecurity GetAccessControl(AccessControlSections includeSections);
		//DirectorySecurity GetAccessControl();
		std::vector<DirectoryInfo> GetDirectories();
		//DirectoryInfo[] GetDirectories(String searchPattern, SearchOption searchOption);
		//DirectoryInfo[] GetDirectories(String searchPattern);
		std::vector<FileInfo> GetFiles();
		//std::vector<FileInfo> GetFiles(String searchPattern, SearchOption searchOption);
		std::vector<FileInfo> GetFiles(String searchPattern);
		//FileSystemInfo GetFileSystemInfos(String searchPattern, SearchOption searchOption);
		FileSystemInfo GetFileSystemInfos();
		FileSystemInfo GetFileSystemInfos(String searchPattern);
		void MoveTo(String destDirName);
		//void SetAccessControl(DirectorySecurity directorySecurity);
		
		String ToString();
	};

}
