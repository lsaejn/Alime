#include "FileStream.h"

#include <Alime/base/exceptionBase.h>
#include <Alime/base/fileSystem/File.h>

//https://docs.microsoft.com/en-ca/cpp/c-runtime-library/reference/fsopen-wfsopen?f1url=https%3A%2F%2Fmsdn.microsoft.com%2Fquery%2Fdev16.query%3FappId%3DDev16IDEF1&l=ZH-CN&k=k(CORECRT_WSTDIO%2F_wfsopen);k(_wfsopen);k(DevLang-C%20%20);k(TargetOS-Windows)&rd=true&view=vs-2019

namespace Alime::base::System::IO
{

	FileStream::FileStream(const String& fileName, FileMode fileMode, FileAccess access, FileShare share)
		:accessRight_(access)
	{
		Init(fileName, fileMode, access, share);
	}

	FileStream::FileStream(const String& fileName, FileMode fileMode)
		: accessRight_(FileAccess::Read)
	{
		Init(fileName, fileMode, FileAccess::ReadWrite, FileShare::ReadWrite);
	}

	FileStream::FileStream(FileStream&& fs)
	{
		file_ = fs.file_;
		accessRight_ = fs.accessRight_;
		fs.file_ = nullptr;
	}

	FileStream& FileStream::operator=(FileStream&& fs)
	{
		file_ = fs.file_;
		accessRight_ = fs.accessRight_;
		return *this;
	}

	FileStream::~FileStream()
	{
		Close();
	}

	void	FileStream::Close()
	{
		if(file_)
		{
			fclose(file_);
			file_=nullptr;
		}
	}

	void FileStream::Init(const String& fileName, FileMode fileMode, FileAccess access, FileShare share)
	{
		const wchar_t* mode = L"rb";//or rt
		switch (fileMode)
		{
		case FileMode::Create:
			if (access == FileAccess::Read)
				throw Error(L"Combining FileMode: Create with FileAccess: Read is invalid. ");
			else if (access == FileAccess::ReadWrite)
			{
				mode = L"w+b";
				accessRight_ = FileAccess::ReadWrite;
			}
			else
			{
				mode = L"wb";
				accessRight_ = FileAccess::Write;
			}
			break;
		case FileMode::Open:
			if(!File::Exists(fileName))//if (!FilePathIsExist(fileName, false))
			{
				throw Error(L"Failed opening '" + fileName + L"', file_ do not exist.");
			}
			if (access == FileAccess::Read)
			{
				mode = L"rb";
				accessRight_ = FileAccess::Read;
			}
			else if (access == FileAccess::ReadWrite)
			{
				mode = L"r+b";
				accessRight_ = FileAccess::ReadWrite;
			}
			else
			{
				mode = L"r+b";//给file_读权限，但读数据时，抛出异常
				accessRight_ = FileAccess::Write;
			}
			break;
		case FileMode::CreateNew:
			if (!File::Exists(fileName))
			{
				throw Error(L"Failed opening '" + fileName + L"', file_ already exists.");
			}
			if (access == FileAccess::Read)
				throw Error(L"Read-only access is incompatible with Create mode.");
			else if (access == FileAccess::ReadWrite)
			{
				mode = L"r+b";
				accessRight_ = FileAccess::ReadWrite;
			}
			else
			{
				mode = L"r+b";
				accessRight_ = FileAccess::Write;
			}
			break;
		case FileMode::Append:
			if (access == FileAccess::Read)
				throw Error(L"Read-only access is incompatible with Append mode.");
			else if (access == FileAccess::ReadWrite)
			{
				mode = L"a+b";
				accessRight_ = FileAccess::ReadWrite;
			}
			else if(access == FileAccess::Write)
			{
				mode = L"ab";
				accessRight_ = FileAccess::Write;
			}
			break;
		case FileMode::OpenOrCreate:
			if (access == FileAccess::Read)
			{
				throw Error(L"Read-only access is incompatible with OpenOrCreate mode.");
			}
			else if (access == FileAccess::ReadWrite)
			{
				mode = L"w+b";
				accessRight_ = FileAccess::ReadWrite;
			}
			else if (access == FileAccess::Write)
			{
				mode = L"w+b";
				accessRight_ = FileAccess::Write;
			}
			break;
		case FileMode::Truncate:
			if (access == FileAccess::Read)
			{
				throw Error(L"Read-only access is incompatible with Truncate mode.");
			}
			else if (access == FileAccess::ReadWrite)
			{
				mode = L"w+b";
				accessRight_ = FileAccess::ReadWrite;
			}
			else if (access == FileAccess::Write)
			{
				mode = L"wb";
				accessRight_ = FileAccess::Write;
			}
			break;
		default:
			break;
		}
		int shFlag = _SH_DENYWR;
		switch (share)
		{
		case FileShare::None:
			shFlag = _SH_DENYRW;
			break;
		case FileShare::Read:
			shFlag = _SH_DENYWR;
			break;
		case FileShare::Write:
			shFlag = _SH_DENYRD;
			break;
		case FileShare::ReadWrite:
			shFlag = _SH_DENYNO;
			break;
		default:
			throw Error(L"Invalid file_ share mode.");
			break;
		}
		file_ = _wfsopen(fileName.c_str(), mode, shFlag);
	}

