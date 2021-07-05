#include "Decoder.h"

#include "Alime/base/third_party/convert_utf/ConvertUTF.h"

#include "assert.h"


static_assert(sizeof abyte == sizeof auint8, "size error");

namespace
{
	int GetUTF8CodePointLengthByFirstByte()
	{
		return 1;
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

#ifdef OS_WIN
	auint8 source[4];//本次
	aint sourceCount = 0;

	wchar_t target[2];
	wchar_t* writing = (chars+ charIndex);
	auto src = (bytes+ byteIndex);
	aint byteLeft = byteCount;

	if (cacheByteSize_ > 0)
	{
		memcpy(source, cacheByte_, cacheByteSize_);
		if ((*source & 0xF0) == 0xF0)
		{
			sourceCount = 4;
		}
		else if ((*source & 0xE0) == 0xE0)
		{
			sourceCount = 3;
		}
		else if ((*source & 0xC0) == 0xC0)
		{
			sourceCount = 2;
		}
		else
			assert(false);

		int cacheByteSurfix = sourceCount - cacheByteSize_;
		assert(cacheByteSurfix>0);

		memcpy(cacheByte_ + cacheByteSize_, src, cacheByteSurfix);

		int nRet =Convert_internal(cacheByte_, sourceCount, target);
		byteLeft -= cacheByteSurfix;
		src += cacheByteSurfix;
		//bytesUsed += cacheByteSurfix;
		if (1 == nRet)
		{
			*writing++ = target[0];
			charsUsed++;
		}
		else if (2 == nRet)
		{
			*writing++ = target[0];
			charsUsed++;
			cacheAvailable_ = true;
			cacheChar_ = target[1];
		}
		else
		{
			assert(false);
		}
		cacheByteSize_ = 0;
	}


	
	while (charsUsed < charCount)//fix me, check chars' index
	{
		if (cacheAvailable_)
		{
			*writing++ = cacheChar_;
			cacheChar_ = L'0';
			charsUsed++;
			cacheAvailable_ = false;
		}
		else
		{
			if (byteLeft < 1)
				break;

			source[0] =(auint8)(*src);

			if ((*source & 0xF0) == 0xF0)
			{
				if (byteLeft < 4)
					break;
				sourceCount = 4;
			}
			else if ((*source & 0xE0) == 0xE0)
			{
				if (byteLeft < 3)
					break;
				sourceCount = 3;
			}
			else if ((*source & 0xC0) == 0xC0)
			{
				if (byteLeft < 3)
					break;
				sourceCount = 2;
			}
			else
			{
				sourceCount = 1;
			}
			memcpy(source, src, sourceCount);
			src += sourceCount; byteLeft-= sourceCount;
			
			int nChar = Convert_internal(source, sourceCount, target);
			if (1 == nChar)
				*writing++ = target[0];
			else if (2 == nChar)
			{
				*writing++ = target[0];
				cacheAvailable_ = true;
				cacheChar_ = target[1];
			}
			else
				break;
			charsUsed++;
		}
	}

	if (byteLeft == 0)
	{
		completed = true;
		cacheByteSize_ = 0;
	}
	else
	{
		assert(byteLeft > 0);

		if (flush)
			return;

		auto fb = (auint8)(*src);
		if ((*source & 0xF0) == 0xF0 && byteLeft < 4)
		{
			cacheByteSize_ = byteLeft;
		}
		else if ((*source & 0xE0) == 0xE0 && byteLeft < 3)
		{
			cacheByteSize_ = byteLeft;
		}
		else if ((*source & 0xC0) == 0xC0 && byteLeft < 2)
		{
			cacheByteSize_ = byteLeft;
		}
		memcpy(cacheByte_, src, cacheByteSize_);
	}
	bytesUsed = byteCount - byteLeft+ cacheByteSize_;
	return;
#else

#endif
}

//fix me, windows only
int NEWUTF8Decoder::GetCharCount(abyte bytes[], int index, int count, bool flush)
{
	int charsUsed = 0;

	auint8 source[4];
	aint sourceCount = 0;

	wchar_t target[2];

	auto src = (bytes + index);
	aint byteLeft = count;

	if (cacheByteSize_ > 0)
	{
		memcpy(source, cacheByte_, cacheByteSize_);
		if ((*source & 0xF0) == 0xF0)
		{
			sourceCount = 4;
		}
		else if ((*source & 0xE0) == 0xE0)
		{
			sourceCount = 3;
		}
		else if ((*source & 0xC0) == 0xC0)
		{
			sourceCount = 2;
		}
		else
			assert(false);

		int cacheByteSurfix = sourceCount - cacheByteSize_;
		assert(cacheByteSurfix > 0);

		memcpy(cacheByte_ + cacheByteSize_, src, cacheByteSurfix);

		int nRet = Convert_internal(cacheByte_, sourceCount, target);
		byteLeft -= cacheByteSurfix;
		src += cacheByteSurfix;
		//bytesUsed += cacheByteSurfix;
		if (1 == nRet)
			charsUsed++;
		else if (2 == nRet)
			charsUsed+=2;
		else
			assert(false);
		//cacheByteSize_ = 0; // just simulate, not to clear internal buffer
	}

	while (byteLeft)//fix me, check chars' index
	{
		if (byteLeft < 1)
			break;

		source[0] = (auint8)(*src);

		if ((*source & 0xF0) == 0xF0)
		{
			if (byteLeft < 4)
				break;
			sourceCount = 4;
		}
		else if ((*source & 0xE0) == 0xE0)
		{
			if (byteLeft < 3)
				break;
			sourceCount = 3;
		}
		else if ((*source & 0xC0) == 0xC0)
		{
			if (byteLeft < 3)
				break;
			sourceCount = 2;
		}
		else
		{
			sourceCount = 1;
		}
		memcpy(source, src, sourceCount);
		src += sourceCount; byteLeft -= sourceCount;

		int nChar = Convert_internal(source, sourceCount, target);
		if (1 == nChar)
			charsUsed++;
		else if (2 == nChar)
		{
			charsUsed+=2;
		}
		else
			break;
	}
	return charsUsed;
}

int NEWUTF8Decoder::Convert_internal(abyte* bytes, int byteCount, Char chars[2])
{
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
