#pragma once

//#include "Interfaces.h"

#include "../fileSystem/file_define.h"
#include "Alime/base/fileSystem/IStream.h"

namespace Alime::base::System::IO
{
		/// <summary>Base type of all character encoder.</summary>
		class CharEncoder : public IEncoder
		{
		protected:
			IStream* stream;
			auint8 cacheBuffer[sizeof(wchar_t)];
			aint cacheSize;

			/// <summary>
			/// 向stream写入chars个wchar_t
			/// </summary>
			/// <param name="_buffer">数据指针</param>
			/// <param name="chars">字符个数</param>
			/// <returns>写入的字符数</returns>
			virtual aint	 WriteString(wchar_t* _buffer, aint chars)=0;
		public:
			CharEncoder();
			void Setup(IStream* _stream);
			void Close();

			/// <summary>
			/// 写入_size个字节，多余字节被cache
			/// </summary>
			/// <param name="_buffer">字节流</param>
			/// <param name="_size">字节数</param>
			/// <returns>成功写入的字节数</returns>
			aint Write(void* _buffer, aint _size);
		};
		
		/// <summary>Base type of all character decoder.</summary>
		class CharDecoder : public IDecoder
		{
		protected:
			IStream* stream;
			auint8 cacheBuffer[sizeof(wchar_t)];
			aint cacheSize;

			/// <summary>
			/// Read data to _buffer from stream
			/// </summary>
			/// <param name="_buffer"></param>
			/// <param name="chars"></param>
			/// <returns></returns>
			virtual aint	 ReadString(wchar_t* _buffer, aint chars)=0;
		public:
			CharDecoder();

			void Setup(IStream* _stream);
			void Close();
			/// <summary>
			/// 
			/// </summary>
			/// <param name="_buffer"></param>
			/// <param name="_size"></param>
			/// <returns>读取的字节数</returns>
			aint Read(void* _buffer, aint _size);
		};

/***********************************************************************
Mbcs
***********************************************************************/
		
		/// <summary>Encoder to transform text in a local code page from wchar_t.</summary>
		class MbcsEncoder : public CharEncoder
		{
		protected:
			aint WriteString(wchar_t* _buffer, aint chars);
		};
		
		/// <summary>Encoder to transform text in a local code page to wchar_t.</summary>
		class MbcsDecoder : public CharDecoder
		{
		protected:
			aint ReadString(wchar_t* _buffer, aint chars);
		};

/***********************************************************************
Utf-16
***********************************************************************/
		
		/// <summary>Encoder to transform UTF-16 text from wchar_t.</summary>
		class Utf16Encoder : public CharEncoder
		{
		protected:
			aint WriteString(wchar_t* _buffer, aint chars);
		};
		
		/// <summary>Decoder to transform UTF-16 text to wchar_t.</summary>
		class Utf16Decoder : public CharDecoder
		{
		protected:
			aint ReadString(wchar_t* _buffer, aint chars);
		};

/***********************************************************************
Utf-16-be
***********************************************************************/
		
		/// <summary>Encoder to transform big endian UTF-16 text from wchar_t.</summary>
		class Utf16BEEncoder : public CharEncoder
		{
		protected:
			aint WriteString(wchar_t* _buffer, aint chars);
		};
		
		/// <summary>Decoder to transform big endian UTF-16 text to wchar_t.</summary>
		class Utf16BEDecoder : public CharDecoder
		{
		protected:
			aint ReadString(wchar_t* _buffer, aint chars);
		};

/***********************************************************************
Utf-8
***********************************************************************/
		
		/// <summary>Encoder to transform UTF-8 text from wchar_t.</summary>
		class Utf8Encoder : public CharEncoder
		{
		protected:
			aint WriteString(wchar_t* _buffer, aint chars);
		};
		
		/// <summary>Decoder to transform UTF-8 text to wchar_t.</summary>
		class Utf8Decoder : public CharDecoder
		{
		protected:

			wchar_t	 cache;
			bool cacheAvailable;

			aint ReadString(wchar_t* _buffer, aint chars);
		public:
			Utf8Decoder();
		};

/***********************************************************************
Bom
***********************************************************************/
		
		/// <summary>Encoder to transform text from wchar_t. A BOM will be added at the beginning.</summary>
		class BomEncoder : public IEncoder
		{
		protected:
			Encoding encoding;
			IEncoder* encoder;
		public:
			/// <summary>Create an encoder.</summary>
			/// <param name="_encoding">Specified encoding.</param>
			BomEncoder(Encoding _encoding);
			~BomEncoder();

			void Setup(IStream* _stream);
			void Close();
			aint Write(void* _buffer, aint _size);
		};
		
		/// <summary>Decoder to transform text to wchar_t. This decoder depends on the BOM information at the beginning to decide the format of the input.</summary>
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
				pos_t	Position()const;
				pos_t	Size()const;
				void	Seek(pos_t _size);
				void	SeekFromBegin(pos_t _size);
				void	SeekFromEnd(pos_t _size);
				aint	Read(void* _buffer, aint _size);
				aint	Write(void* _buffer, aint _size);
				aint	Peek(void* _buffer, aint _size);
			};
		protected:
			IDecoder* decoder;
			IStream* stream;

		public:
			/// <summary>Create an decoder.</summary>
			BomDecoder();
			~BomDecoder();

			void Setup(IStream* _stream);
			void Close();
			aint Read(void* _buffer, aint _size);
		};

/***********************************************************************
Encoding Test
***********************************************************************/

		/// <summary>Guess the text encoding in a buffer.</summary>
		/// <param name="buffer">The buffer to guess.</param>
		/// <param name="size">Size of the buffer in bytes.</param>
		/// <param name="encoding">Returns the most possible encoding.</param>
		/// <param name="containsBom">Returns true if the BOM information is at the beginning of the buffer.</param>
		extern void	TestEncoding(unsigned char* buffer, aint size, Encoding& encoding, bool& containsBom);

}


