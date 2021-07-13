#include "Decoder.h"

#include "Alime/base/third_party/convert_utf/ConvertUTF.h"

#include "assert.h"


static_assert(sizeof abyte == sizeof auint8, "size error");

namespace
{
	/// <summary>
	/// 我们和C#接口保持一致，0xxxxxxxx和10xxxxxxx都视为一个完整码点
	/// </summary>
	int GetUTF8CodePointLengthByFirstByte(auint8 byte)
	{
		if ((byte & 0xF0) == 0xF0)
		{
			return 4;
		}
		else if ((byte & 0xE0) == 0xE0)
		{
			return 3;
		}
		else if ((byte & 0xC0) == 0xC0)
		{
			return 2;
		}
		else
			return 1;
	}

	bool IsValidUTF8Value(auint8 byte)
	{
		if ((byte & 0xC0) == 0x80)
			return true;
		else
			return false;
	}
}


//从C#接口来看，ut8decoder不会缓存一个完整Char
//也就是说，ut8decoder缓存字节但不缓存字符，这个特性很有意思
// 如果传递L"你好" 但buffer只有一个字符，不会缓存bytes里多余字节
//但是如果传递L"你好"前五个字节，多出的2个字节则会被缓存下来，这正是Alime里buffer一个wchar_t的原因
void NEWUTF8Decoder::Convert(abyte bytes[], int byteIndex,
	int byteCount, Char chars[], int charIndex, int charCount,
	bool flush, int& bytesUsed, int& charsUsed, bool& completed)
{
	//out
	bytesUsed = 0;
	charsUsed = 0;
	completed = false;

	wchar_t target[2];
	wchar_t* writing = (chars+ charIndex);
	auto src = (bytes+ byteIndex);
	aint byteLeft = byteCount;

	auint8 source[4];
	//aint sourceCount = 0;

	// 0 1 2 3
	if (cacheByteSize_ > 0)
	{
		int surfixBytesUsed = 0;
		int surfixBytesNeeded = 0;
		bool isLegal = false;
		GetNextCodePointLength(src, byteLeft, surfixBytesUsed, surfixBytesNeeded, isLegal);

		if (surfixBytesUsed == surfixBytesNeeded)
		{
			memcpy(source, cacheByte_, cacheByteSize_);
			memcpy(source + cacheByteSize_, src, surfixBytesUsed);
			int charNum= Convert_internal(source, surfixBytesNeeded, target);
			if (charNum> charCount)
			{
				goto ThrowStatement;
			}
			else
			{
				memcpy(writing, target, sizeof(Char) * charNum);
				writing += charNum;
				charsUsed++;
				Reset();
			}
			src += surfixBytesUsed;
			byteLeft -= surfixBytesUsed;
		}
		else
		{
			if (isLegal)//仅源数据长度不够
			{
				if (flush)
				{
					charsUsed++;
					*writing++ = -1;
					Reset();
				}
				else//cache it
				{
					memcpy(cacheByte_+cacheByteSize_, src, surfixBytesUsed);
					cacheByteSize_ += surfixBytesUsed;
				}
				bytesUsed = surfixBytesUsed;
				completed = true;
				return;
			}
			else//不合法，截断src形成一个无效字符
			{
				assert(surfixBytesUsed < byteCount);
				charsUsed++;
				*writing++ = -1;
				Reset();
				src += surfixBytesUsed;
				byteLeft -= surfixBytesUsed;
			}
		}
	}

	auint8* pos = src;
	auint8* end = src + byteLeft;// [src, end)

	while (charsUsed < charCount)//fix me, check chars' index
	{
		if (cacheAvailable_)
		{
			if(charsUsed== charCount)
				throw "The output char buffer is too small to contain the decoded characters, encoding 'Unicode (UTF-8)'";
			*writing++ = cacheChar_;
			cacheChar_ = L'0';
			charsUsed++;
			cacheAvailable_ = false;
		}
		else
		{
			if (0 == byteLeft)
				break;
			int bytesUsed = 0;
			int bytesCompleted = 0;
			bool isLegal = false;

			GetNextCodePointLength(pos, byteLeft, bytesUsed, bytesCompleted, isLegal);
			if (bytesUsed == bytesCompleted)
			{
				assert(isLegal);
				int n = Convert_internal(pos, bytesUsed, target);
				if (0 == n)
				{
					n = 1;//invalid single codepoint by BeginOfbyte--0x10xxxxx
					charsUsed++;
					*writing++ = -1;
				}
				else
				{
					charsUsed++;
					assert(n == 1 || n == 2);
					*writing++ = target[0];
					if (n == 2)
					{
						cacheAvailable_ = true;
						cacheChar_ = target[1];
					}	
				}
				pos += bytesUsed;
				byteLeft -= bytesUsed;
			}
			else
			{
				assert(bytesUsed < bytesCompleted);
				assert(byteLeft >= bytesUsed);
				if (isLegal)//仅长度不够
				{
					if (byteLeft == bytesUsed)//数据全部耗尽。这正是GetNextCodePointLength
					{
						if (flush)
						{
							charsUsed++;
							*writing++ = -1;
							pos += bytesUsed;
							byteLeft -= bytesUsed;
						}
						else//缓存
						{
							memcpy(cacheByte_ + cacheByteSize_, pos, bytesUsed);
							cacheByteSize_ += bytesUsed;
							byteLeft-= bytesUsed;
							break;
						}
					}
					else//
					{
						assert(false);
						charsUsed++;
						*writing++ = -1;
						pos += bytesUsed;
						byteLeft -= bytesUsed;
					}
				}
				else//非法
				{
					charsUsed++;
					*writing++ = -1;
					pos += bytesUsed;
					byteLeft -= bytesUsed;
				}
			}
		}
	}

	if (byteLeft == 0)
		completed = true;
	else
	{
		assert(byteLeft > 0);

		if (flush)
			return;

		int surfixBytesUsed = 0;
		int surfixBytesNeeded = 0;
		bool isLegal = false;
		GetNextCodePointLength(pos, byteLeft, surfixBytesUsed, surfixBytesNeeded, isLegal);
		
		if (byteLeft < surfixBytesNeeded)
		{
			memcpy(cacheByte_, pos, surfixBytesUsed);
			cacheByteSize_ = surfixBytesUsed;
			byteLeft -= cacheByteSize_;
			completed = true;
		}
		
	}
	bytesUsed = byteCount - byteLeft;
	completed = byteLeft == 0 ? true : false;
	return;

ThrowStatement: {//本函数
	bytesUsed = 0;
	charsUsed = 0;
	completed = false;
	throw "The output char buffer is too small to contain the decoded characters, encoding 'Unicode (UTF-8)'";
	}
	return;

}

