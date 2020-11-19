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
		using String = String;
	public:
		FileStream(const String& fileName,FileMode fileMode, FileAccess accessRight, FileShare share);
		FileStream(const String& fileName, FileMode fileMode=FileMode::Open);
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
		aint	Read(void* _buffer, aint _size);
		aint	Write(void* _buffer, aint  _size);
		aint	Peek(void* _buffer, aint _size);

	protected:
		void Init(const String& fileName, FileMode fileMode, FileAccess _accessRight, FileShare share);
		FileAccess	accessRight_;
		FILE* file_;
	};
}

