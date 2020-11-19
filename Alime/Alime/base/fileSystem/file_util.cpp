#include <memory>


#if defined(OS_POSIX)
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#endif  // OS_POSIX

#include "windows.h"
#include <Alime/base/fileSystem/file_util.h>


class ScopedWinHandle
{
public:

	ScopedWinHandle() : handle_(INVALID_HANDLE_VALUE) {}
	ScopedWinHandle(HANDLE handle) : handle_(handle) {}
	~ScopedWinHandle() { Reset(INVALID_HANDLE_VALUE); }

	bool Valid() const { return handle_ != INVALID_HANDLE_VALUE; }
	HANDLE Get() const { return handle_; }
	HANDLE Release() { HANDLE old_handle = handle_; handle_ = INVALID_HANDLE_VALUE; return old_handle; }
	void Reset(HANDLE handle) { if (Valid()) ::CloseHandle(handle_); handle_ = handle; }
	HANDLE* operator&() { return &handle_; }
	operator HANDLE() const { return handle_; }

private:
	HANDLE handle_;

	//DISALLOW_COPY_AND_ASSIGN(ScopedWinHandle);
};

namespace Alime::base
{

#if defined(OS_POSIX)
	const Char kEndChar = '\0';
	const Char kFilePathSeparators[] = "/";
	const Char kFilePathCurrentDirectory[] = ".";
	const Char kFilePathParentDirectory[] = "..";
	const Char kFilePathExtensionSeparator = '.';
#elif defined(OS_WIN)
	const Char kEndChar = L'\0';
	const Char kFilePathSeparators[] = L"\\/";
	const Char kFilePathCurrentDirectory[] = L".";
	const Char kFilePathParentDirectory[] = L"..";
	const Char kFilePathExtensionSeparator = L'.';
#endif  // OS_WIN

	bool FilePathCurrentDirectory(String& directory_out)
	{
		Char directory[MAX_PATH];
		directory[0] = 0;
		DWORD len = ::GetCurrentDirectoryW(MAX_PATH, directory);
		if (len == 0 || len > MAX_PATH)
		{
			return false;
		}
		directory_out = directory;
		directory_out.append(1, kFilePathSeparators[0]);

		return true;
	}

	bool IsFilePathSeparator(const Char separator)
	{
		if (separator == kEndChar)
			return false;

		size_t len = sizeof(kFilePathSeparators) / sizeof(Char);
		for (size_t i = 0; i < len; i++)
		{
			if (separator == kFilePathSeparators[i])
				return true;
		}

		return false;
	}

	bool IsFilePathSeparator(const String& separator)
	{
		if (separator.empty())
			return false;
		Char c = separator[0];
		return IsFilePathSeparator(c);
	}

	bool FilePathExtension(const String& filepath_in, String& extension_out)
	{
		if (filepath_in.size() == 0)
			return false;
		bool ret = false;
		String file_name;
		if (FilePathApartFileName(filepath_in, file_name))
		{
			size_t pos = file_name.rfind(kFilePathExtensionSeparator);
			if (pos != String::npos)
			{
				extension_out = file_name.substr(pos, String::npos);
				ret = true;
			}
		}
		return ret;
	}

	bool FilePathApartDirectory(const String& filepath_in,
		String& directory_out)
	{
		size_t index = filepath_in.size() - 1;
		if (index <= 0 || filepath_in.size() == 0)
			return false;
		for (; index != 0; index--)
		{
			if (IsFilePathSeparator(filepath_in[index]))
			{
				if (index == filepath_in.size() - 1)
					directory_out = filepath_in;
				else
					directory_out = filepath_in.substr(0, index + 1);
				return true;
			}
		}
		return false;
	}

	bool FilePathApartFileName(const String& filepath_in,
		String& filename_out)
	{
		if (filepath_in.size() == 0)
			return false;
		bool ret = true;
		size_t separator_pos = String::npos;
		size_t separators_count = sizeof(kFilePathSeparators) / sizeof(Char);
		for (size_t index = 0; index < separators_count; index++)
		{
			separator_pos = filepath_in.rfind(kFilePathSeparators[index]);
			if (separator_pos != String::npos)
				break;
		}
		if (separator_pos++ != String::npos && separator_pos < filepath_in.size())
			filename_out = filepath_in.substr(separator_pos);
		else if (separator_pos >= filepath_in.size())
			ret = false;
		else
			filename_out = filepath_in;
		return ret;
	}

	template<typename CharType>
	bool ParsePathComponentsT(const CharType* path,
		const CharType* seperators,
		std::list<std::basic_string<CharType> >& components)
	{
		components.clear();
		if (path == NULL)
			return false;
		const CharType* prev = NULL;
		const CharType* next = path;
		const CharType* c;
		while (*next)
		{
			prev = next;
			// find the first seperator
			for (;;)
			{
				for (c = seperators; *c && *next != *c; c++);
				if (*c || !*next)
					break;
				next++;
			}
			components.push_back(std::basic_string<CharType>(prev, next - prev));
			if (*next)
				components.back().push_back(*seperators);
			// skip duplicated seperators
			for (++next;;)
			{
				for (c = seperators; *c && *next != *c; c++);
				if (!*c)
					break;
				next++;
			}
		}
		return true;
	}