	bool FileStream::CanRead()const
	{
		return file_ != 0 && (accessRight_ == FileAccess::Read || accessRight_ == FileAccess::ReadWrite);
	}

	bool FileStream::CanWrite()const
	{
		return file_ != 0 && (accessRight_ == FileAccess::Write || accessRight_ == FileAccess::ReadWrite);
	}

	bool FileStream::CanSeek()const
	{
		return file_ != 0;
	}

	bool FileStream::CanPeek()const
	{
		return file_ != 0 && (accessRight_ == FileAccess::Read || accessRight_ == FileAccess::ReadWrite);
	}

	bool FileStream::IsLimited()const
	{
		return file_ != 0 && accessRight_ == FileAccess::Read;
	}

	bool FileStream::IsAvailable()const
	{
		return file_ != 0;
	}

	pos_t FileStream::Position() const
	{
		if (file_ != 0)
		{
			fpos_t position = 0;
			if (fgetpos(file_, &position) == 0)
			{
				return position;
			}
		}
		return -1;
	}

	pos_t FileStream::Size() const
	{
		if (file_ != 0)
		{
			fpos_t position = 0;
			if (fgetpos(file_, &position) == 0)
			{
				if (fseek(file_, 0, SEEK_END) == 0)
				{
					pos_t size = Position();
					if (fsetpos(file_, &position) == 0)
					{
						return size;
					}
				}
			}
		}
		return -1;
	}

	void FileStream::Seek(pos_t _size)
	{
		if (Position() + _size > Size())
		{
			_fseeki64(file_, 0, SEEK_END);
		}
		else if (Position() + _size < 0)
		{
			_fseeki64(file_, 0, SEEK_SET);
		}
		else
		{
			_fseeki64(file_, _size, SEEK_CUR);
		}
	}

	void FileStream::SeekFromBegin(pos_t _size)
	{
		if (_size > Size())
		{
			_fseeki64(file_, 0, SEEK_END);
		}
		else if (_size < 0)
		{
			_fseeki64(file_, 0, SEEK_SET);
		}
		else
		{
			_fseeki64(file_, _size, SEEK_SET);
		}
	}

	void FileStream::SeekFromEnd(pos_t _size)
	{
		if (_size < 0)
		{
			_fseeki64(file_, 0, SEEK_END);
		}
		else if (_size > Size())
		{
			_fseeki64(file_, 0, SEEK_SET);
		}
		else
		{
			_fseeki64(file_, -_size, SEEK_END);
		}
	}

	aint FileStream::Read(void* _buffer, aint _size)
	{
		//CHECK_ERROR(file_ != 0, L"FileStream::Read(pos_t)#Stream is closed, cannot perform this operation.");
		//CHECK_ERROR(_size >= 0, L"FileStream::Read(void*, vint)#Argument size cannot be negative.");
		if (!CanRead())
			throw Error(L"No Access to Write");
		return fread(_buffer, 1, _size, file_);
	}

	aint FileStream::Write(void* _buffer, aint _size)
	{
		//CHECK_ERROR(file_ != 0, L"FileStream::Write(pos_t)#Stream is closed, cannot perform this operation.");
		//CHECK_ERROR(_size >= 0, L"FileStream::Write(void*, vint)#Argument size cannot be negative.");
		if(!CanWrite())
			throw Error(L"No Access to Write");
		return fwrite(_buffer, 1, _size, file_);
	}

	aint FileStream::Peek(void* _buffer, aint _size)
	{
		//CHECK_ERROR(file_ != 0, L"FileStream::Peek(pos_t)#Stream is closed, cannot perform this operation.");
		//CHECK_ERROR(_size >= 0, L"FileStream::Peek(void*, vint)#Argument size cannot be negative.");

		fpos_t position = 0;
		if (fgetpos(file_, &position) == 0)
		{
			size_t count = fread(_buffer, 1, _size, file_);
			if (fsetpos(file_, &position) == 0)
			{
				return count;
			}
		}
		return -1;
	}

	void FileStream::CheckFileHandle()
	{
		if(file_ != 0)
			throw Error(L"Stream is closed, cannot perform this operation");
	}

}