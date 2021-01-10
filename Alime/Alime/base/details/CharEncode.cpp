#include "CharEncode.h"
#include <windows.h>

namespace Alime::base::System::IO
{

/***********************************************************************
CharEncoder
***********************************************************************/

		CharEncoder::CharEncoder()
			:stream(0)
			,cacheSize(0)
		{
		}

		void CharEncoder::Setup(IStream* _stream)
		{
			stream=_stream;
		}

		void CharEncoder::Close()
		{
		}

		aint CharEncoder::Write(void* _buffer, aint _size)
		{
			const aint all=cacheSize+_size;
			const aint chars=all/sizeof(wchar_t);
			const aint bytes=chars*sizeof(wchar_t);
			wchar_t* unicode=0;
			bool needToFree=false;
			aint result=0;

			if(chars)
			{
				if(cacheSize>0)
				{
					unicode=new wchar_t[chars];
					memcpy(unicode, cacheBuffer, cacheSize);
					memcpy(((auint8*)unicode)+cacheSize, _buffer, bytes-cacheSize);
					needToFree=true;
				}
				else
				{
					unicode=(wchar_t*)_buffer;
				}
				result=WriteString(unicode, chars)*sizeof(wchar_t)-cacheSize;
				cacheSize=0;
			}

			if(needToFree)
			{
				delete[] unicode;
			}
			if(all-bytes>0)
			{
				cacheSize=all-bytes;
				memcpy(cacheBuffer, (auint8*)_buffer+_size-cacheSize, cacheSize);
				result+=cacheSize;
			}
			return result;
		}

/***********************************************************************
CharDecoder
***********************************************************************/

		CharDecoder::CharDecoder()
			:stream(0)
			,cacheSize(0)
		{
		}

		void CharDecoder::Setup(IStream* _stream)
		{
			stream=_stream;
		}

		void CharDecoder::Close()
		{
		}

		aint CharDecoder::Read(void* _buffer, aint _size)
		{
			auint8* unicode=(auint8*)_buffer;
			aint result=0;
			{
				aint index=0;
				while(cacheSize>0 && _size>0)
				{
					*unicode++=cacheBuffer[index]++;
					cacheSize--;
					_size--;
					result++;
				}
			}

			const aint chars=_size/sizeof(wchar_t);
			aint bytes=ReadString((wchar_t*)unicode, chars)*sizeof(wchar_t);
			result+=bytes;
			_size-=bytes;
			unicode+=bytes;

			if(_size>0)
			{
				wchar_t c;
				if(ReadString(&c, 1)==1)
				{
					cacheSize=sizeof(wchar_t)-_size;
					memcpy(unicode, &c, _size);
					memcpy(cacheBuffer, (auint8*)&c+_size, cacheSize);
					result+=_size;
				}
			}
			return result;
		}

/***********************************************************************
Mbcs
***********************************************************************/

		aint MbcsEncoder::WriteString(wchar_t* _buffer, aint chars)
		{
			aint length=WideCharToMultiByte(CP_THREAD_ACP, 0, _buffer, (int)chars, NULL, NULL, NULL, NULL);
			char* mbcs=new char[length];
			WideCharToMultiByte(CP_THREAD_ACP, 0, _buffer, (int)chars, mbcs, (int)length, NULL, NULL);
			aint result=stream->Write(mbcs, length);
			delete[] mbcs;
			if(result==length)
			{
				return chars;
			}
			else
			{
				Close();
				return 0;
			}
		}

		aint MbcsDecoder::ReadString(wchar_t* _buffer, aint chars)
		{
			char* source=new char[chars*2];
			char* reading=source;
			aint readed=0;
			while(readed<chars)
			{
				if(stream->Read(reading, 1)!=1)
				{
					break;
				}
				if(IsDBCSLeadByte(*reading))
				{
					if(stream->Read(reading+1, 1)!=1)
					{
						break;
					}
					reading+=2;
				}
				else
				{
					reading++;
				}
				readed++;
			}

			MultiByteToWideChar(CP_THREAD_ACP, 0, source, (int)(reading-source), _buffer, (int)chars);

			delete[] source;
			return readed;
		}

/***********************************************************************
Utf-16
***********************************************************************/

		aint Utf16Encoder::WriteString(wchar_t* _buffer, aint chars)
		{
			return stream->Write(_buffer, chars*sizeof(wchar_t))/sizeof(wchar_t);
		}

		aint Utf16Decoder::ReadString(wchar_t* _buffer, aint chars)
		{
			return stream->Read(_buffer, chars*sizeof(wchar_t))/sizeof(wchar_t);
		}

/***********************************************************************
Utf-16-be
***********************************************************************/

