#pragma once

#include <list>
#include <string>

#include <Alime/base/fileSystem/file_define.h>

namespace Alime::base
{

	// Null-terminated array of separators used to separate components in path.
	// Each character in this array is a valid separator
	extern const Char kFilePathSeparators[];
	// A special path component meaning "this directory."
	extern const Char kFilePathCurrentDirectory[];
	// A special path component meaning "the parent directory."
	extern const Char kFilePathParentDirectory[];
	// The character used to identify a file extension.
	extern const Char kFilePathExtensionSeparator;

	// Check the character is filepath separator
	bool IsFilePathSeparator(const Char separator);
	bool IsFilePathSeparator(const String& separator);

	// Get the file extension from filepath
	bool FilePathExtension(const String& filepath_in, String& extension_out);

	// Get the directory from the whole filepath
	bool FilePathApartDirectory(const String& filepath_in, String& directory_out);
	// Get the filename from the whole filepath
	bool FilePathApartFileName(const String& filepath_in, String& filename_out);

	// Parse and extract all components of a path
	bool ParsePathComponents(const Char* Char, std::list<String>& components);

	// Check if |component| is a directory
	bool IsDirectoryComponent(const String& component);

	// Compose the filepath from directory and filename
	bool FilePathCompose(const String& directory_in, const String& filename_in, String& filepath_out);

	// Get the current filepath
	bool FilePathCurrentDirectory(String& directory_out);

	// Check the filepath is exist
	// If |is_directory| is true, check a directory, or check the path
	bool FilePathIsExist(const Char* filepath_in, bool is_directory);
	bool FilePathIsExist(const String& filepath_in, bool is_directory);

	// Wrapper for fopen-like calls. Returns non-NULL FILE* on success
	FILE* OpenFile(const Char* filepath, const Char* mode);
	FILE* OpenFile(const String& filepath, const Char* mode);

	// Closes file opened by OpenFile. Returns true on success
	bool CloseFile(FILE* file);

	// Reads the given number of bytes from the file into the buffer
	int ReadFile(const Char* filepath, void* data_out, size_t size);
	int ReadFile(const String& filepath, void* data_out, size_t size);


	// Read the file at |path| into |contents|, returning true on success.
	bool ReadFileToString(const String& filepath, std::string& contents_out);

	// Writes the content of given buffer into the file
	int WriteFile(const Char* filepath, const void* data, size_t size);
	int WriteFile(const String& filepath, const std::string& data);


	// Create a directory, all subdirs will be created if not existing
	bool CreateADirectory(const String& full_path);

	// Copies a single file.
	bool CopyAFile(const String& from_path, const String& to_path);

	// Deletes the given path.
	bool DeleteAFile(const String& filepath);

	// Get file size.
	int64_t GetFileSize(const String& filepath);

}