#pragma once

#include <list>
#include <string>

#include <Alime/base/fileSystem/file_define.h>

namespace Alime::base
{

	// Null-terminated array of separators used to separate components in path.
	// Each character in this array is a valid separator
	extern const PathChar kFilePathSeparators[];
	// A special path component meaning "this directory."
	extern const PathChar kFilePathCurrentDirectory[];
	// A special path component meaning "the parent directory."
	extern const PathChar kFilePathParentDirectory[];
	// The character used to identify a file extension.
	extern const PathChar kFilePathExtensionSeparator;

	// Check the character is filepath separator
	bool IsFilePathSeparator(const PathChar separator);
	bool IsFilePathSeparator(const PathString& separator);

	// Get the file extension from filepath
	bool FilePathExtension(const PathString& filepath_in, PathString& extension_out);

	// Get the directory from the whole filepath
	bool FilePathApartDirectory(const PathString& filepath_in, PathString& directory_out);
	// Get the filename from the whole filepath
	bool FilePathApartFileName(const PathString& filepath_in, PathString& filename_out);

	// Parse and extract all components of a path
	bool ParsePathComponents(const PathChar* PathChar, std::list<PathString>& components);

	// Check if |component| is a directory
	bool IsDirectoryComponent(const PathString& component);

	// Compose the filepath from directory and filename
	bool FilePathCompose(const PathString& directory_in, const PathString& filename_in, PathString& filepath_out);

	// Get the current filepath
	bool FilePathCurrentDirectory(PathString& directory_out);

	// Check the filepath is exist
	// If |is_directory| is true, check a directory, or check the path
	bool FilePathIsExist(const PathChar* filepath_in, bool is_directory);
	bool FilePathIsExist(const PathString& filepath_in, bool is_directory);

	// Wrapper for fopen-like calls. Returns non-NULL FILE* on success
	FILE* OpenFile(const PathChar* filepath, const PathChar* mode);
	FILE* OpenFile(const PathString& filepath, const PathChar* mode);

	// Closes file opened by OpenFile. Returns true on success
	bool CloseFile(FILE* file);

	// Reads the given number of bytes from the file into the buffer
	int ReadFile(const PathChar* filepath, void* data_out, size_t size);
	int ReadFile(const PathString& filepath, void* data_out, size_t size);


	// Read the file at |path| into |contents|, returning true on success.
	bool ReadFileToString(const PathString& filepath, std::string& contents_out);

	// Writes the content of given buffer into the file
	int WriteFile(const PathChar* filepath, const void* data, size_t size);
	int WriteFile(const PathString& filepath, const std::string& data);


	// Create a directory, all subdirs will be created if not existing
	bool CreateADirectory(const PathString& full_path);

	// Copies a single file.
	bool CopyAFile(const PathString& from_path, const PathString& to_path);

	// Deletes the given path.
	bool DeleteAFile(const PathString& filepath);

	// Get file size.
	int64_t GetFileSize(const PathString& filepath);

}