		aint Utf16BEEncoder::WriteString(wchar_t* _buffer, aint chars)
		{
			aint writed=0;
			while(writed<chars)
			{
				if(stream->Write(((unsigned char*)_buffer)+1, 1)!=1)
				{
					break;
				}
				if(stream->Write(_buffer, 1)!=1)
				{
					break;
				}
				_buffer++;
				writed++;
			}
			if(writed!=chars)
			{
				Close();
			}
			return writed;
		}

		aint Utf16BEDecoder::ReadString(wchar_t* _buffer, aint chars)
		{
			chars=stream->Read(_buffer, chars*sizeof(wchar_t))/sizeof(wchar_t);
			unsigned char* unicode=(unsigned char*)_buffer;
			for(aint i=0;i<chars;i++)
			{
				unsigned char t=unicode[0];
				unicode[0]=unicode[1];
				unicode[1]=t;
				unicode++;
			}
			return chars;
		}

/***********************************************************************
Utf8
***********************************************************************/

		aint Utf8Encoder::WriteString(wchar_t* _buffer, aint chars)
		{

			aint length=WideCharToMultiByte(CP_UTF8, 0, _buffer, (int)chars, NULL, NULL, NULL, NULL);
			char* mbcs=new char[length];
			WideCharToMultiByte(CP_UTF8, 0, _buffer, (int)chars, mbcs, (int)length, NULL, NULL);
			aint result=stream->Write(mbcs, length);
			delete[] mbcs;
			if(result==length)
			{
				return chars;
			}
			else
			{
				Close();
				return 0;
			}
		}

		Utf8Decoder::Utf8Decoder()
			:cache(0)
			,cacheAvailable(false)
		{
		}

		aint Utf8Decoder::ReadString(wchar_t* _buffer, aint chars)
		{
			auint8 source[4];

			wchar_t target[2];
			wchar_t* writing=_buffer;
			aint readed=0;
			aint sourceCount=0;

			while(readed<chars)
			{

				if(cacheAvailable)
				{
					*writing++=cache;
					cache=0;
					cacheAvailable=false;
				}
				else
				{
					if(stream->Read(source, 1)!=1)
					{
						break;
					}
					if((*source & 0xF0) == 0xF0)
					{
						if(stream->Read(source+1, 3)!=3)
						{
							break;
						}
						sourceCount=4;
					}
					else if((*source & 0xE0) == 0xE0)
					{
						if(stream->Read(source+1, 2)!=2)
						{
							break;
						}
						sourceCount=3;
					}
					else if((*source & 0xC0) == 0xC0)
					{
						if(stream->Read(source+1, 1)!=1)
						{
							break;
						}
						sourceCount=2;
					}
					else
					{
						sourceCount=1;
					}
					int targetCount=MultiByteToWideChar(CP_UTF8, 0, (char*)source, (int)sourceCount, target, 2);
					if(targetCount==1)
					{
						*writing++=target[0];
					}
					else if(targetCount==2)
					{
						*writing++=target[0];
						cache=target[1];
						cacheAvailable=true;
					}
					else
					{
						break;
					}
				}
				readed++;
			}
			return readed;
		}

/***********************************************************************
BomEncoder
***********************************************************************/

		BomEncoder::BomEncoder(Encoding _encoding)
			:encoding(_encoding)
			,encoder(0)
		{
			switch(encoding)
			{
			case Encoding::Mbcs:
				encoder=new MbcsEncoder;
				break;
			case Encoding::Utf8:
				encoder=new Utf8Encoder;
				break;
			case Encoding::Utf16:
				encoder=new Utf16Encoder;
				break;
			case Encoding::Utf16BE:
				encoder=new Utf16BEEncoder;
				break;
			}
		}

		BomEncoder::~BomEncoder()
		{
			Close();
		}

		void BomEncoder::Setup(IStream* _stream)
		{
			switch(encoding)
			{
			case Encoding::Mbcs:
				break;
			case Encoding::Utf8:
				_stream->Write((void*)"\xEF\xBB\xBF", 3);
				break;
			case Encoding::Utf16:
				_stream->Write((void*)"\xFF\xFE", 2);
				break;
			case Encoding::Utf16BE:
				_stream->Write((void*)"\xFE\xFF", 2);
				break;
			}
			encoder->Setup(_stream);
		}

		void BomEncoder::Close()
		{
			if(encoder)
			{
				encoder->Close();
				delete encoder;
				encoder=0;
			}
		}