int NEWUTF8Decoder::GetCharCount(abyte* bytes, int count, bool flush)
{
	return GetCharCount(bytes, 0, count, flush);
}

int NEWUTF8Decoder::GetCharCount(abyte bytes[], int index, int count)
{
	return GetCharCount(bytes, index, count, false);
}

//10 , 0xxxxxxx, 110xxxxx, 1110xxxx, 1111xxxx
//fix me, windows only
int NEWUTF8Decoder::GetCharCount(abyte bytes[], int index, int count, bool flush)
{
	int charsCaculated = 0;

	auint8 source[4];
	//aint sourceCount = 0;

	auto src = (bytes + index);
	aint byteLeft = count;

	// 0 1 2 3
	if (cacheByteSize_ > 0)
	{
		int surfixBytesUsed = 0;
		int surfixBytesNeeded = 0;
		bool isLegal = false;
		GetNextCodePointLength(src, byteLeft, surfixBytesUsed, surfixBytesNeeded, isLegal);

		if (surfixBytesUsed == surfixBytesNeeded)
		{
			memcpy(source, cacheByte_, cacheByteSize_);
			memcpy(source + cacheByteSize_, src, surfixBytesUsed);
			//fix
			charsCaculated += Convert_internal(source, surfixBytesNeeded, NULL);
			src += surfixBytesUsed;
			byteLeft -= surfixBytesUsed;
		}
		else
		{
			if (isLegal)//合法，意味着src长度不够
			{
				return flush ? 1 : 0;
			}
			else//不合法，阶段src
			{
				assert(surfixBytesUsed < count);
				charsCaculated++;
				src += surfixBytesUsed;
				byteLeft -= surfixBytesUsed;
			}
		}
	}

	auint8* pos = src;
	auint8* end = src+ byteLeft;// [src, end)
	while (pos!= end)
	{
		int bytesUsed = 0;
		int bytesCompleted = 0;
		bool isLegal = false;
		GetNextCodePointLength(pos, byteLeft, bytesUsed, bytesCompleted, isLegal);

		//aint legalCount = GetUTF8CodePointLengthByFirstByte(*pos);
		if (bytesUsed == bytesCompleted)
		{
			assert(isLegal);
			int n= Convert_internal(pos, bytesUsed, NULL);
			if (0 == n)
			{
				n = 1;//invalid single codepoint by BeginOfbyte--0x10xxxxx
			}
			charsCaculated += n;
			pos+=bytesUsed;
			byteLeft -= bytesUsed;
		}
		else
		{
			if (isLegal)//break?
			{
				if (byteLeft== bytesUsed)
				{
					if (flush)
					{
						charsCaculated++;
						pos += bytesUsed;
						byteLeft -= bytesUsed;
					}
					else
					{
						break;
					}
				}
				else
				{
					charsCaculated++;
					pos += bytesUsed;
					byteLeft -= bytesUsed;
				}
			}
			else
			{
				charsCaculated++;
				pos += bytesUsed;
				byteLeft -= bytesUsed;
			}
		}	
	}
	return charsCaculated;
}

