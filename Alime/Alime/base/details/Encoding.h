#include "../fileSystem/file_define.h"
#include <Alime/base/fileSystem/IStream.h>

namespace Alime::base::System::IO
{
	class IEncoder
	{
	public:
		virtual void Setup(IStream* _stream)=0;
		virtual void Close()=0;
		virtual aint	 Write(void* _buffer, aint _size)=0;
	};
		
	class IDecoder
	{
	public:
		virtual void Setup(IStream* _stream)=0;
		virtual void Close()=0;
		virtual aint	 Read(void* _buffer, aint _size)=0;
	};

	/// <summary>
	/// base class transate from some specified format to wchar_t
	/// </summary>
	class CharDecoder : public IDecoder
	{
	public:
		virtual void Setup(IStream* _stream);
		virtual void Close();
		virtual aint	 Read(void* _buffer, aint _size);
	protected:
		IStream* stream;
		auint8 cacheBuffer[sizeof(wchar_t)];
		aint cacheSize;
	};

	class Utf8Decoder : public CharDecoder
	{
	protected:
		wchar_t							cache;
		bool							cacheAvailable;
		aint ReadString(wchar_t* _buffer, aint chars);
	public:
		Utf8Decoder();
	};

	class BomDecoder : public IDecoder
	{
	private:
		class BomStream : public IStream
		{
		protected:
			IStream* stream;
			char bom[3];
			aint bomLength;
			aint bomPosition;
		public:
			BomStream(IStream* _stream, char* _bom, aint _bomLength);

			bool CanRead()const;
			bool CanWrite()const;
			bool CanSeek()const;
			bool CanPeek()const;
			bool IsLimited()const;
			bool IsAvailable()const;
			void Close();
			pos_t Position()const;
			pos_t Size()const;
			void Seek(pos_t _size);
			void SeekFromBegin(pos_t _size);
			void SeekFromEnd(pos_t _size);
			aint Read(void* _buffer, aint _size);
			aint Write(void* _buffer, aint _size);
			aint Peek(void* _buffer, aint _size);
		};
	protected:
		IDecoder* decoder;
		IStream* stream;

	public:
		BomDecoder();
		~BomDecoder();

		void Setup(IStream* _stream);
		void Close();
		aint Read(void* _buffer, aint _size);
	};

}