		aint BomEncoder::Write(void* _buffer, aint _size)
		{
			return encoder->Write(_buffer, _size);
		}

/***********************************************************************
BomDecoder
***********************************************************************/

		BomDecoder::BomStream::BomStream(IStream* _stream, char* _bom, aint _bomLength)
			:stream(_stream)
			,bomPosition(0)
			,bomLength(_bomLength)
		{
			memcpy(bom, _bom, bomLength);
		}

		bool BomDecoder::BomStream::CanRead()const
		{
			return IsAvailable();
		}

		bool BomDecoder::BomStream::CanWrite()const
		{
			return false;
		}

		bool BomDecoder::BomStream::CanSeek()const
		{
			return false;
		}

		bool BomDecoder::BomStream::CanPeek()const
		{
			return false;
		}

		bool BomDecoder::BomStream::IsLimited()const
		{
			return stream!=0 && stream->IsLimited();
		}

		bool BomDecoder::BomStream::IsAvailable()const
		{
			return stream!=0 && stream->IsAvailable();
		}

		void BomDecoder::BomStream::Close()
		{
			stream=0;
		}

		pos_t BomDecoder::BomStream::Position()const
		{
			return IsAvailable()?bomPosition+stream->Position():-1;
		}

		pos_t BomDecoder::BomStream::Size()const
		{
			return -1;
		}

		void BomDecoder::BomStream::Seek(pos_t _size)
		{
			//CHECK_FAIL(L"BomDecoder::BomStream::Seek(pos_t)#Operation not supported.");
		}

		void BomDecoder::BomStream::SeekFromBegin(pos_t _size)
		{
			//CHECK_FAIL(L"BomDecoder::BomStream::SeekFromBegin(pos_t)#Operation not supported.");
		}

		void BomDecoder::BomStream::SeekFromEnd(pos_t _size)
		{
			//CHECK_FAIL(L"BomDecoder::BomStream::SeekFromEnd(pos_t)#Operation not supported.");
		}

		aint BomDecoder::BomStream::Read(void* _buffer, aint _size)
		{
			aint result=0;
			unsigned char* buffer=(unsigned char*)_buffer;
			if(bomPosition<bomLength)
			{
				aint remain=bomLength-bomPosition;
				result=remain<_size?remain:_size;
				memcpy(buffer, bom+bomPosition, result);
				buffer+=result;
				bomPosition+=result;
				_size-=result;
			}
			if(_size)
			{
				result+=stream->Read(buffer, _size);
			}
			return result;
		}

		aint BomDecoder::BomStream::Write(void* _buffer, aint _size)
		{
			//CHECK_FAIL(L"BomDecoder::BomStream::Write(void*, aint)#Operation not supported.");
			do { throw std::exception("fuck"); } while (0);
		}

		aint BomDecoder::BomStream::Peek(void* _buffer, aint _size)
		{
			do { throw std::exception("fuck"); } while (0);
			//CHECK_FAIL(L"BomDecoder::BomStream::Peek(void*, aint)#Operation not supported.");
		}

		BomDecoder::BomDecoder()
			:decoder(0)
		{
		}

		BomDecoder::~BomDecoder()
		{
			Close();
		}

		void BomDecoder::Setup(IStream* _stream)
		{
			char bom[3]={0};
			aint length=_stream->Read(bom, sizeof(bom));
			if(strncmp(bom, "\xEF\xBB\xBF", 3)==0)
			{
				decoder=new Utf8Decoder;
				stream=new BomStream(_stream, bom+3, 0);
			}
			else if(strncmp(bom, "\xFF\xFE", 2)==0)
			{
				decoder=new Utf16Decoder;
				stream=new BomStream(_stream, bom+2, 1);
			}
			else if(strncmp(bom, "\xFE\xFF", 2)==0)
			{
				decoder=new Utf16BEDecoder;
				stream=new BomStream(_stream, bom+2, 1);
			}
			else
			{
				decoder=new MbcsDecoder;
				stream=new BomStream(_stream, bom, 3);
			}
			decoder->Setup(stream);
		}

		void BomDecoder::Close()
		{
			if(decoder)
			{
				decoder->Close();
				delete decoder;
				decoder=0;
				stream->Close();
				delete stream;
				stream=0;
			}
		}

		aint BomDecoder::Read(void* _buffer, aint _size)
		{
			return decoder->Read(_buffer, _size);
		}

/***********************************************************************
CharEncoder
***********************************************************************/

		bool CanBeMbcs(unsigned char* buffer, aint size)
		{
			for(aint i=0;i<size;i++)
			{
				if(buffer[i]==0) return false;
			}
			return true;
		}

