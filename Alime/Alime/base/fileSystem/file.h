#pragma once

#include <vector>
#include <String>

#include <Alime/base/fileSystem/file_define.h>
#include <Alime/base/fileSystem/filePath.h>
#include <Alime/base/fileSystem/IStream.h>


namespace Alime::base::System::IO
{
	class FileStream;

	enum class FileAttributes
	{
		ReadOnly = 1,
		Hidden = 2,
		System = 4,
		Directory = 16,
		Archive = 32,
		Device = 64,
		Normal = 128,
		Temporary = 256,
		SparseFile = 512,
		ReparsePoint = 1024,
		Compressed = 2048,
		Offline = 4096,
		NotContentIndexed = 8192,
		Encrypted = 16384,
		//IntegrityStream = 32768, // win32 did not support here
		//NoScrubData = 131072
	};

	class File
	{
	private:
		File()=delete;
		~File() = delete;
	public:
		//using String = const std::wstring&;
		static void AppendAllLines(String path, std::vector<String> contents);
		static void AppendAllLines(String path, std::vector<String> contents, Encoding encoding);
		static void AppendAllText(String path, String contents);
		static void AppendAllText(String path, String contents, Encoding encoding);
		//static StreamWriter AppendText(String path);
		static void Copy(String sourceFileName, String destFileName);
		static void Copy(String sourceFileName, String destFileName, bool overwrite);
		static FileStream Create(String path);
		static FileStream Create(String path, int bufferSize);
		//static FileStream Create(String path, int bufferSize, FileOptions options);
		//static StreamWriter CreateText(String path);
		static void Decrypt(String path);
		static bool Delete(String path);
		static void Encrypt(String path);
		static bool Exists(String path);
		static FileAttributes GetAttributes(String path);
		//static DateTime GetCreationTime(String path);
		//static DateTime GetCreationTimeUtc(String path);
		//static DateTime GetLastAccessTime(String path);
		//static DateTime GetLastAccessTimeUtc(String path);
		//static DateTime GetLastWriteTime(String path);
		//static DateTime GetLastWriteTimeUtc(String path);
		static void Move(String sourceFileName, String destFileName);
		static void Move(String sourceFileName, String destFileName, bool overwrite);
		static FileStream Open(String path, FileMode mode);
		static FileStream Open(String path, FileMode mode, FileAccess access);
		static FileStream Open(String path, FileMode mode, FileAccess access, FileShare share);
		static FileStream OpenRead(String path);
		//static StreamReader OpenText(String path);
		static FileStream OpenWrite(String path);
		static std::vector<abyte> ReadAllBytes(String path);
		static std::vector<String> ReadAllLines(String path);
		static std::vector<String> ReadAllLines(String path, Encoding encoding);
		static String ReadAllText(String path);
		static String ReadAllText(String path, Encoding encoding);
		static void Replace(String sourceFileName, String destinationFileName, String destinationBackupFileName);
		static void Replace(String sourceFileName, String destinationFileName, String destinationBackupFileName, bool ignoreMetadataErrors);
		static void SetAttributes(String path, FileAttributes fileAttributes);
		//static void SetCreationTime(String path, DateTime creationTime);
		//static void SetCreationTimeUtc(String path, DateTime creationTimeUtc);
		//static void SetLastAccessTime(String path, DateTime lastAccessTime);
		//static void SetLastAccessTimeUtc(String path, DateTime lastAccessTimeUtc);
		//static void SetLastWriteTime(String path, DateTime lastWriteTime);
		//static void SetLastWriteTimeUtc(String path, DateTime lastWriteTimeUtc);
		static void WriteAllBytes(String path, std::vector<abyte> bytes);
		static void WriteAllLines(String path, std::vector<String> contents);
		static void WriteAllLines(String path, std::vector<String> contents, Encoding encoding);
		static void WriteAllText(String path, String contents);
		static void WriteAllText(String path, String contents, Encoding encoding);
	};
}

