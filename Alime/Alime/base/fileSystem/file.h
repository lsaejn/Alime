#pragma once

#include <vector>
#include <String>

#include <Alime/base/fileSystem/file_define.h>
#include <Alime/base/fileSystem/filePath.h>
#include <Alime/base/fileSystem/IStream.h>
#include <Alime/base/time/DateTime.h>
#include <Alime/base/fileSystem/StreamWriter.h>
#include <Alime/base/fileSystem/StreamReader.h>

namespace Alime::base::System::IO
{
	class FileStream;
	class File
	{
	private:
		File()=delete;
		~File() = delete;
	public:
		static void AppendAllLines(const String& path, std::vector<String> contents);
		static void AppendAllLines(const String& path, std::vector<String> contents, Encoding encoding);
		static void AppendAllText(const String& path, const String& contents);
		static void AppendAllText(const String& path, const String& contents, Encoding encoding);
		static StreamWriter AppendText(const String& path);
		static void Copy(const String& sourceFileName, const String& destFileName);
		static void Copy(const String& sourceFileName, const String& destFileName, bool overwrite);
		static FileStream Create(const String& path);
		static FileStream Create(const String& path, int bufferSize);
		//static FileStream Create(const String& path, int bufferSize, FileOptions options);
		static StreamWriter CreateText(const String& path);
		static void Decrypt(const String& path);
		static bool Delete(const String& path);
		static void Encrypt(const String& path);
		static bool Exists(const String& path);
		static FileAttributes GetAttributes(const String& path);
		static DateTime GetCreationTime(const String& path);
		static DateTime GetCreationTimeUtc(const String& path);
		static DateTime GetLastAccessTime(const String& path);
		static DateTime GetLastAccessTimeUtc(const String& path);
		static DateTime GetLastWriteTime(const String& path);
		static DateTime GetLastWriteTimeUtc(const String& path);
		static void Move(const String& sourceFileName, const String& destFileName);
		static void Move(const String& sourceFileName, const String& destFileName, bool overwrite);
		static FileStream Open(const String& path, FileMode mode);
		static FileStream Open(const String& path, FileMode mode, FileAccess access);
		static FileStream Open(const String& path, FileMode mode, FileAccess access, FileShare share);
		static FileStream OpenRead(const String& path);
		static StreamReader OpenText(const String& path);
		static FileStream OpenWrite(const String& path);
		static std::vector<abyte> ReadAllBytes(const String& path);
		static std::vector<String> ReadAllLines(const String& path);
		static std::vector<String> ReadAllLines(const String& path, Encoding encoding);
		static String ReadAllText(const String& path);
		static String ReadAllText(const String& path, Encoding encoding);
		static void Replace(const String& sourceFileName, const String& destinationFileName, const String& destinationBackupFileName);
		static void Replace(const String& sourceFileName, const String& destinationFileName, const String& destinationBackupFileName, bool ignoreMetadataErrors);
		static void SetAttributes(const String& path, FileAttributes fileAttributes);
		static void SetCreationTime(const String& path, DateTime creationTime);
		static void SetCreationTimeUtc(const String& path, DateTime creationTimeUtc);
		static void SetLastAccessTime(const String& path, DateTime lastAccessTime);
		static void SetLastAccessTimeUtc(const String& path, DateTime lastAccessTimeUtc);
		static void SetLastWriteTime(const String& path, DateTime lastWriteTime);
		static void SetLastWriteTimeUtc(const String& path, DateTime lastWriteTimeUtc);
		static void WriteAllBytes(const String& path, std::vector<abyte> bytes);
		static void WriteAllLines(const String& path, std::vector<String> contents);
		static void WriteAllLines(const String& path, std::vector<String> contents, Encoding encoding);
		static void WriteAllText(const String& path, const String& contents);
		static void WriteAllText(const String& path, const String& contents, Encoding encoding);
	};
}