		bool CanBeUtf8(unsigned char* buffer, aint size)
		{
			for(aint i=0;i<size;i++)
			{
				unsigned char c=(unsigned char)buffer[i];
				if(c==0)
				{
					return false;
				}
				else
				{
					aint count10xxxxxx=0;
					if((c&0x80)==0x00) /* 0x0xxxxxxx */ count10xxxxxx=0;
					else if((c&0xE0)==0xC0) /* 0x110xxxxx */ count10xxxxxx=1;
					else if((c&0xF0)==0xE0) /* 0x1110xxxx */ count10xxxxxx=2;
					else if((c&0xF8)==0xF0) /* 0x11110xxx */ count10xxxxxx=3;
					else if((c&0xFC)==0xF8) /* 0x111110xx */ count10xxxxxx=4;
					else if((c&0xFE)==0xFC) /* 0x1111110x */ count10xxxxxx=5;

					if(size<=i+count10xxxxxx)
					{
						return false;
					}
					else
					{
						for(aint j=0;j<count10xxxxxx;j++)
						{
							c=(unsigned char)buffer[i+j+1];
							if((c&0xC0)!=0x80) /* 0x10xxxxxx */ return false;
						}
					}
					i+=count10xxxxxx;
				}
			}
			return true;
		}

		bool CanBeUtf16(unsigned char* buffer, aint size, bool& hitSurrogatePairs)
		{
			hitSurrogatePairs = false;
			if (size % 2 != 0) return false;
			bool needTrail = false;
			for (aint i = 0; i < size; i += 2)
			{
				auint16 c = buffer[i] + (buffer[i + 1] << 8);
				if (c == 0) return false;
				aint type = 0;
				if (0xD800 <= c && c <= 0xDBFF) type = 1;
				else if (0xDC00 <= c && c <= 0xDFFF) type = 2;
				if (needTrail)
				{
					if (type == 2)
					{
						needTrail = false;
					}
					else
					{
						return false;
					}
				}
				else
				{
					if (type == 1)
					{
						needTrail = true;
						hitSurrogatePairs = true;
					}
					else if (type != 0)
					{
						return false;
					}
				}
			}
			return !needTrail;
		}

		bool CanBeUtf16BE(unsigned char* buffer, aint size, bool& hitSurrogatePairs)
		{
			hitSurrogatePairs = false;
			if (size % 2 != 0) return false;
			bool needTrail = false;
			for (aint i = 0; i < size; i += 2)
			{
				auint16 c = buffer[i + 1] + (buffer[i] << 8);
				if (c == 0) return false;
				aint type = 0;
				if (0xD800 <= c && c <= 0xDBFF) type = 1;
				else if (0xDC00 <= c && c <= 0xDFFF) type = 2;
				if (needTrail)
				{
					if (type == 2)
					{
						needTrail = false;
					}
					else
					{
						return false;
					}
				}
				else
				{
					if (type == 1)
					{
						needTrail = true;
						hitSurrogatePairs = true;
					}
					else if (type != 0)
					{
						return false;
					}
				}
			}
			return !needTrail;
		}

#if defined VCZH_MSVC
		template<aint Count>
		bool GetEncodingResult(int(&tests)[Count], bool(&results)[Count], int test)
		{
			for (aint i = 0; i < Count; i++)
			{
				if (tests[i] & test)
				{
					if (results[i]) return true;
				}
			}
			return false;
		}
#endif

