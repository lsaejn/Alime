#pragma once

#include <stdio.h>
#include <string>

#include <Alime/base/fileSystem/IStream.h>
#include <Alime/base/fileSystem/file_define.h>

namespace Alime::base::System::IO
{
	//fix me, copyable?
	/*
	* 没有提供buffer
	*/
	class FileStream : public virtual IStream
	{
	public:
		FileStream(const String& fileName,FileMode fileMode, FileAccess accessRight, FileShare share);
		FileStream(const String& fileName, FileMode fileMode=FileMode::Open);

		FileStream(FileStream&& fs);
		FileStream(const FileStream& fs)=delete;
		
		FileStream& operator=(const FileStream&) = delete;
		FileStream& operator=(FileStream&& fs);

		~FileStream();

		bool	CanRead()const;
		bool	CanWrite()const;
		bool	CanSeek()const;
		bool	CanPeek()const;
		bool	IsLimited()const;
		bool	IsAvailable()const;
		void	Close();
		pos_t Position() const;
		pos_t Size() const;
		void	Seek(pos_t _size);
		void	SeekFromBegin(pos_t _size);
		void	SeekFromEnd(pos_t _size);
		aint	Read(void* _buffer, aint _size);
		aint	Write(void* _buffer, aint  _size);
		aint	Peek(void* _buffer, aint _size);

	protected:
		void CheckFileHandle();

	protected:
		void Init(const String& fileName, FileMode fileMode, FileAccess _accessRight, FileShare share);
		FileAccess	accessRight_;
		FILE* file_;
	private:
		abyte* buffer[4096];
	};
}

