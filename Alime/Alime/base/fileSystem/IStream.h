#pragma once

#include <Alime/base/fileSystem/file_define.h>

namespace Alime::base::System::IO
{
	class IStream
	{
	public:
		virtual bool CanRead() const = 0;
		virtual bool CanWrite() const = 0;
		virtual bool CanSeek() const = 0;
		virtual bool CanPeek() const = 0;
		virtual bool IsLimited() const = 0;
		virtual bool IsAvailable() const = 0;
		virtual void Close() = 0;

		virtual pos_t Position() const = 0;
		virtual pos_t Size() const = 0;

		virtual void Seek(pos_t _size) = 0;
		virtual void SeekFromEnd(pos_t _size) = 0;
		virtual void SeekFromBegin(pos_t _size) = 0;

		virtual aint Read(void* _buffer, aint _size) = 0;
		virtual aint Write(void* _buffer, aint _size) = 0;
		virtual aint Peek(void* _buffer, aint _size) = 0;
	};

	class IEncoder
	{
	public:
		virtual void Setup(IStream* _stream) = 0;
		virtual void Close() = 0;
		virtual int Write(void* _buffer, int _size) = 0;
	};

	class IDecoder
	{
	public:
		virtual void Setup(IStream* _stream) = 0;
		virtual void Close() = 0;
		virtual int Read(void* _buffer, int _size) = 0;
	};

}