		void TestEncoding(unsigned char* buffer, aint size, Encoding& encoding, bool& containsBom)
		{
			if (size >= 3 && strncmp((char*)buffer, "\xEF\xBB\xBF", 3) == 0)
			{
				encoding = Encoding::Utf8;
				containsBom = true;
			}
			else if (size >= 2 && strncmp((char*)buffer, "\xFF\xFE", 2) == 0)
			{
				encoding = Encoding::Utf16;
				containsBom = true;
			}
			else if (size >= 2 && strncmp((char*)buffer, "\xFE\xFF", 2) == 0)
			{
				encoding = Encoding::Utf16BE;
				containsBom = true;
			}
			else
			{
				encoding = Encoding::Mbcs;
				containsBom = false;

				bool utf16HitSurrogatePairs = false;
				bool utf16BEHitSurrogatePairs = false;
				bool roughMbcs = CanBeMbcs(buffer, size);
				bool roughUtf8 = CanBeUtf8(buffer, size);
				bool roughUtf16 = CanBeUtf16(buffer, size, utf16HitSurrogatePairs);
				bool roughUtf16BE = CanBeUtf16BE(buffer, size, utf16BEHitSurrogatePairs);

				aint roughCount = (roughMbcs ? 1 : 0) + (roughUtf8 ? 1 : 0) + (roughUtf16 ? 1 : 0) + (roughUtf16BE ? 1 : 0);
				if (roughCount == 1)
				{
					if (roughUtf8) encoding = Encoding::Utf8;
					else if (roughUtf16) encoding = Encoding::Utf16;
					else if (roughUtf16BE) encoding = Encoding::Utf16BE;
				}
				else if (roughCount > 1)
				{
#if defined VCZH_MSVC
					int tests[] =
					{
						IS_TEXT_UNICODE_REVERSE_ASCII16,
						IS_TEXT_UNICODE_REVERSE_STATISTICS,
						IS_TEXT_UNICODE_REVERSE_CONTROLS,

						IS_TEXT_UNICODE_ASCII16,
						IS_TEXT_UNICODE_STATISTICS,
						IS_TEXT_UNICODE_CONTROLS,

						IS_TEXT_UNICODE_ILLEGAL_CHARS,
						IS_TEXT_UNICODE_ODD_LENGTH,
						IS_TEXT_UNICODE_NULL_BYTES,
					};

					const aint TestCount = sizeof(tests) / sizeof(*tests);
					bool results[TestCount];
					for (aint i = 0; i < TestCount; i++)
					{
						int test = tests[i];
						results[i] = IsTextUnicode(buffer, (int)size, &test) != 0;
					}

					if (size % 2 == 0
						&& !GetEncodingResult(tests, results, IS_TEXT_UNICODE_REVERSE_ASCII16)
						&& !GetEncodingResult(tests, results, IS_TEXT_UNICODE_REVERSE_STATISTICS)
						&& !GetEncodingResult(tests, results, IS_TEXT_UNICODE_REVERSE_CONTROLS)
						)
					{
						for (aint i = 0; i < size; i += 2)
						{
							unsigned char c = buffer[i];
							buffer[i] = buffer[i + 1];
							buffer[i + 1] = c;
						}
						// 3 = (count of reverse group) = (count of unicode group)
						for (aint i = 0; i < 3; i++)
						{
							int test = tests[i + 3];
							results[i] = IsTextUnicode(buffer, (int)size, &test) != 0;
						}
						for (aint i = 0; i < size; i += 2)
						{
							unsigned char c = buffer[i];
							buffer[i] = buffer[i + 1];
							buffer[i + 1] = c;
						}
					}

					if (GetEncodingResult(tests, results, IS_TEXT_UNICODE_NOT_UNICODE_MASK))
					{
						if (GetEncodingResult(tests, results, IS_TEXT_UNICODE_NOT_ASCII_MASK))
						{
							encoding = BomEncoder::Utf8;
						}
						else if (roughUtf8 || !roughMbcs)
						{
							encoding = BomEncoder::Utf8;
						}
					}
					else if (GetEncodingResult(tests, results, IS_TEXT_UNICODE_ASCII16))
					{
						encoding = BomEncoder::Utf16;
					}
					else if (GetEncodingResult(tests, results, IS_TEXT_UNICODE_REVERSE_ASCII16))
					{
						encoding = BomEncoder::Utf16BE;
					}
					else if (GetEncodingResult(tests, results, IS_TEXT_UNICODE_CONTROLS))
					{
						encoding = BomEncoder::Utf16;
					}
					else if (GetEncodingResult(tests, results, IS_TEXT_UNICODE_REVERSE_CONTROLS))
					{
						encoding = BomEncoder::Utf16BE;
					}
					else
					{
						if (!roughUtf8)
						{
							if (GetEncodingResult(tests, results, IS_TEXT_UNICODE_STATISTICS))
							{
								encoding = BomEncoder::Utf16;
							}
							else if (GetEncodingResult(tests, results, IS_TEXT_UNICODE_STATISTICS))
							{
								encoding = BomEncoder::Utf16BE;
							}
						}
						else if (GetEncodingResult(tests, results, IS_TEXT_UNICODE_NOT_UNICODE_MASK))
						{
							encoding = BomEncoder::Utf8;
						}
						else if (roughUtf8 || !roughMbcs)
						{
							encoding = BomEncoder::Utf8;
						}
					}
#elif defined VCZH_GCC
					if (roughUtf16 && roughUtf16BE && !roughUtf8)
					{
						if (utf16BEHitSurrogatePairs && !utf16HitSurrogatePairs)
						{
							encoding = BomEncoder::Utf16BE;
						}
						else
						{
							encoding = BomEncoder::Utf16;
						}
					}
					else
					{
						encoding = BomEncoder::Utf8;
					}
#endif
				}
			}
		}

}