	bool ParsePathComponents(const Char* path,
		std::list<String>& components)
	{
		return ParsePathComponentsT<Char>(path,
			kFilePathSeparators,
			components);
	}

	bool IsDirectoryComponent(const String& component)
	{
		if (component.empty())
			return false;
#if defined(OS_WIN)
		return *component.rbegin() == kFilePathSeparators[0] ||
			*component.rbegin() == kFilePathSeparators[1];
#else
		return *component.rbegin() == kFilePathSeparators[0];
#endif // OS_WIN
	}

	bool FilePathCompose(const String& directory_in,
		const String& filename_in,
		String& filepath_out)
	{
		String directory;
		if (!FilePathApartDirectory(directory_in, directory))
			return false;
		filepath_out = directory + filename_in;
		return true;
	}

	bool FilePathIsExist(const String& filepath_in, bool is_directory)
	{
		return FilePathIsExist((const Char*)filepath_in.c_str(), is_directory);
	}

	bool FilePathIsExist(const Char* filepath_in, bool is_directory)
	{
		const DWORD file_attr = ::GetFileAttributesW(filepath_in);
		if (file_attr != INVALID_FILE_ATTRIBUTES)
		{
			if (is_directory)
				return (file_attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
			else
				return true;
		}
		return false;
	}

	FILE* OpenFile(const String& filepath, const Char* mode)
	{
		return OpenFile(filepath.c_str(), mode);
	}

	FILE* OpenFile(const Char* filepath, const Char* mode)
	{
		return _wfsopen(filepath, mode, _SH_DENYNO);
	}

	bool CloseFile(FILE* file)
	{
		if (NULL == file)
			return true;
		return fclose(file) == 0;
	}

	int ReadFile(const String& filepath, void* data_out, size_t size)
	{
		return ReadFile(filepath.c_str(), data_out, size);
	}

	int WriteFile(const String& filepath, const std::string& data)
	{
		return WriteFile(filepath.c_str(), data.c_str(), data.size());
	}

	int ReadFile(const Char* filepath, void* data_out, size_t size)
	{
		ScopedWinHandle file(CreateFileW(filepath,
			GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_SEQUENTIAL_SCAN,
			NULL));
		if (!file)
			return -1;

		DWORD read;
		if (::ReadFile(file, data_out, (DWORD)size, &read, NULL) &&
			static_cast<int>(read) == size)
			return read;
		return -1;
	}

	int WriteFile(const Char* filepath, const void* data, size_t size)
	{
		ScopedWinHandle file(CreateFileW(filepath,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			0,
			NULL));
		if (!file)
			return -1;

		DWORD written;
		BOOL result = ::WriteFile(file, data, (DWORD)size, &written, NULL);
		if (result && static_cast<int>(written) == size)
			return written;

		return -1;
	}

	bool ReadFileToString(const String& filepath, std::string& out)
	{
		std::unique_ptr<FILE/*, nbase::DeleterFileClose*/> file;
#if defined(OS_WIN)
		file.reset(OpenFile(filepath.c_str(), L"rb"));
#else
		file.reset(OpenFile(filepath.c_str(), "rb"));
#endif

		if (!file)
			return false;

		size_t file_size = (size_t)GetFileSize(filepath);
		if (file_size > size_t(-1)) //x64编译条件下，size_t的最大值转为int64_t会溢出，因此用无符号
			return false; // Too large

		bool read_ok = true;
		out.resize(file_size);
		if (!out.empty()) {
			read_ok = fread(&out[0], 1, out.size(), file.get()) == out.size();
		}

		return read_ok;
	}

	bool CopyAFile(const String& from_path, const String& to_path)
	{
		if (from_path.size() >= MAX_PATH ||
			to_path.size() >= MAX_PATH) {
			return false;
		}
		return (::CopyFileW(from_path.c_str(), to_path.c_str(),
			false) != 0);
	}

	bool DeleteAFile(const String& filepath)
	{
		if (::DeleteFileW(filepath.c_str()) != 0)
			return true;
		return false;
	}

	bool CreateDirectory(const String& full_path)
	{
		return CreateDirectory(full_path.c_str());
	}

	bool CreateDirectory(const Char* full_path)
	{
		if (full_path == nullptr)
			return false;

		if (FilePathIsExist(full_path, true))
			return true;

		std::list<String> subpaths;
		ParsePathComponents(full_path, subpaths);
		if (subpaths.empty())
			return false;

		// Collect a list of all parent directories.
		auto curr = subpaths.begin();
		for (auto prev = curr++; curr != subpaths.end(); curr++) {
			*curr = *prev + *curr;
			prev = curr;
		}

		// Iterate through the parents and create the missing ones.
		for (auto i = subpaths.begin(); i != subpaths.end(); ++i) {
			if (FilePathIsExist(i->c_str(), true))
				continue;
			if (!::CreateDirectoryW(i->c_str(), NULL)) {
				DWORD error = ::GetLastError();
				if (error == ERROR_ALREADY_EXISTS &&
					FilePathIsExist(i->c_str(), true)) {
					continue;
				}
				return false;
			}
		}
		return true;
	}

	int64_t GetFileSize(const String& filepath)
	{
		//fix me
		return 0;
	}

};  // namespace nbase
