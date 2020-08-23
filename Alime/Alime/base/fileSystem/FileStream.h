#pragma once

#include <stdio.h>
#include <string>

#include <Alime/base/fileSystem/IStream.h>
#include <Alime/base/fileSystem/file_define.h>

namespace Alime::base
{
	class FileStream : public virtual IStream
	{
	public:
		using StringTy = std::wstring;
		using pos_t = int64_t;
	public:
		FileStream(const StringTy& fileName,FileMode fileMode, FileAccess accessRight, FileShare share);
		FileStream(const StringTy& fileName, FileMode fileMode=FileMode::Open);
		~FileStream();

		bool	CanRead()const;
		bool	CanWrite()const;
		bool	CanSeek()const;
		bool	CanPeek()const;
		bool	IsLimited()const;
		bool	IsAvailable()const;
		void	Close();
		pos_t	Position() const;
		pos_t	Size() const;
		void	Seek(pos_t _size);
		void	SeekFromBegin(pos_t _size);
		void	SeekFromEnd(pos_t _size);
		size_t		Read(void* _buffer, size_t _size);
		size_t		Write(void* _buffer, size_t _size);
		size_t		Peek(void* _buffer,  size_t _size);

	protected:
		void Init(const StringTy& fileName, FileMode fileMode, FileAccess _accessRight, FileShare share);
		FileAccess	accessRight_;
		FILE* file_;
	};
}