void NEWUTF8Decoder::GetNextCodePointLength(abyte* bytes, int byteCount, int& bytesUsed, int& bytesCompleted, bool& isLegal)
{
	assert(byteCount > 0);

	auint8 source[4];
	aint sourceCount = 0;

	if (cacheByteSize_ > 0)
	{
		memcpy(source, cacheByte_, cacheByteSize_);
		sourceCount = GetUTF8CodePointLengthByFirstByte(*source);
		bytesCompleted = sourceCount- cacheByteSize_;
		assert(cacheByteSize_ < sourceCount&& sourceCount>1 && sourceCount <= 4 && cacheByteSize_ >= 1);

		int surfixBytesNeeded = sourceCount - cacheByteSize_;

		isLegal = true;
		for (int i = 0; i < byteCount; ++i)
		{
			if (IsValidUTF8Value(bytes[i]))
			{
				if (++bytesUsed == surfixBytesNeeded)
					break;
			}
			else
			{
				isLegal = false;
				break;
			}
		}
	}
	else
	{
		bytesCompleted = GetUTF8CodePointLengthByFirstByte(*bytes);
		if (1 == bytesCompleted)
		{
			isLegal = true;
			bytesUsed = 1;
			return;
		}
		else
		{
			isLegal = true;
			bytesUsed += 1;
			int i = 1;
			for (; i < byteCount; ++i)
			{
				if (IsValidUTF8Value(bytes[i]))
				{
					bytesUsed++;
					if (i == bytesCompleted-1)
						break;
				}
				else
				{
					isLegal = false;
					break;
				}
			}
		}
	}
}

void NEWUTF8Decoder::ClearCacheChar()
{
	cacheAvailable_ = false;
	cacheChar_ = 0;
}

void NEWUTF8Decoder::ClearCacheBytes()
{
	memset(cacheByte_, 0, sizeof(cacheByte_));
	cacheByteSize_ = 0;
}

void NEWUTF8Decoder::Reset()
{
	ClearCacheChar();
	ClearCacheBytes();
}

int NEWUTF8Decoder::Convert_internal(abyte* bytes, int byteCount, Char chars[2])
{
	UTF16 temp[2];
	if (!chars)
	{
		chars = reinterpret_cast<Char*>(temp);
	}

	const UTF8* sourceStart = reinterpret_cast<const UTF8*> (bytes);
	const UTF8* sourceEnd = reinterpret_cast<const UTF8*> (bytes + byteCount);

	UTF16* targetU16 = reinterpret_cast<UTF16*>(chars);
	UTF16* targetEndU16 = reinterpret_cast<UTF16*>(chars + 2);

	ConversionResult ret = ConvertUTF8toUTF16(
		&sourceStart, sourceEnd,
		&targetU16, targetEndU16, lenientConversion);

	int targetCount = reinterpret_cast<wchar_t*>(targetU16) - chars;
	wchar_t test = chars[0];
	if (ret == conversionOK)
		return targetCount;
	else
		return 0;
}
