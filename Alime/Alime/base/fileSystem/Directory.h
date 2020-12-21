#pragma once
#include <Alime/base/fileSystem/FileSystemInfo.h>
#include <Alime/base/fileSystem/DirectoryInfo.h>

namespace Alime::base::System::IO
{

	class Directory
	{
	public:
		static DirectoryInfo CreateDirectory_(String path);
		//static DirectoryInfo CreateDirectory(String path, DirectorySecurity directorySecurity);
		static void Delete(String path, bool recursive);
		static void Delete(String path);
		//static IEnumerable<String> EnumerateDirectories(String path);
		//static IEnumerable<String> EnumerateDirectories(String path, String searchPattern);
		//static IEnumerable<String> EnumerateDirectories(String path, String searchPattern, SearchOption searchOption);
		//static IEnumerable<String> EnumerateFiles(String path);
		//static IEnumerable<String> EnumerateFiles(String path, String searchPattern);
		//static IEnumerable<String> EnumerateFiles(String path, String searchPattern, SearchOption searchOption);
		//static IEnumerable<String> EnumerateFileSystemEntries(String path);
		//static IEnumerable<String> EnumerateFileSystemEntries(String path, String searchPattern, SearchOption searchOption)
		//static IEnumerable<String> EnumerateFileSystemEntries(String path, String searchPattern);
		static bool Exists(String path);
		//static DirectorySecurity GetAccessControl(String path);
		//static DirectorySecurity GetAccessControl(String path, AccessControlSections includeSections);
		static DateTime GetCreationTime(String path);
		static DateTime GetCreationTimeUtc(String path);
		static String GetCurrentDirectory_();
		//static std::vector<String> GetDirectories(String path, String searchPattern, SearchOption searchOption);
		static std::vector<String> GetDirectories(String path, String searchPattern);
		static std::vector<String> GetDirectories(String path);
		static String GetDirectoryRoot(String path);
		//static std::vector<String> GetFiles(String path, String searchPattern, SearchOption searchOption);
		static std::vector<String> GetFiles(String path);
		static std::vector<String> GetFiles(String path, String searchPattern);
		static std::vector<String> GetFileSystemEntries(String path);
		static std::vector<String> GetFileSystemEntries(String path, String searchPattern);
		//static std::vector<String> GetFileSystemEntries(String path, String searchPattern, SearchOption searchOption);
		static DateTime GetLastAccessTime(String path);
		static DateTime GetLastAccessTimeUtc(String path);
		static DateTime GetLastWriteTime(String path);
		static DateTime GetLastWriteTimeUtc(String path);
		static std::vector<String> GetLogicalDrives();
		static DirectoryInfo GetParent(String path);
		static void Move(String sourceDirName, String destDirName);
		//static void SetAccessControl(String path, DirectorySecurity directorySecurity);
		static void SetCreationTime(String path, DateTime creationTime);
		static void SetCreationTimeUtc(String path, DateTime creationTimeUtc);
		static void SetCurrentDirectory_(String path);
		static void SetLastAccessTime(String path, DateTime lastAccessTime);
		static void SetLastAccessTimeUtc(String path, DateTime lastAccessTimeUtc);
		static void SetLastWriteTime(String path, DateTime lastWriteTime);
		static void SetLastWriteTimeUtc(String path, DateTime lastWriteTimeUtc);
	};

}